// *********************************
// *** Author: PZH Geselleschaft ***
// *** Date: 9 Mai 2026          ***
// *** Update 9 Mai 2026         ***
// *** License: AGPL-3-or-later  ***
// *********************************

#ifndef PZH_PZHMM_HILFS_JSON_H
#define PZH_PZHMM_HILFS_JSON_H
#ifdef __cplusplus
extern "C" {
#endif

// *****************
// *** Libraries ***
// *****************

#include <yyjson.h>

// **************
// *** Macros ***
// **************

#ifndef PZH_BUFFER_JSON
#define PZH_BUFFER_JSON 0x1000
#endif

// ***************
// *** Aliases ***
// ***************

// ******************
// *** Structures ***
// ******************

typedef struct {

	char buffer[PZH_BUFFER_JSON];
	yyjson_alc alc;
	yyjson_doc* doc;
	yyjson_val* root;
	yyjson_read_err error;
	yyjson_read_flag flag;

} pzh_json;

// *****************
// *** Functions ***
// *****************

extern void pzh_init_json(pzh_json*);
extern void pzh_read_json(pzh_json*, uint32_t, char*);
extern void pzh_free_json(pzh_json*);

#ifdef __cplusplus
}
#endif
#endif // PZH_PZHMM_HILFS_JSON_H