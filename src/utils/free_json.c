// *********************************
// *** Author: PZH Geselleschaft ***
// *** Datum: 14 Mai 2026        ***
// *** Aktua: 14 Mai 2026        ***
// *** Lizenz: AGPL-3-or-later   ***
// *********************************

#include <yyjson.h>
#include <pzh/pzhmm/utils/json.h>

void pzh_free_json(pzh_json* json) {

// *********************
// *** Free Document ***
// *********************

	if (json -> doc)
		yyjson_doc_free(json -> doc);
}