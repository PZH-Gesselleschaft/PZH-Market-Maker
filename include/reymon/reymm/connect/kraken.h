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
#include <curl/curl.h>

// **************
// *** Macros ***
// **************

#if defined(REYMM_DEMO)

#define BASE_REST_URL "Not Found"
#define BASE_WS_URL   "Not Found"

#elif defined(REYMM_RELEASE)

#define BASE_REST_URL "https://api.kraken.com/0/private"
#define BASE_WS_URL   "wss://ws-auth.kraken.com/v2"

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