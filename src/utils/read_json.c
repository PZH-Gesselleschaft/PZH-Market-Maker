// *********************************
// *** Author: PZH Geselleschaft ***
// *** Datum: 14 Mai 2026        ***
// *** Aktua: 14 Mai 2026        ***
// *** Lizenz: AGPL-3-or-later   ***
// *********************************

#include <stdint.h>
#include <yyjson.h>
#include <pzh/pzhmm/utils/common.h>
#include <pzh/pzhmm/utils/json.h>

void pzh_read_json(pzh_json* p_json, uint32_t ui_type, char* pc_data) {;

// **********************
// *** Secure Pointer ***
// **********************

	if (!p_json) return;

// ************************************
// *** Select type of the operation ***
// ************************************

	switch (ui_type) {

		case PZH_STRING: {

		// *******************
		// *** Json String ***
		// *******************

			p_json -> doc = yyjson_read_opts(pc_data, strlen(pc_data), p_json -> flag, &p_json -> alc, &p_json -> error);

			if (!p_json -> doc) return;

		// ************************
		// *** Json String Root ***
		// ************************

			p_json -> root = yyjson_doc_get_root(p_json -> doc);

			if (p_json -> root == NULL) yyjson_doc_free(p_json -> doc);

			return;
		}

		case PZH_FILE: {

		// ****************
		// *** Document ***
		// ****************

			p_json -> doc = yyjson_read_file(pc_data, p_json -> flag, &p_json -> alc, &p_json -> error);

			if (!p_json -> doc) return;

		// *********************
		// *** Document Root ***
		// *********************

			p_json -> root = yyjson_doc_get_root(p_json -> doc);

			if (!p_json -> root) yyjson_doc_free(p_json -> doc);

			return;
		}
	}

}