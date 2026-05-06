// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <reymon/reymm/reymm.h>
#include <reymon/reymm/connect/connect.h>
#include <reymon/reymm/connect/binance.h>

reymm_st_connect binance_initialiseren() {

	reymm_read_json_file();

	return reymm_websocket_open();
}