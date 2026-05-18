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

static char _payload[512];

char* binance_payload(binance_order* order, bnb_ms timestamp) {
	
	order -> timestamp = timestamp;
	
	snprintf(_payload, sizeof(_payload),
		"apiKey=%s"
		"&price=%s"
		"&quantity=%s"
		"&side=%s"
		"&symbol=%s"
		"&timeInForce=%s"
		"&timestamp=%lld"
		"&type=%s",
		setting.apikey,
		order->price,
		order->quantity,
		order->side,
		order->symbol,
		order->timeInForce,
		order->timestamp,
		order->type
	);
	
	return _payload;
}

static char _payload[512];