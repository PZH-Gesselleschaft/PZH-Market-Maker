// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <libwebsockets.h>
#include <reymon/reymm/reymm.h>
#include <reymon/reymm/utilities/common.h>
#include <reymon/reymm/utilities/json.h>
#include <reymon/reymm/connect/connect.h>
#include <reymon/reymm/connect/binance.h>


void websocket_concat(char (*symbol)[], char (*stream_path)[], size_t length) {

	strncpy(*symbol, setting.symbol, length);
	reymm_tolower(*symbol);

	strcat(*stream_path, REYMM_WS_STREAM_PATH);
	
	strcat(*stream_path, *symbol);
	strcat(*stream_path, "@bookTicker/");

	strcat(*stream_path, *symbol);
	strcat(*stream_path, "@trade/");

	strcat(*stream_path, *symbol);
	strcat(*stream_path, "@depth20");
}