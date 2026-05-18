// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <reymon/reymm/reymm.h>
#include <reymon/reymm/connect/connect.h>
#include <reymon/reymm/connect/binance.h>

// ******************
// *** Structures ***
// ******************

typedef struct {
	char  *data;
	size_t size;
} reymm_buffer;

// *****************
// *** Functions ***
// *****************

static size_t reymm_write_cb(void*, size_t, size_t, void*);

static void reymm_init_history(const char*);

static void reymm_get_klines(void);

// ******************************
// *** Initialiseren Function ***
// ******************************

reymm_st_connect binance_initialiseren() {

	reymm_read_json_file();
	
	reymm_get_klines();

	stoikov.gamma         = setting.gamma;
	stoikov.session_close = time(NULL) + 86400;

	return reymm_websocket_open();
}

// ****************
// *** Callback ***
// ****************

size_t reymm_write_cb(void* ptr, size_t size, size_t nmemb, void* userdata) {
	
	reymm_buffer *buf  = (reymm_buffer*)userdata;
	size_t        real = size * nmemb;
	buf->data          = realloc(buf->data, buf->size + real + 1);
	memcpy(buf->data + buf->size, ptr, real);
	buf->size         += real;
	buf->data[buf->size] = '\0';
	return real;
}

// *****************************
// *** Initialiseren History ***
// *****************************

void reymm_init_history(const char* json) {
	yyjson_doc *doc  = yyjson_read(json, strlen(json), 0);
	yyjson_val *root = yyjson_doc_get_root(doc);

	size_t idx, max;
	yyjson_val *kline;
	yyjson_arr_foreach(root, idx, max, kline) {
		yyjson_val *close = yyjson_arr_get(kline, 4);
		double mid = atof(yyjson_get_str(close));
		stoikov.sigma.midprice[stoikov.sigma.idx % REYMM_SIGMA_WINDOW] = mid;
		stoikov.sigma.idx++;
		if (stoikov.sigma.count < REYMM_SIGMA_WINDOW) stoikov.sigma.count++;
	}

	yyjson_doc_free(doc);
}

// *******************
// *** Get History ***
// *******************

void reymm_get_klines(void) {
	CURL         *curl;
	CURLcode      res;
	reymm_buffer  buf = { NULL, 0 };

	char url[256];
	snprintf(url, sizeof(url),
		"https://api.binance.com/api/v3/klines"
		"?symbol=%s&interval=1s&limit=%d",
		setting.symbol, REYMM_SIGMA_WINDOW);

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL,           url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, reymm_write_cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA,     &buf);

	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	if (res == CURLE_OK && buf.data)
		reymm_init_history(buf.data);

	free(buf.data);
}