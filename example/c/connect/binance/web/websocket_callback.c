// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <libwebsockets.h>
#include <reymon/reymm/reymm.h>
#include <reymon/reymm/risk/mp.h>
#include <reymon/reymm/utilities/json.h>
#include <reymon/reymm/connect/connect.h>
#include <reymon/reymm/connect/binance.h>

// ************************
// *** Static Variables ***
// ************************

static bool orders_pending;
static bnb_ms timestamp;

static double bid_price;
static double ask_price;


static binance_order bid_order;
static binance_order ask_order;

static size_t bid_counter;
static size_t ask_counter;

static size_t bid_send;
static size_t ask_send;

static double active_bid_price;
static double active_ask_price;

static bool cancel_pending;

static char bid_signature[65];
static char ask_signature[65];

// ************************
// *** Static Functions ***
// ************************

static void cancel_orders();

static void create_bid_order(binance_order*);
static void create_ask_order(binance_order*);
static void set_bid_order_value(binance_order*, size_t);
static void set_ask_order_value(binance_order*, size_t);

static void save_signature_quotes(binance_order*, binance_order*);

// ****************
// *** Callback ***
// ****************

int reymm_websocket_callback(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len) {

	switch ((int) reason) {
		
		case LWS_CALLBACK_CLIENT_ESTABLISHED: {
			

			
			return 0;
			break;
		}

		case LWS_CALLBACK_CLIENT_WRITEABLE: {

			if (wsi == stream.wsi) {

				if (stoikov.exchange_time == 0)
					return 0;

				reymm_quotes(&stoikov, &bid_price, &ask_price);

				//printf(
				//	"Bid Price: %.5f\n"
				//	"Ask Price: %.5f\n",
				//	bid_price,
				//	ask_price
				//);

				reymm_volume(&stoikov);
				
				stoikov.sigma_value = reymm_sigma(&stoikov);
				stoikov.kappa_value = reymm_kappa(&stoikov);

				orders_pending = true;
				lws_callback_on_writable(orders.wsi);
				return 0;
			}

			if (wsi == orders.wsi) {

				if (cancel_pending) {
				
					cancel_orders();

					return 0;
				}

				if (!orders_pending)
					return 0;
			
				orders_pending = false;

				reymm_read_json_file();

				// Bid
				if (bid_send < setting.bid) {

					bid_order.doc    = yyjson_mut_doc_new(NULL);
					bid_order.root   = yyjson_mut_obj(bid_order.doc);
					bid_order.params = yyjson_mut_obj(bid_order.doc);
					yyjson_mut_doc_set_root(bid_order.doc, bid_order.root);

					set_bid_order_value(&bid_order, bid_counter);
					timestamp = (bnb_ms)time(NULL) * 1000;
					
					reymm_signature(bid_signature, binance_payload(&bid_order, timestamp));
					
					save_signature_quotes(&bid_order, NULL);
					create_bid_order(&bid_order);

					size_t bid_json_len;
					char*  bid_json_str = yyjson_mut_write(bid_order.doc, 0, &bid_json_len);
			
					uint8_t bid_buf[LWS_PRE + bid_json_len];
					memcpy(&bid_buf[LWS_PRE], bid_json_str, bid_json_len);
					
					if (bid_price != 0.0)
						lws_write(orders.wsi, &bid_buf[LWS_PRE], bid_json_len, LWS_WRITE_TEXT);
			
					free(bid_json_str);
					yyjson_mut_doc_free(bid_order.doc);

					active_bid_price = bid_price;
					bid_send++;
					bid_counter++;
				}

				// Ask
				if (ask_send < setting.ask) {

					ask_order.doc    = yyjson_mut_doc_new(NULL);
					ask_order.root   = yyjson_mut_obj(ask_order.doc);
					ask_order.params = yyjson_mut_obj(ask_order.doc);
					yyjson_mut_doc_set_root(ask_order.doc, ask_order.root);

					set_ask_order_value(&ask_order, ask_counter);
					timestamp = (bnb_ms)time(NULL) * 1000;
				
					reymm_signature(ask_signature, binance_payload(&ask_order, timestamp));
				
					save_signature_quotes(NULL, &ask_order);
					create_ask_order(&ask_order);

					size_t ask_json_len;
					char*  ask_json_str = yyjson_mut_write(ask_order.doc, 0, &ask_json_len);
				
					unsigned char ask_buf[LWS_PRE + ask_json_len];
					memcpy(&ask_buf[LWS_PRE], ask_json_str, ask_json_len);

					if (ask_price != 0.0)
						lws_write(orders.wsi, &ask_buf[LWS_PRE], ask_json_len, LWS_WRITE_TEXT);
				
					free(ask_json_str);
					yyjson_mut_doc_free(ask_order.doc);

					active_ask_price = ask_price;
					ask_send++;
					ask_counter++;
				}

				return 0;
			}

			break;
		}

		case LWS_CALLBACK_CLIENT_RECEIVE: {

			yyjson_doc* doc  = yyjson_read((char*) in, len, 0);
			yyjson_val* root = yyjson_doc_get_root(doc);

			yyjson_val* a = yyjson_obj_get(root, "a");
			yyjson_val* b = yyjson_obj_get(root, "b");

			yyjson_val* B = yyjson_obj_get(root, "B");
			yyjson_val* A = yyjson_obj_get(root, "A");

			yyjson_val* E = yyjson_obj_get(root, "E");

			stoikov.exchange_time = (time_t) time(NULL) * 1000;

			if (A && B) {

				stoikov.bid_size = atof(yyjson_get_str(B));
				stoikov.ask_size = atof(yyjson_get_str(A));
			}

			if (E) {
				stoikov.exchange_time = (time_t) yyjson_get_uint(E);
			}

			if (a && b) {

				stoikov.bid = atof(yyjson_get_str(b));
				stoikov.ask = atof(yyjson_get_str(a));
				
				// En LWS_CALLBACK_CLIENT_RECEIVE
				if (active_bid_price != 0.0 && fabs(stoikov.bid - active_bid_price) >= 0.0001) {
	
					// Solo cancelar si hay órdenes activas
					if (bid_send > 0 || ask_send > 0)
						cancel_pending = true;

					bid_send = 0;
					ask_send = 0;
				}
			}

			yyjson_doc_free(doc);
			lws_callback_on_writable(stream.wsi);

			return 0;
			break;
		}

		case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
			printf("Error de conexión: %s\n", (char*) user);
			return 1;
			break;

		case LWS_CALLBACK_CLOSED:
			orders.wsi = NULL;
			puts("Conexión cerrada");
			return 0;
			break;
	}

	return 0;
}

// *****************
// *** Variables ***
// *****************

static bool orders_pending = false;
static bnb_ms timestamp = 0;

static double bid_price = 0.0;
static double ask_price = 0.0;

static binance_order bid_order;
static binance_order ask_order;

static size_t bid_counter = 0;
static size_t ask_counter = 2;

static size_t bid_send = 0;
static size_t ask_send = 0;

static double active_bid_price = 0.0;
static double active_ask_price = 0.0;

static bool cancel_pending = false;

static char bid_signature[65];
static char ask_signature[65];

// *********************
// *** Cancel Orders ***
// *********************


void cancel_orders() {

	cancel_pending = false;
	active_bid_price = 0.0;
	active_ask_price = 0.0;

	// Construir orden de cancelación
	yyjson_mut_doc *cdoc  = yyjson_mut_doc_new(NULL);
	yyjson_mut_val *croot = yyjson_mut_obj(cdoc);
	yyjson_mut_val *cpar  = yyjson_mut_obj(cdoc);
	yyjson_mut_doc_set_root(cdoc, croot);

	bnb_ms cts = (bnb_ms)time(NULL) * 1000;

	char cancel_sig[65];
	char cancel_payload[256];
	snprintf(cancel_payload, sizeof(cancel_payload),
		"apiKey=%s"
		"&symbol=%s"
		"&timestamp=%lld",
		setting.apikey,
		setting.symbol,
		cts
	);

	reymm_signature(cancel_sig, cancel_payload);

	yyjson_mut_obj_add_str(cdoc, croot, "id",     "Cancel-All");
	yyjson_mut_obj_add_str(cdoc, croot, "method", "openOrders.cancelAll");
	yyjson_mut_obj_add_str(cdoc, cpar,  "symbol",    setting.symbol);
	yyjson_mut_obj_add_str(cdoc, cpar,  "apiKey",    setting.apikey);
	yyjson_mut_obj_add_int(cdoc, cpar,  "timestamp", cts);
	yyjson_mut_obj_add_str(cdoc, cpar,  "signature", cancel_sig);
	yyjson_mut_obj_add_val(cdoc, croot, "params",    cpar);

	size_t clen;
	char*  cstr = yyjson_mut_write(cdoc, 0, &clen);
					
	unsigned char  cbuf[LWS_PRE + clen];
	memcpy(&cbuf[LWS_PRE], cstr, clen);
					
	lws_write(orders.wsi, &cbuf[LWS_PRE], clen, LWS_WRITE_TEXT);
					
	free(cstr);
	yyjson_mut_doc_free(cdoc);
}

// *****************
// *** Bid Order ***
// *****************

void create_bid_order(binance_order* order) {

	sprintf(order -> price, "%.5f", bid_price);

	yyjson_mut_obj_add_str(order -> doc, order -> root, "id", "Bid-Order");
	yyjson_mut_obj_add_str(order -> doc, order -> root, "method", order -> method);

	yyjson_mut_obj_add_str(order -> doc, order -> params, "symbol",      order -> symbol);
	yyjson_mut_obj_add_str(order -> doc, order -> params, "side",        order -> side);
	yyjson_mut_obj_add_str(order -> doc, order -> params, "type",        order -> type);
	yyjson_mut_obj_add_str(order -> doc, order -> params, "price",       order -> price);
	yyjson_mut_obj_add_str(order -> doc, order -> params, "quantity",    order -> quantity);
	yyjson_mut_obj_add_str(order -> doc, order -> params, "timeInForce", order -> timeInForce);
	yyjson_mut_obj_add_int(order -> doc, order -> params, "timestamp",   order -> timestamp);
	yyjson_mut_obj_add_str(order -> doc, order -> params, "apiKey",      order -> apikey);
	yyjson_mut_obj_add_str(order -> doc, order -> params, "signature",   order -> signature);

	yyjson_mut_obj_add_val(order -> doc, order -> root, "params", order -> params);
}

// *****************
// *** Ask Order ***
// *****************

void create_ask_order(binance_order* order) {

	sprintf(order -> price, "%.5f", ask_price);

	yyjson_mut_obj_add_str(order -> doc, order -> root, "id", "Ask-Order");
	yyjson_mut_obj_add_str(order -> doc, order -> root, "method", order -> method);

	yyjson_mut_obj_add_str(order -> doc, order -> params, "symbol",      order -> symbol);
	yyjson_mut_obj_add_str(order -> doc, order -> params, "side",        order -> side);
	yyjson_mut_obj_add_str(order -> doc, order -> params, "type",        order -> type);
	yyjson_mut_obj_add_str(order -> doc, order -> params, "price",       order -> price);
	yyjson_mut_obj_add_str(order -> doc, order -> params, "quantity",    order -> quantity);
	yyjson_mut_obj_add_str(order -> doc, order -> params, "timeInForce", order -> timeInForce);
	yyjson_mut_obj_add_int(order -> doc, order -> params, "timestamp",   order -> timestamp);
	yyjson_mut_obj_add_str(order -> doc, order -> params, "apiKey",      order -> apikey);
	yyjson_mut_obj_add_str(order -> doc, order -> params, "signature",   order -> signature);

	yyjson_mut_obj_add_val(order -> doc, order -> root, "params", order -> params);
}

// ****************************
// *** Set Bid Order Values ***
// ****************************

void set_bid_order_value(binance_order* order, size_t counter) {

	sprintf(order -> id, "%ld", counter);

	snprintf(order -> method, sizeof(order -> method), "%s", "order.place");
	snprintf(order -> symbol, sizeof(order -> symbol), "%s", setting.symbol);
	snprintf(order -> side,   sizeof(order -> side),   "%s", "BUY");
	snprintf(order -> type,   sizeof(order -> type),   "%s", setting.order_type);

	sprintf(order -> price,    "%.5f", bid_price);
	sprintf(order -> quantity, "%ld", setting.size);
	
	snprintf(order -> timeInForce, sizeof(order -> timeInForce), "%s", "GTC");

	order -> timestamp = timestamp;

	snprintf(order -> apikey, 65, "%s", setting.apikey);
}

// ****************************
// *** Set Ask Order Values ***
// ****************************

void set_ask_order_value(binance_order* order, size_t counter) {

	sprintf(order -> id, "%ld", counter);

	snprintf(order -> method, sizeof(order -> method), "%s", "order.place");
	snprintf(order -> symbol, sizeof(order -> symbol), "%s", setting.symbol);
	snprintf(order -> side,   sizeof(order -> side),   "%s", "SELL");
	snprintf(order -> type,   sizeof(order -> type),   "%s", setting.order_type);

	sprintf(order -> price,    "%.5f", ask_price);
	sprintf(order -> quantity, "%ld", setting.size);
	
	snprintf(order -> timeInForce, sizeof(order -> timeInForce), "%s", "GTC");

	order -> timestamp = timestamp;

	snprintf(order -> apikey, 65, "%s", setting.apikey);

}

// **********************
// *** Save signature ***
// **********************

void save_signature_quotes(binance_order *b, binance_order *a) {
	if (b) snprintf(b->signature, sizeof(b->signature), "%s", bid_signature);
	if (a) snprintf(a->signature, sizeof(a->signature), "%s", ask_signature);
}