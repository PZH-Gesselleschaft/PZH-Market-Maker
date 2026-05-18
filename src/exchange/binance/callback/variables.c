// ********************************
// *** Author: PZH Gesellschaft ***
// *** Datum: 16. Mai. 2026     ***
// *** Aktua: 16. Mai. 2026     ***
// *** Lizenz: AGPL-3-or-later  ***
// ********************************

#include <stdbool.h>
#include <stdint.h>
#include <libwebsockets.h>
#include <pzh/pzhmm/pzhmm.h>
#include <pzh/pzhmm/exchange/binance.h>

// ************************
// *** Global Variables ***
// ************************

bool keep_orders;
binance_ws_connect stream;
binance_ws_connect orders;
binance_ws_connect websocket;