// *********************************
// *** Author: PZH Geselleschaft ***
// *** Date: 9 Mai 2026          ***
// *** Update: 9 Mai 2026        ***
// *** License: AGPL-3-or-later  ***
// *********************************

#ifndef PZH_PZHMM_EXCHANGE_BINANCE_H
#define PZH_PZHMM_EXCHANGE_BINANCE_H
#if defined(__cplusplus)
extern "C" {
#endif

// *****************
// *** Libraries ***
// *****************

#include <stdint.h>
#include <yyjson.h>
#include <libwebsockets.h>

#include "exchange.h"

// **************
// *** Macros ***
// **************

#if defined(PZH_DEMO)

#define BASE_REST_URL "https://demo-api.binance.com"
#define BASE_WS_URL   "demo-ws-api.binance.com/ws-api/v3"

#define PZHMM_WS_STREAM_ADDRESS "stream.binance.com"
#define PZHMM_WS_STREAM_PATH    "/ws/usdcusdt@bookTicker"
#define PZHMM_WS_STREAM_PORT	9443

#define PZHMM_WS_ORDER_ADDRESS "demo-ws-api.binance.com"
#define PZHMM_WS_ORDER_PATH    "/ws-api/v3"
#define PZHMM_WS_ORDER_PORT	   443

#define PZHMM_WS_PROTOCOL "binance"
#define PZHMM_WS_USER     "PZH Client Binance"

#elif defined(PZH_RELEASE)

#define BASE_REST_URL "https://api.binance.com/api"
#define BASE_WS_URL   "ws-api.binance.com:9443/ws-api/v3"

#define PZHMM_WS_STREAM_ADDRESS "stream.binance.com"
#define PZHMM_WS_STREAM_PATH    "/ws"
#define PZHMM_WS_STREAM_PORT	9443

#define PZHMM_WS_ORDER_ADDRESS "ws-api.binance.com"
#define PZHMM_WS_ORDER_PATH    "/ws-api/v3"
#define PZHMM_WS_ORDER_PORT	   443

#define PZHMM_WS_PROTOCOL "binance"
#define PZHMM_WS_USER     "PZH Client Binance"

#endif

// ***************
// *** Aliases ***
// ***************

typedef long long bnb_ms;

// ******************
// *** Enumerates ***
// ******************

typedef enum {

	BINANCE_STATE_CONNECT_SUCCESS,
	BINANCE_STATE_CONNECT_FAILURE

} binance_state_connect;

// ******************
// *** Structures ***
// ******************

typedef struct {

	struct lws* wsi;
	struct lws_context* context;
	
	struct lws_client_connect_info   client_info;
	struct lws_context_creation_info context_creation;

	bool exit;

} binance_ws_connect;

typedef struct {

	yyjson_mut_doc* doc;
	yyjson_mut_val* root;
	yyjson_mut_val* params;

	char   id[18];
	char   method[20];
	char   symbol[10];
	char   side[10];
	char   type[10];
	char   price[18];
	char   quantity[18];
	char   timeInForce[5];
	bnb_ms timestamp;
	char   apikey[65];
	char   signature[65];

} binance_order;

// *****************
// *** Functions ***
// *****************

extern void  binance_subscribe(struct lws* wsi, pzh_exchange_settings*);
extern char* binance_payload(binance_order* order, bnb_ms);
extern int   binance_callback(struct lws*, enum lws_callback_reasons,  void*, void*, size_t);

#if defined(__cplusplus)
}
#endif
#endif // PZH_PZHMM_BÖRSE_BINANCE_H