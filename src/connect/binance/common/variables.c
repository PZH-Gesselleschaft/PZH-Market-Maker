// **************************
// *** Author: Reymon Dev ***
// *** Date: 4 Mai 2026   ***
// *** Update: 4 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <reymon/reymm/reymm.h>
#include <reymon/reymm/connect/binance.h>

struct lws_protocols protocols[2] = {
	{
		REYMM_WS_PROTOCOL,        // Name
		reymm_websocket_callback, // Callback
		0,                        // Per session size data
		REYMM_BUFFER_SIZE,        // Buffer Size
		0,                        // Id
		REYMM_WS_USER,            // User
		0                         // Packet Size
	},

	 { NULL, NULL, 0, 0, 0, NULL, 0 }
};