// *********************************
// *** Author: PZH Geselleschaft ***
// *** Date: 9 Mai 2026          ***
// *** Update: 9 Mai 2026        ***
// *** License: AGPL-3-or-later  ***
// *********************************

#ifndef PZH_PZHMM_EXCHANGE_EXCHANGE_H
#define PZH_PZHMM_EXCHANGE_EXCHANGE_H
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

// ******************
// *** Structures ***
// ******************

typedef struct {

	char symbol[64];

	size_t bid;
	size_t ask;
	double size;
	size_t limit;

	double gamma;

	char apikey[128];
	char apisecret[128];

} pzh_exchange_settings;

#if defined(__cplusplus)
}
#endif
#endif // PZH_PZHMM_BÖRSE_BINANCE_H