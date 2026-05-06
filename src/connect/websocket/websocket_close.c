// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <reymon/reymm/reymm.h>

reymm_st_connect reymm_websocket_close() {
	
	lws_cancel_service(websocket.context);
	lws_context_destroy(websocket.context);

	return REYMM_ST_CONNECT_SUCCESS;
}