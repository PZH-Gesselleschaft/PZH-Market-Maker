// *********************************
// *** Author: PZH Geselleschaft ***
// *** Datum: 17 Mai 2026        ***
// *** Aktua: 17 Mai 2026        ***
// *** Lizenz: AGPL-3-or-later   ***
// *********************************

#include <yyjson.h>
#include <pzh/pzhmm/utils/json.h>

void pzh_init_json(pzh_json* json) {

	json -> flag = YYJSON_READ_ALLOW_COMMENTS | YYJSON_READ_ALLOW_TRAILING_COMMAS;

	yyjson_alc_pool_init(&json -> alc, json -> buffer, PZH_BUFFER_JSON);

}