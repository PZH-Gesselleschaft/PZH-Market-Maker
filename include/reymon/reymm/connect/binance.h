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

#include "connect.h"
#include <curl/curl.h>

// **************
// *** Macros ***
// **************

#if defined(REYMM_DEMO)

#define BASE_REST_URL "https://demo-api.binance.com"
#define BASE_WS_URL   "wss://demo-ws-api.binance.com/ws-api/v3"

#elif defined(REYMM_RELEASE)

#define BASE_REST_URL "https://api.binance.com/api"
#define BASE_WS_URL   "wss://ws-api.binance.com:9443/ws-api/v3"

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
#endif // REYMON_REYMM_CONNECT_BINANCE_H