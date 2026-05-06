// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <reymon/reymm/reymm.h>
#include <reymon/reymm/connect/connect.h>
#include <reymon/reymm/connect/binance.h>

reymm_st_connect reymm_websocket_open() {

	// Contexto de creación del websocket
	websocket.context_creation.port      = CONTEXT_PORT_NO_LISTEN;
	websocket.context_creation.protocols = protocols;
	websocket.context_creation.options   = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;

	// Antes de lws_create_context()
	// lws_set_log_level(LLL_ERR | LLL_WARN | LLL_NOTICE | LLL_INFO | LLL_DEBUG, NULL);

	websocket.context = lws_create_context(&websocket.context_creation);

	// Conexión al stream

	char symbol[64];
	char stream_path[512] = "";

	websocket_concat(&symbol, &stream_path, sizeof(symbol));

	stream.client_info.context        = websocket.context;
	stream.client_info.address        = REYMM_WS_STREAM_ADDRESS;
	stream.client_info.port           = REYMM_WS_STREAM_PORT;
	stream.client_info.path           = stream_path;
	stream.client_info.host           = REYMM_WS_STREAM_ADDRESS;
	stream.client_info.origin         = REYMM_WS_STREAM_ADDRESS;
	stream.client_info.ssl_connection = LCCSCF_USE_SSL;
	stream.client_info.protocol       = REYMM_WS_PROTOCOL;

	// Conexión a las órdenes
	orders.client_info.context        = websocket.context;
	orders.client_info.address        = REYMM_WS_ORDER_ADDRESS;
	orders.client_info.port           = REYMM_WS_ORDER_PORT;
	orders.client_info.path           = REYMM_WS_ORDER_PATH;
	orders.client_info.host           = REYMM_WS_ORDER_ADDRESS;
	orders.client_info.origin         = REYMM_WS_ORDER_ADDRESS;
	orders.client_info.ssl_connection = LCCSCF_USE_SSL;
	orders.client_info.protocol       = REYMM_WS_PROTOCOL;

	// Conexiones
	stream.wsi = lws_client_connect_via_info(&stream.client_info);
    orders.wsi = lws_client_connect_via_info(&orders.client_info);

	return REYMM_ST_CONNECT_SUCCESS;
}