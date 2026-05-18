// *********************************
// *** Author: PZH Geselleschaft ***
// *** Date: 15 Mai 2026         ***
// *** Update: 15 Mai 2026       ***
// *** License: AGPL-3-or-later  ***
// *********************************

#include <math.h>
#include <pzh/pzhmm/utils/string.h>
#include <pzh/pzhmm/exchange/binance.h>

static size_t id = 0;

void binance_subscribe(struct lws* wsi, pzh_exchange_settings* settings) {

	// ************
	// *** Vars ***
	// ************

	char symbol[0xFF];

	char depth[512];
	// char bookTicker[0xFF];
	char miniTicker[512];

	// ******************
	// *** Set symbol ***
	// ******************

	snprintf(symbol, sizeof(symbol), "%s", settings -> symbol);

	pzh_lower(symbol);

	// *************************
	// *** Set subscriptions ***
	// *************************

	snprintf(depth,      sizeof(depth),      "%s@depth20@100ms", symbol);
	snprintf(miniTicker, sizeof(miniTicker), "%s@miniTicker",    symbol);

	// *******************
	// *** Create Json ***
	// *******************

	yyjson_mut_doc* doc  = yyjson_mut_doc_new(NULL);
	yyjson_mut_val* root = yyjson_mut_obj(doc);
	yyjson_mut_doc_set_root(doc, root);

	yyjson_mut_obj_add_str(doc, root, "method", "SUBSCRIBE");
	
	yyjson_mut_val* arr = yyjson_mut_obj_add_arr(doc, root, "params");

	yyjson_mut_arr_add_str(doc, arr, depth);
	yyjson_mut_arr_add_str(doc, arr, miniTicker);

	yyjson_mut_obj_add_uint(doc, root, "id", id);

	id++;

	// *************************
	// *** Send subscription ***
	// *************************

	size_t length;
	char*  stream = yyjson_mut_write(doc, 0, &length);

	uint8_t buffer[LWS_PRE + length];

	memcpy(buffer + LWS_PRE, stream, length);

	lws_write(wsi, buffer + LWS_PRE, length, LWS_WRITE_TEXT);

	// ************
	// *** Free ***
	// ************

	free(stream);
	yyjson_mut_doc_free(doc);
}