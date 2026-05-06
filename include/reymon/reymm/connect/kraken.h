// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#ifndef REYMON_REYMM_CONNECT_KRAKEN_H
#define REYMON_REYMM_CONNECT_KRAKEN_H
#if defined(__cplusplus)
extern "C" {
#endif

// *****************
// *** Libraries ***
// *****************

#include "connect.h"
#include <libwebsockets.h>

// **************
// *** Macros ***
// **************

#if defined(REYMM_DEMO)

#define BASE_REST_URL "Not Found"
#define BASE_WS_URL   "Not Found"

#define REYMM_WS_STREAM_ADDRESS "Not Found"
#define REYMM_WS_STREAM_PATH    "Not Found"
#define REYMM_WS_STREAM_PORT	0

#define REYMM_WS_ORDER_ADDRESS "Not Found"
#define REYMM_WS_ORDER_PATH    "Not Found"
#define REYMM_WS_ORDER_PORT	   0

#define REYMM_WS_PROTOCOL "kraken"
#define REYMM_WS_USER     "Reymon Kraken"

#elif defined(REYMM_RELEASE)

#define BASE_REST_URL "https://api.kraken.com/0/private"
#define BASE_WS_URL   "wss://ws-auth.kraken.com/v2"

#define REYMM_WS_STREAM_ADDRESS "ws-auth.kraken.com/v2"
#define REYMM_WS_STREAM_PATH    "/ws/"
#define REYMM_WS_STREAM_PORT	9443

#define REYMM_WS_ORDER_ADDRESS "ws-auth.kraken.com/v2"
#define REYMM_WS_ORDER_PATH    "/ws-api/v3"
#define REYMM_WS_ORDER_PORT	   443

#define REYMM_WS_PROTOCOL "Kraken"
#define REYMM_WS_USER     "Reymon Kraken"

#endif


// ***************
// *** Aliases ***
// ***************

// ******************
// *** Structures ***
// ******************

// *****************
// *** Functions ***
// *****************

#if defined(__cplusplus)
}
#endif
#endif // REYMON_REYMM_CONNECT_KRAKEN_H