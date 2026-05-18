// *********************************
// *** Author: PZH Geselleschaft ***
// *** Date: 9 Mai 2026          ***
// *** Update: 9 Mai 2026        ***
// *** License: AGPL-3-or-later  ***
// *********************************

#ifndef PZH_PZHMM_EXCHANGE_KRAKEN_H
#define PZH_PZHMM_EXCHANGE_KRAKEN_H
#if defined(__cplusplus)
extern "C" {
#endif

// *****************
// *** Libraries ***
// *****************

#include <stdint.h>
#include <yyjson.h>
#include <libwebsockets.h>

// **************
// *** Macros ***
// **************


#define PZHMM_BUFFER_SIZE 0x1000

#if defined(PZH_DEMO)

#define BASE_REST_URL "Not Found"
#define BASE_WS_URL   "Not Found"

#define PZHMM_WS_STREAM_ADDRESS "Not Found"
#define PZHMM_WS_STREAM_PATH    "Not Found"
#define PZHMM_WS_STREAM_PORT	0

#define PZHMM_WS_ORDER_ADDRESS "Not Found"
#define PZHMM_WS_ORDER_PATH    "Not Found"
#define PZHMM_WS_ORDER_PORT	   0

#define PZHMM_WS_PROTOCOL "kraken"
#define PZHMM_WS_USER     "Reymon Kraken"

#elif defined(PZH_RELEASE)

#define BASE_REST_URL "https://api.kraken.com/0/private"
#define BASE_WS_URL   "wss://ws-auth.kraken.com/v2"

#define PZHMM_WS_STREAM_ADDRESS "ws-auth.kraken.com/v2"
#define PZHMM_WS_STREAM_PATH    "/ws/"
#define PZHMM_WS_STREAM_PORT	9443

#define PZHMM_WS_ORDER_ADDRESS "ws-auth.kraken.com/v2"
#define PZHMM_WS_ORDER_PATH    "/ws-api/v3"
#define PZHMM_WS_ORDER_PORT	   443

#define PZHMM_WS_PROTOCOL "Kraken"
#define PZHMM_WS_USER     "Reymon Kraken"

#endif

// ***************
// *** Aliases ***
// ***************

typedef long long krak_ms;

// ******************
// *** Enumerates ***
// ******************

typedef enum {

	KRAKEN_STATE_CONNECT_SUCCESS,
	KRAKEN_STATE_CONNECT_FAILURE

} kraken_state_connect;

// ******************
// *** Structures ***
// ******************

typedef struct {

	struct lws* wsi;
	struct lws_context* context;
	
	struct lws_client_connect_info   client_info;
	struct lws_context_creation_info context_creation;

} kraken_ws_connect;

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
	krak_ms timestamp;
	char   apikey[65];
	char   signature[65];

} kraken_order;

// *****************
// *** Functions ***
// *****************

extern int   kraken_callback();
extern char* kraken_payload(kraken_order* order, krak_ms);

#if defined(__cplusplus)
}
#endif
#endif // PZH_PZHMM_EXCHANGE_KRAKEN_H