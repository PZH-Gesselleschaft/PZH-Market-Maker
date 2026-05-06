// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#include <reymon/reymm/reymm.h>
#include <reymon/reymm/connect/binance.h>

reymm_st_connect binance_aktualisieren() {

	// Check whether you are going out
	if (reymm_exit()){

		return REYMM_ST_CONNECT_FAILURE;
	}

	lws_service(websocket.context, 1000);

	return REYMM_ST_CONNECT_SUCCESS;
}