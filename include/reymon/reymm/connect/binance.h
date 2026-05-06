// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#ifndef REYMON_REYMM_CONNECT_BINANCE_H
#define REYMON_REYMM_CONNECT_BINANCE_H
#if defined(__cplusplus)
extern "C" {
#endif

// *****************
// *** Libraries ***
// *****************

#include <stdint.h>

#include "connect.h"
#include <yyjson.h>
#include <libwebsockets.h>

// **************
// *** Macros ***
// **************

#if defined(REYMM_DEMO)

#define BASE_REST_URL "https://demo-api.binance.com"
#define BASE_WS_URL   "demo-ws-api.binance.com/ws-api/v3"

#define REYMM_WS_STREAM_ADDRESS "stream.binance.com"
#define REYMM_WS_STREAM_PATH    "/ws/"
#define REYMM_WS_STREAM_PORT	9443

#define REYMM_WS_ORDER_ADDRESS "demo-ws-api.binance.com"
#define REYMM_WS_ORDER_PATH    "/ws-api/v3"
#define REYMM_WS_ORDER_PORT	   443

#define REYMM_WS_PROTOCOL "binance"
#define REYMM_WS_USER     "Reymon Binance"

#elif defined(REYMM_RELEASE)

#define BASE_REST_URL "https://api.binance.com/api"
#define BASE_WS_URL   "ws-api.binance.com:9443/ws-api/v3"

#define REYMM_WS_STREAM_ADDRESS "stream.binance.com"
#define REYMM_WS_STREAM_PATH    "/ws/"
#define REYMM_WS_STREAM_PORT	9443

#define REYMM_WS_ORDER_ADDRESS "ws-api.binance.com"
#define REYMM_WS_ORDER_PATH    "/ws-api/v3"
#define REYMM_WS_ORDER_PORT	   443

#define REYMM_WS_PROTOCOL "binance"
#define REYMM_WS_USER     "Reymon Binance"

#endif

// ***************
// *** Aliases ***
// ***************

// ******************
// *** Structures ***
// ******************

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
	time_t timestamp;
	char   apikey[65];
	char   signature[65];

} binance_order;

// *****************
// *** Functions ***
// *****************

extern void websocket_concat(char (*)[], char (*)[], size_t);
extern int64_t binance_get_timestamp_ms();
extern char* binance_payload(binance_order* order);

#if defined(__cplusplus)
}
#endif
#endif // REYMON_REYMM_CONNECT_BINANCE_H