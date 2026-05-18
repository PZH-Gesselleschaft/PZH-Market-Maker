// *********************************
// *** Author: PZH Geselleschaft ***
// *** Datum: 09 Mai 2026        ***
// *** Aktua: 14 Mai 2026        ***
// *** Lizenz: AGPL-3-or-later   ***
// *********************************

// *****************
// *** Libraries ***
// *** *************

#include <yyjson.h>
#include <libwebsockets.h>

#include <pzh/pzhmm/pzhmm.h>
#include <pzh/pzhmm/exchange/binance.h>

// ************************
// *** Static Variables ***
// ************************

static struct lws_protocols protocols[2];

// **************************
// *** External Variables ***
// **************************

extern binance_ws_connect stream;
extern binance_ws_connect orders;
extern binance_ws_connect websocket;

// ****************
// *** Function ***
// ****************

void* binance_connect(void* args) {

	if (args != NULL)
		return NULL;

	// Creation to the websocket context

	websocket.context_creation.port      = CONTEXT_PORT_NO_LISTEN;
	websocket.context_creation.protocols = protocols;
	websocket.context_creation.options   = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
	websocket.exit = false;

	// For Debug

	#ifdef PZH_DEMO
	lws_set_log_level(LLL_ERR | LLL_WARN | LLL_NOTICE | LLL_INFO | LLL_DEBUG, NULL);
	#endif

	// Create Context

	websocket.context = lws_create_context(&websocket.context_creation);

	// Connect to the stream

	stream.client_info.context        = websocket.context;
	stream.client_info.address        = PZHMM_WS_STREAM_ADDRESS;
	stream.client_info.port           = PZHMM_WS_STREAM_PORT;
	stream.client_info.path           = PZHMM_WS_STREAM_PATH;
	stream.client_info.host           = PZHMM_WS_STREAM_ADDRESS;
	stream.client_info.origin         = PZHMM_WS_STREAM_ADDRESS;
	stream.client_info.ssl_connection = LCCSCF_USE_SSL;
	stream.client_info.protocol       = PZHMM_WS_PROTOCOL;

	orders.client_info.context        = websocket.context;
	orders.client_info.address        = PZHMM_WS_STREAM_ADDRESS;
	orders.client_info.port           = PZHMM_WS_STREAM_PORT;
	orders.client_info.path           = PZHMM_WS_STREAM_PATH;
	orders.client_info.host           = PZHMM_WS_STREAM_ADDRESS;
	orders.client_info.origin         = PZHMM_WS_STREAM_ADDRESS;
	orders.client_info.ssl_connection = LCCSCF_USE_SSL;
	orders.client_info.protocol       = PZHMM_WS_PROTOCOL;

	stream.wsi = lws_client_connect_via_info(&stream.client_info);
    orders.wsi = lws_client_connect_via_info(&orders.client_info);

    // Join to the websocket
	while (websocket.exit == false)
		lws_service(websocket.context, 0);

	lws_context_destroy(websocket.context);

	return NULL;
}

// *****************
// *** Variables ***
// *****************

static struct lws_protocols protocols[2] = {
	{
		PZHMM_WS_PROTOCOL, // Name
		binance_callback,  // Callback
		0,                 // Per session size data
		PZHMM_BUFFER_SIZE, // Buffer Size
		0,                 // Id
		PZHMM_WS_USER,     // User
		0                  // Packet Size
	},

	 { NULL, NULL, 0, 0, 0, NULL, 0 }
};