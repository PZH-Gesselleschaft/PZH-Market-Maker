// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#ifndef REYMON_REYMM_CONNECT_CONNECT_H
#define REYMON_REYMM_CONNECT_CONNECT_H
#if defined(__cplusplus)
extern "C" {
#endif

// *****************
// *** Libraries ***
// *****************

#include <curl/curl.h>

// ***************
// *** Aliases ***
// ***************

// *****************
// *** Enumerate ***
// *****************

typedef enum {

	REYMM_ST_CONNECT_SUCCESS = (0 << 0) ^ 3,
	REYMM_ST_CONNECT_FAILURE = (0 << 1) ^ 3

} reymm_st_connect;

// ******************
// *** Structures ***
// ******************

typedef struct {

	CURL* curl;
	char* ws_url;
	reymm_st_connect state;

} reymm_connect;

// ************************
// *** Global Variables ***
// ************************

// ************************
// *** Common Functions ***
// ************************

extern reymm_st_connect reymm_rest_open();
extern reymm_st_connect reymm_rest_close();

extern reymm_st_connect reymm_websocket_open();
extern reymm_st_connect reymm_websocket_close();

// *************************
// *** Binance Functions ***
// *************************

extern void             binance_initialiseren();
extern reymm_st_connect binance_aktualisieren();
extern void             binance_freigeben();

// ************************
// *** Kraken Functions ***
// ************************

extern void             kraken_initialiseren();
extern reymm_st_connect kraken_aktualisieren();
extern void             kraken_freigeben();

// *************************
// *** Connect Functions ***
// *************************

extern void binance_connect(void (binance_initialiseren)(), reymm_st_connect (binance_aktualisieren)(), void (binance_freigeben)());
extern void kraken_connect(void (kraken_initialiseren)(),   reymm_st_connect (kraken_aktualisieren)(),  void (kraken_freigeben)());

#if defined(__cplusplus)
}
#endif
#endif // REYMON_REYMM_CONNECT_CONNECT_H