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

#include <stddef.h>
#include <libwebsockets.h>

// **************
// *** Macros ***
// **************

#define REYMM_BUFFER_SIZE 0x10000

// ***************
// *** Aliases ***
// ***************

// *****************
// *** Enumerate ***
// *****************

typedef enum {

	REYMM_ST_CONNECT_SUCCESS = (0 << 0) ^ 3,
	REYMM_ST_CONNECT_FAILURE = (1 << 1) ^ 3

} reymm_st_connect;

// ******************
// *** Structures ***
// ******************

typedef struct {

	struct lws_context_creation_info context_creation;
 	struct lws_context*              context;

} reymm_context;

typedef struct {

	struct lws* wsi;
	struct lws_client_connect_info client_info;

} reymm_connect;

// ************************
// *** Global Variables ***
// ************************

extern reymm_connect stream;
extern reymm_connect orders;
extern reymm_context websocket;
extern struct lws_protocols protocols[2];

// ************************
// *** Common Functions ***
// ************************

extern reymm_st_connect reymm_rest_open();
extern reymm_st_connect reymm_rest_close();

extern reymm_st_connect reymm_websocket_open();
extern reymm_st_connect reymm_websocket_close();

extern int reymm_websocket_callback(struct lws*, enum lws_callback_reasons, void*, void*, size_t);

// *************************
// *** Binance Functions ***
// *************************

extern reymm_st_connect binance_initialiseren();
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

extern void binance_connect(reymm_st_connect (*)(), reymm_st_connect (*)(), void (*)());
extern void kraken_connect (void (*)(),  reymm_st_connect (*)(),  void (*)());

#if defined(__cplusplus)
}
#endif
#endif // REYMON_REYMM_CONNECT_CONNECT_H