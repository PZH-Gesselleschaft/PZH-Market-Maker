/*
 * binance_margin_ws.c
 *
 * Envía una orden de margen (cross) vía REST con libcurl,
 * luego escucha actualizaciones de ejecución en tiempo real
 * por WebSocket usando la misma librería (libcurl >= 7.86).
 *
 * Dependencias:
 *   - libcurl >= 7.86  (con soporte WebSocket compilado)
 *   - OpenSSL          (para HMAC-SHA256)
 *   - jsmn             (header-only JSON parser)
 *
 * Compilar:
 *   gcc -o main main.c -lcurl -lssl -lcrypto -Wall -O2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/select.h>

#include <curl/curl.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>

#define JSMN_STATIC
#include <jsmn.h>

/* ── Credenciales ─────────────────────────────────────────────────────────── */
#define API_KEY     "w4mcIS33KoJzzLWqDmPDWQZHLm3YRYRhfCUpeeDHl5qpi31QVtnVYwjTSOLYEcqP" // Demo API
#define SECRET_KEY  "Ujz1JdQp4PXDuCmGoMWid5U3EtKh1WKNQMWIjlqB66U2STwjIRhAl7sA7fdTVggg" // Demo API

/* ── Endpoints ────────────────────────────────────────────────────────────── */
#define REST_BASE   "https://demo-api.binance.com"
#define WS_BASE     "wss://demo-ws-api.binance.com/ws-api/v3"

/* ── Parámetros de la orden ───────────────────────────────────────────────── */
#define SYMBOL          "USDCUSDT"
#define SIDE            "SELL"
#define ORDER_TYPE      "LIMIT"
#define TIME_IN_FORCE   "GTC"
#define QUANTITY        "20000"
#define PRICE           "1.0001"
/*
 * sideEffectType:
 *   NO_SIDE_EFFECT  — sin auto-préstamo (debes tener fondos ya prestados)
 *   MARGIN_BUY      — auto-pide prestado si hace falta
 *   AUTO_REPAY      — repaga automáticamente al cerrar posición
 */
#define SIDE_EFFECT     "MARGIN_BUY"

/* ── Tamaños de buffer ────────────────────────────────────────────────────── */
#define BUF_SMALL   256
#define BUF_MED     512
#define BUF_LARGE   4096
#define JSON_TOKENS 64

/* ═══════════════════════════════════════════════════════════════════════════
 * Utilidades generales
 * ═══════════════════════════════════════════════════════════════════════════ */

/* Buffer dinámico para respuestas HTTP */
typedef struct {
	char  *data;
	size_t len;
} DynBuf;

static size_t dynbuf_write(void *ptr, size_t size, size_t nmemb, void *ud)
{
	size_t   total = size * nmemb;
	DynBuf  *b     = (DynBuf *)ud;
	char    *tmp   = realloc(b->data, b->len + total + 1);
	if (!tmp) return 0;
	b->data = tmp;
	memcpy(b->data + b->len, ptr, total);
	b->len += total;
	b->data[b->len] = '\0';
	return total;
}

/* Timestamp en milisegundos */
static long long now_ms(void)
{
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return (long long)ts.tv_sec * 1000LL + ts.tv_nsec / 1000000LL;
}

/* HMAC-SHA256 → cadena hexadecimal */
static void hmac_sha256_hex(const char *key, const char *data, char out[65])
{
	unsigned char  digest[EVP_MAX_MD_SIZE];
	unsigned int   dlen = 0;

	HMAC(EVP_sha256(),
		 key,  strlen(key),
		 (const unsigned char *)data, strlen(data),
		 digest, &dlen);

	for (unsigned int i = 0; i < dlen; i++)
		sprintf(out + i * 2, "%02x", digest[i]);
	out[dlen * 2] = '\0';
}

/* ═══════════════════════════════════════════════════════════════════════════
 * Helpers jsmn
 * ═══════════════════════════════════════════════════════════════════════════ */

/* ¿El token es igual a la cadena `s`? */
static int tok_eq(const char *json, const jsmntok_t *t, const char *s)
{
	int len = t->end - t->start;
	return t->type == JSMN_STRING
		&& (int)strlen(s) == len
		&& strncmp(json + t->start, s, len) == 0;
}

/* Copia el valor de un token a `out` (null-terminado) */
static void tok_str(const char *json, const jsmntok_t *t,
					char *out, size_t out_size)
{
	size_t len = (size_t)(t->end - t->start);
	if (len >= out_size) len = out_size - 1;
	memcpy(out, json + t->start, len);
	out[len] = '\0';
}

/*
 * Busca `key` en el objeto JSON raíz y devuelve el índice del token
 * de valor correspondiente, o -1 si no lo encuentra.
 */
static int json_find(const char *json, const jsmntok_t *toks,
					 int count, const char *key)
{
	for (int i = 1; i < count - 1; i++)
		if (tok_eq(json, &toks[i], key))
			return i + 1;
	return -1;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * PASO 1 — Obtener listenKey (User Data Stream de margen)
 *
 * POST /sapi/v1/userDataStream   → { "listenKey": "..." }
 * La clave caduca a los 60 min; hay que renovarla con PUT cada 30 min.
 * ═══════════════════════════════════════════════════════════════════════════ */
static int get_listen_key(char listen_key[BUF_SMALL])
{
	CURL    *curl = curl_easy_init();
	if (!curl) return -1;

	DynBuf resp = { malloc(1), 0 };
	resp.data[0] = '\0';

	struct curl_slist *hdrs = NULL;
	hdrs = curl_slist_append(hdrs, "X-MBX-APIKEY: " API_KEY);

	curl_easy_setopt(curl, CURLOPT_URL,
					 REST_BASE "/sapi/v1/userDataStream");
	curl_easy_setopt(curl, CURLOPT_POST,          1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS,    "");   /* cuerpo vacío */
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER,    hdrs);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dynbuf_write);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA,     &resp);

	CURLcode rc = curl_easy_perform(curl);
	curl_slist_free_all(hdrs);
	curl_easy_cleanup(curl);

	if (rc != CURLE_OK) {
		fprintf(stderr, "[listenKey] curl error: %s\n",
				curl_easy_strerror(rc));
		free(resp.data);
		return -1;
	}

	/* Parsear { "listenKey": "xxxx..." } */
	jsmn_parser  p;
	jsmntok_t    toks[16];
	jsmn_init(&p);
	int n = jsmn_parse(&p, resp.data, resp.len, toks, 16);

	int ret = -1;
	if (n > 0) {
		int vi = json_find(resp.data, toks, n, "listenKey");
		if (vi >= 0) {
			tok_str(resp.data, &toks[vi], listen_key, BUF_SMALL);
			printf("[listenKey] %s\n", listen_key);
			ret = 0;
		}
	}
	if (ret < 0)
		fprintf(stderr, "[listenKey] respuesta inesperada: %s\n", resp.data);

	free(resp.data);
	return ret;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * PASO 2 — Enviar orden de margen (REST)
 *
 * POST /sapi/v1/margin/order
 * ═══════════════════════════════════════════════════════════════════════════ */
static void print_order_response(const char *json, size_t len)
{
	jsmn_parser  p;
	jsmntok_t    toks[JSON_TOKENS];
	char         val[BUF_SMALL];

	jsmn_init(&p);
	int n = jsmn_parse(&p, json, len, toks, JSON_TOKENS);
	if (n < 0 || toks[0].type != JSMN_OBJECT) {
		fprintf(stderr, "[orden] JSON inválido: %s\n", json);
		return;
	}

	/* Campos que nos interesa mostrar */
	const char *fields[] = {
		"symbol", "orderId", "clientOrderId",
		"side",   "type",    "status",
		"price",  "origQty", "timeInForce"
	};
	int nf = (int)(sizeof(fields) / sizeof(fields[0]));

	printf("\n── Orden de margen enviada ───────────────────\n");
	for (int f = 0; f < nf; f++) {
		int vi = json_find(json, toks, n, fields[f]);
		if (vi >= 0) {
			tok_str(json, &toks[vi], val, sizeof(val));
			printf("  %-16s: %s\n", fields[f], val);
		}
	}
	printf("─────────────────────────────────────────────\n\n");
}

static int send_margin_order(void)
{
	/* 1. Construir query string sin firma */
	char     query[BUF_LARGE];
	long long ts = now_ms();

	snprintf(query, sizeof(query),
		"symbol=%s"
		"&side=%s"
		"&type=%s"
		"&timeInForce=%s"
		"&quantity=%s"
		"&price=%s"
		"&sideEffectType=%s"
		"&isIsolated=FALSE"          /* FALSE = margen cruzado */
		"&timestamp=%lld",
		SYMBOL, SIDE, ORDER_TYPE, TIME_IN_FORCE,
		QUANTITY, PRICE, SIDE_EFFECT, ts);

	/* 2. Firmar */
	char sig[65];
	hmac_sha256_hex(SECRET_KEY, query, sig);

	/* 3. Body completo */
	char body[BUF_LARGE + 80];
	snprintf(body, sizeof(body), "%s&signature=%s", query, sig);

	/* 4. Ejecutar request */
	CURL *curl = curl_easy_init();
	if (!curl) return -1;

	DynBuf resp = { malloc(1), 0 };
	resp.data[0] = '\0';

	struct curl_slist *hdrs = NULL;
	hdrs = curl_slist_append(hdrs, "X-MBX-APIKEY: " API_KEY);
	hdrs = curl_slist_append(hdrs,
							 "Content-Type: application/x-www-form-urlencoded");

	curl_easy_setopt(curl, CURLOPT_URL,
					 REST_BASE "/sapi/v1/margin/order");
	curl_easy_setopt(curl, CURLOPT_POST,          1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS,    body);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER,    hdrs);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dynbuf_write);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA,     &resp);

	CURLcode rc = curl_easy_perform(curl);
	curl_slist_free_all(hdrs);
	curl_easy_cleanup(curl);

	if (rc != CURLE_OK) {
		fprintf(stderr, "[orden] curl error: %s\n", curl_easy_strerror(rc));
		free(resp.data);
		return -1;
	}

	print_order_response(resp.data, resp.len);
	free(resp.data);
	return 0;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * PASO 3 — Escuchar actualizaciones en tiempo real (WebSocket)
 *
 * Conectamos a wss://stream.binance.com:9443/ws/<listenKey>
 * y procesamos eventos executionReport (actualizaciones de orden).
 *
 * libcurl >= 7.86: usamos CURLOPT_CONNECT_ONLY=2L para WebSocket,
 * luego curl_ws_recv / curl_ws_send en loop manual.
 * ═══════════════════════════════════════════════════════════════════════════ */

/* Interpreta un evento executionReport y lo muestra */
static void handle_execution_report(const char *json, size_t len)
{
	jsmn_parser  p;
	jsmntok_t    toks[JSON_TOKENS * 2];
	char         val[BUF_SMALL];

	jsmn_init(&p);
	int n = jsmn_parse(&p, json, len, toks, JSON_TOKENS * 2);
	if (n < 0) return;

	/* Campos relevantes del evento executionReport */
	const char *fields[] = {
		"s",   /* symbol              */
		"S",   /* side                */
		"o",   /* order type          */
		"q",   /* quantity            */
		"p",   /* price               */
		"X",   /* order status        */
		"i",   /* orderId             */
		"l",   /* last executed qty   */
		"z",   /* cumulative filled   */
		"L",   /* last executed price */
		"n",   /* commission amount   */
		"N",   /* commission asset    */
	};
	const char *labels[] = {
		"symbol", "side", "type", "qty", "price",
		"status", "orderId", "lastQty", "cumQty",
		"lastPrice", "commission", "commAsset"
	};
	int nf = (int)(sizeof(fields) / sizeof(fields[0]));

	printf("── executionReport ───────────────────────────\n");
	for (int f = 0; f < nf; f++) {
		int vi = json_find(json, toks, n, fields[f]);
		if (vi >= 0) {
			tok_str(json, &toks[vi], val, sizeof(val));
			printf("  %-12s: %s\n", labels[f], val);
		}
	}
	printf("─────────────────────────────────────────────\n\n");
}

/* Despacha el mensaje WebSocket recibido */
static void dispatch_ws_message(const char *json, size_t len)
{
	jsmn_parser  p;
	jsmntok_t    toks[8];
	char         etype[BUF_SMALL];

	jsmn_init(&p);
	int n = jsmn_parse(&p, json, len, toks, 8);
	if (n < 0) return;

	/* Todos los eventos de user data tienen campo "e" con el tipo */
	int vi = json_find(json, toks, n, "e");
	if (vi < 0) return;

	tok_str(json, &toks[vi], etype, sizeof(etype));

	if (strcmp(etype, "executionReport") == 0)
		handle_execution_report(json, len);
	else
		printf("[ws] evento: %s (ignorado)\n", etype);
}

/*
 * Recibe un frame WebSocket completo, ensamblando fragmentos si los hay.
 * Retorna CURLE_OK en éxito, CURLE_AGAIN si no hay datos todavía.
 */
static CURLcode ws_recv_frame(CURL *curl, char *buf, size_t bufsz,
							   size_t *out_len, int *is_ping)
{
	size_t                    offset = 0;
	const struct curl_ws_frame *meta  = NULL;
	CURLcode                   rc;

	*out_len = 0;
	*is_ping = 0;

	do {
		size_t got = 0;
		rc = curl_ws_recv(curl, buf + offset, bufsz - offset - 1, &got, &meta);

		if (rc == CURLE_AGAIN)
			return CURLE_AGAIN;
		if (rc != CURLE_OK)
			return rc;

		offset += got;

		if (meta->flags & CURLWS_PING) {
			*is_ping = 1;
			return CURLE_OK;
		}

	} while (meta->bytesleft > 0 && offset < bufsz - 1);

	buf[offset] = '\0';
	*out_len    = offset;
	return CURLE_OK;
}

static int listen_user_stream(const char *listen_key)
{
	/* Construir URL del stream de usuario para margen */
	char url[BUF_MED];
	snprintf(url, sizeof(url), "%s/%s", WS_BASE, listen_key);
	printf("[ws] conectando a %s\n\n", url);

	CURL *curl = curl_easy_init();
	if (!curl) return -1;

	curl_easy_setopt(curl, CURLOPT_URL,          url);
	curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);   /* modo WebSocket */

	/* Establecer conexión + handshake WebSocket */
	CURLcode rc = curl_easy_perform(curl);
	if (rc != CURLE_OK) {
		fprintf(stderr, "[ws] handshake fallido: %s\n",
				curl_easy_strerror(rc));
		curl_easy_cleanup(curl);
		return -1;
	}
	printf("[ws] conexión establecida\n\n");

	/* Obtener el file descriptor para usar con select() */
	curl_socket_t sockfd;
	curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sockfd);

	char     frame_buf[BUF_LARGE * 4];
	size_t   frame_len;
	int      is_ping;
	time_t   last_pong = time(NULL);

	/*
	 * Loop principal:
	 *  - Usamos select() con timeout de 1 s para no bloquear.
	 *  - Respondemos PONG a cada PING del servidor.
	 *  - Binance envía PING cada ~3 min; si no recibimos PONG
	 *    en 10 min desconecta.
	 */
	while (1) {
		fd_set  rset;
		struct  timeval tv = { .tv_sec = 1, .tv_usec = 0 };

		FD_ZERO(&rset);
		FD_SET(sockfd, &rset);

		int sel = select((int)sockfd + 1, &rset, NULL, NULL, &tv);
		if (sel < 0) {
			perror("select");
			break;
		}
		if (sel == 0) {
			/* Timeout: verificar si llevan > 9 min sin PONG */
			if (difftime(time(NULL), last_pong) > 540) {
				fprintf(stderr, "[ws] timeout de keepalive\n");
				break;
			}
			continue;
		}

		rc = ws_recv_frame(curl, frame_buf, sizeof(frame_buf),
						   &frame_len, &is_ping);

		if (rc == CURLE_AGAIN)
			continue;

		if (rc == CURLE_GOT_NOTHING) {
			printf("[ws] servidor cerró la conexión\n");
			break;
		}
		if (rc != CURLE_OK) {
			fprintf(stderr, "[ws] recv error: %s\n",
					curl_easy_strerror(rc));
			break;
		}

		/* Responder PONG si el servidor envió PING */
		if (is_ping) {
			size_t sent;
			curl_ws_send(curl, "", 0, &sent, 0, CURLWS_PONG);
			last_pong = time(NULL);
			printf("[ws] PING → PONG\n");
			continue;
		}

		/* Procesar frame de texto (JSON) */
		if (frame_len > 0)
			dispatch_ws_message(frame_buf, frame_len);
	}

	/* Cerrar WebSocket limpiamente */
	size_t sent;
	curl_ws_send(curl, "", 0, &sent, 0, CURLWS_CLOSE);
	curl_easy_cleanup(curl);
	return 0;
}

/* ═══════════════════════════════════════════════════════════════════════════
 * main
 * ═══════════════════════════════════════════════════════════════════════════ */
int main(void)
{
	curl_global_init(CURL_GLOBAL_ALL);

	/* 1. Obtener listenKey para el stream de usuario de margen */
	char listen_key[BUF_SMALL];
	if (get_listen_key(listen_key) < 0) {
		fprintf(stderr, "No se pudo obtener el listenKey\n");
		curl_global_cleanup();
		return 1;
	}

	/* 2. Enviar la orden de margen por REST */
	if (send_margin_order() < 0) {
		fprintf(stderr, "Error al enviar la orden\n");
		curl_global_cleanup();
		return 1;
	}

	/*
	 * 3. Escuchar actualizaciones por WebSocket.
	 *    En producción esto correría en un hilo separado
	 *    junto a un timer que renueve el listenKey cada 30 min
	 *    con PUT /sapi/v1/userDataStream.
	 */
	listen_user_stream(listen_key);

	curl_global_cleanup();
	return 0;
}