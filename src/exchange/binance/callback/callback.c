// ********************************
// *** Author: PZH Gesellschaft ***
// *** Datum: 14. Mai. 2026     ***
// *** Aktua: 14. Mai. 2026     ***
// *** Lizenz: AGPL-3-or-later  ***
// ********************************

#include <stdint.h>
#include <libwebsockets.h>
#include <pzh/pzhmm/pzhmm.h>
#include <pzh/pzhmm/exchange/binance.h>

// ************************
// *** Static Variables ***
// ************************

static pzh_json json;
static char* spc_symbol = NULL;
static bool stream_suscribe = false;
static pzh_exchange_settings settings;

// **************************
// *** External Variables ***
// **************************

extern bool keep_orders;
extern binance_ws_connect stream;
extern binance_ws_connect orders;
extern binance_ws_connect websocket;

// ****************
// *** Callback ***
// ****************

int binance_callback(struct lws* wsi, enum lws_callback_reasons reason,  void* user, void* in, size_t length) {

	int i_reason = (int) reason;

	switch (i_reason) {

		case LWS_CALLBACK_CLIENT_ESTABLISHED: {

		// **********************************
		// *** Establish important inform ***
		// **********************************

			websocket.exit  = false;
			stream_suscribe = false;

			pzh_init_json(&json);

		// *****************************************
		// *** Inform the user of the connection ***
		// *****************************************

			puts("Connection Established");

		// *****************************************
		// *** Get information from binance.json ***
		// *****************************************

			pzh_read_json(&json, PZH_FILE, "config/binance.json");
			if (json.doc == NULL) {
				puts("Not found config/binance.json");
				return -1;
			}

		// ****************
		// *** Settings ***
		// ****************

			char* symbol = (char*) yyjson_get_str(yyjson_obj_get(json.root, "symbol"));
			if (!symbol) {
				puts("Enter the symbol");
				return -1;
			}

			char* apikey = (char*) yyjson_get_str(yyjson_obj_get(json.root, "apikey"));
			if (!symbol) {
				puts("Enter the apikey");
				return -1;
			}

			char* apisecret = (char*) yyjson_get_str(yyjson_obj_get(json.root, "apisecret"));
			if (!symbol) {
				puts("Enter the apisecret");
				return -1;
			}

			settings.bid   = yyjson_get_uint(yyjson_obj_get(json.root, "bid"));
			settings.ask   = yyjson_get_uint(yyjson_obj_get(json.root, "ask"));
			settings.size  = yyjson_get_num(yyjson_obj_get(json.root, "size"));
			settings.limit = yyjson_get_uint(yyjson_obj_get(json.root, "limit"));

			settings.gamma = yyjson_get_num(yyjson_obj_get(json.root, "gamma"));

			snprintf(settings.symbol, sizeof(settings.symbol), "%s", symbol);
			snprintf(settings.apikey, sizeof(settings.apikey), "%s", apikey);
			snprintf(settings.apisecret, sizeof(settings.apisecret), "%s", apisecret);

			// *****************
			// *** Free json ***
			// *****************

			pzh_free_json(&json);

			lws_callback_on_writable(wsi);

			return 0;
		}

		// ******************
		// *** Send reply ***
		// ******************

		case LWS_CALLBACK_CLIENT_WRITEABLE: {

			// *****************************
			// *** Subscribe Market Info ***
			// *****************************

			if (stream_suscribe == false) {
				binance_subscribe(stream.wsi, &settings);
				stream_suscribe = true;
			}

			// **************
			// *** Stream ***
			// **************

			if (stream.wsi == wsi && stream_suscribe == true) {
				puts("Hola desde Stream");
			}

			// **************
			// *** Orders ***
			// **************

			if (orders.wsi == wsi && stream_suscribe == true) {
				puts("Hola desde órdenes");
			}

			return 0;
		}

		// *****************************
		// *** Update of Information ***
		// *****************************

		case LWS_CALLBACK_CLIENT_RECEIVE: {

			#ifdef PZH_DEMO
				puts((char*) in);
			#endif

			// **********************
			// *** Read JSON File ***
			// **********************

			pzh_read_json(&json, PZH_FILE, "config/binance.json");

			keep_orders    = yyjson_get_bool(yyjson_obj_get(json.root, "keep_orders"));
			spc_symbol     =(char*) yyjson_get_str(yyjson_obj_get(json.root, "symbol"));
			websocket.exit = yyjson_get_bool(yyjson_obj_get(json.root, "exit"));

			pzh_free_json(&json);

			// ******************
			// *** Exit Logic ***
			// ******************

			if (websocket.exit == true) {
				
				lws_cancel_service(lws_get_context(wsi));

				return 0;
			}

			// *********************
			// *** Change Symbol ***
			// *********************

			if (strcmp(spc_symbol, settings.symbol) != 0) {

				stream_suscribe = false;
				lws_callback_on_writable(wsi);

				return 0;
			}

			// ***********************************
			// *** Read Information of Binance ***
			// ***********************************

			pzh_read_json(&json, PZH_STRING, (char*) in);



			pzh_free_json(&json);

			return 0;
		}

		// ************************
		// *** Connection Error ***
		// ************************

		case LWS_CALLBACK_CLIENT_CONNECTION_ERROR: {

			pzh_free_json(&json);

			return 0;
		}

		// ************************
		// *** Close Connection ***
		// ************************

		case LWS_CALLBACK_CLOSED: {

			pzh_free_json(&json);

			return 0;
		}

		// ****************************************
		// *** Managing connections I don't use ***
		// ****************************************

		default: {
			return lws_callback_http_dummy(wsi, reason, user, in, length);
		}

	}
	
	return 0;
}