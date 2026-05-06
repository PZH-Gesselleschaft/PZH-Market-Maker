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

static double bid_price;
static double ask_price;

static binance_order bid_order;
static binance_order ask_order;

static size_t bid_counter;
static size_t ask_counter;

static char bid_signature[65];
static char ask_signature[65];

static reymm_stoikov stoikov;

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
		
		case LWS_CALLBACK_CLIENT_ESTABLISHED:

			lws_callback_on_writable(wsi);
			break;

		case LWS_CALLBACK_CLIENT_WRITEABLE: {

			// Retrieve the information from the JSON
			reymm_read_json_file();

			// Bid Order
			bid_order.doc    = yyjson_mut_doc_new(NULL);
			bid_order.root   = yyjson_mut_obj(bid_order.doc);
			bid_order.params = yyjson_mut_obj(bid_order.doc);
			yyjson_mut_doc_set_root(bid_order.doc, bid_order.root);

			// Ask Order
			ask_order.doc    = yyjson_mut_doc_new(NULL);
			ask_order.root   = yyjson_mut_obj(ask_order.doc);
			ask_order.params = yyjson_mut_obj(ask_order.doc);
			yyjson_mut_doc_set_root(ask_order.doc, ask_order.root);

			if (bid_price != atof(bid_order.price) && ask_price != atof(ask_order.price))
				cancel_orders();

			// Calculate Prices
			reymm_quotes(&stoikov, &bid_price, &ask_price);

			printf("Spread: %f\n", reymm_optimal_spread(&stoikov));

			// Set Order Values

			set_bid_order_value(&bid_order, bid_counter);
			set_ask_order_value(&ask_order, ask_counter);

			// Create Signature

			reymm_signature(bid_signature, binance_payload(&bid_order));
			reymm_signature(ask_signature, binance_payload(&ask_order));

			// Save Signature

			save_signature_quotes(&bid_order, &ask_order);

			// Create Orders

			create_bid_order(&bid_order);
			create_ask_order(&ask_order);

			// Serializar a string

			size_t bid_json_len;
			size_t ask_json_len;

			char* bid_json_str = yyjson_mut_write(bid_order.doc, 0, &bid_json_len);
			char* ask_json_str = yyjson_mut_write(ask_order.doc, 0, &ask_json_len);
			
			unsigned char bid_buffer[LWS_PRE + bid_json_len];
			unsigned char ask_buffer[LWS_PRE + ask_json_len];
			
			memcpy(&bid_buffer[LWS_PRE], bid_json_str, bid_json_len);
			memcpy(&ask_buffer[LWS_PRE], ask_json_str, ask_json_len);

			// Send Orders

			if (bid_price != 0.0 && ask_price != 0.0)
				lws_write(orders.wsi, &bid_buffer[LWS_PRE], bid_json_len, LWS_WRITE_TEXT);

			if (ask_price != 0.0)
				lws_write(orders.wsi, &ask_buffer[LWS_PRE], ask_json_len, LWS_WRITE_TEXT);

			// Free Memory
			free(bid_json_str);
			free(ask_json_str);
			yyjson_mut_doc_free(bid_order.doc);
			yyjson_mut_doc_free(ask_order.doc);
			
			break;
		}

		case LWS_CALLBACK_CLIENT_RECEIVE: {

			yyjson_doc *doc  = yyjson_read((char*)in, len, 0);
			yyjson_val *root = yyjson_doc_get_root(doc);

			yyjson_val* b = yyjson_obj_get(root, "b");
			yyjson_val* a = yyjson_obj_get(root, "a");

			if (b && a) {

				bid_price = atof(yyjson_get_str(b));
				ask_price = atof(yyjson_get_str(a));

				ask_price = ask_price + 0.0001;

			}

//			if (b && !e) {
//				// Es bookTicker — no tiene campo "e"
//				stoikov.bid      = atof(yyjson_get_str(b));
//				stoikov.ask      = atof(yyjson_get_str(yyjson_obj_get(root, "a")));
//				stoikov.bid_size = atof(yyjson_get_str(yyjson_obj_get(root, "B")));
//				stoikov.ask_size = atof(yyjson_get_str(yyjson_obj_get(root, "A")));
//
//				// Usar timestamp local ya que bookTicker no tiene "T"
//				struct timespec ts;
//				clock_gettime(CLOCK_REALTIME, &ts);
//				stoikov.exchange_time = (int64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
//
//				lws_callback_on_writable(orders.wsi);
//
//			} else if (e) {
//				const char *event = yyjson_get_str(e);
//				if (strcmp(event, "trade") == 0) {
//					stoikov.exchange_time = yyjson_get_uint(yyjson_obj_get(root, "T"));
//					reymm_kappa(&stoikov);
//				}
//			}

//			free(root);
			yyjson_doc_free(doc);
			break;
		}

		case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
			printf("Error de conexión: %s\n", (char*) user);
			break;

		case LWS_CALLBACK_CLOSED:
			puts("Conexión cerrada");
			break;
	}

	return 0;
}

// *****************
// *** Variables ***
// *****************

static double bid_price = 0.9999;
static double ask_price = 1.0000;

static binance_order bid_order;
static binance_order ask_order;

static size_t bid_counter = 0;
static size_t ask_counter = 2;

static char bid_signature[65];
static char ask_signature[65];

static reymm_stoikov stoikov;

// *********************
// *** Cancel Orders ***
// *********************

void cancel_orders() {

}

// *****************
// *** Bid Order ***
// *****************

void create_bid_order(binance_order* order) {

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

	sprintf(order -> price,    "%.8f", bid_price);
	sprintf(order -> quantity, "%ld", setting.size);
	
	snprintf(order -> timeInForce, sizeof(order -> timeInForce), "%s", "GTC");

	order -> timestamp = 0;

	snprintf(order -> apikey,    sizeof(order -> signature), "%s", setting.apikey);
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

	sprintf(order -> price,    "%.8f", ask_price);
	sprintf(order -> quantity, "%ld", setting.size);
	
	snprintf(order -> timeInForce, sizeof(order -> timeInForce), "%s", "GTC");

	order -> timestamp = 0;

	snprintf(order -> apikey,    sizeof(order -> signature), "%s", setting.apikey);

}

// **********************
// *** Save signature ***
// **********************

void save_signature_quotes(binance_order* bid_order, binance_order* ask_order) {

	snprintf(bid_order -> signature, sizeof(bid_order -> signature), "%s", bid_signature);
	snprintf(ask_order -> signature, sizeof(ask_order -> signature), "%s", ask_signature);

}