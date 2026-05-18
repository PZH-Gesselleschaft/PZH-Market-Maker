// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <reymon/reymm/reymm.h>
#include <reymon/reymm/utilities/json.h>

void reymm_read_json_file() {
	yyjson_read_flag flg = YYJSON_READ_ALLOW_COMMENTS | YYJSON_READ_ALLOW_TRAILING_COMMAS;
	yyjson_read_err err;

	if (setting.content) free(setting.content);
	if (setting.doc)     yyjson_doc_free(setting.doc);

	setting.doc  = yyjson_read_file("setting.json", flg, NULL, &err);
	setting.root = yyjson_doc_get_root(setting.doc);

	setting.exchange   = (char*) yyjson_get_str(yyjson_obj_get(setting.root, "exchange"));
	setting.symbol     = (char*) yyjson_get_str(yyjson_obj_get(setting.root, "symbol"));
	setting.order_type = (char*) yyjson_get_str(yyjson_obj_get(setting.root, "ordertype"));

	setting.bid   = yyjson_get_int(yyjson_obj_get(setting.root, "bid"));
	setting.ask   = yyjson_get_int(yyjson_obj_get(setting.root, "ask"));
	setting.size  = yyjson_get_int(yyjson_obj_get(setting.root, "size"));
	setting.gamma = yyjson_get_num(yyjson_obj_get(setting.root, "gamma"));

	setting.apikey    = (char*) yyjson_get_str(yyjson_obj_get(setting.root, "apikey"));
	setting.apisecret = (char*) yyjson_get_str(yyjson_obj_get(setting.root, "apisecret"));
}