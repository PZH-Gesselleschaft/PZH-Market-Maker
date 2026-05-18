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
#include <reymon/reymm/utilities/json.h>
#include <reymon/reymm/connect/connect.h>
#include <reymon/reymm/connect/binance.h>

int reymm_websocket_callback(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {

	switch ((int) reason) {
		
		case LWS_CALLBACK_CLIENT_ESTABLISHED:
			lws_callback_on_writable(wsi);
			break;

		case LWS_CALLBACK_CLIENT_WRITEABLE: {
			// Retrieve the information from the JSON
			reymm_read_json_file();
			
			break;
		}

		case LWS_CALLBACK_CLIENT_RECEIVE:
			printf("Respuesta: %.*s\n", (int)len, (char *)in);
			break;

		case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
			printf("Error de conexión: %s\n", (char*) user);
			break;

		case LWS_CALLBACK_CLOSED:
			puts("Conexión cerrada");
			break;
	}

	return 0;
}