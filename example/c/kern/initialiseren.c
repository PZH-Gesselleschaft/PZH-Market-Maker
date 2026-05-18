// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <reymon/reymm/reymm.h>

void reymm_initialiseren() {

	yyjson_read_flag flg = YYJSON_READ_ALLOW_COMMENTS | YYJSON_READ_ALLOW_TRAILING_COMMAS;
	yyjson_read_err  err;

	setting.doc  = yyjson_read_file("setting.json", flg, NULL, &err);
	setting.root = yyjson_doc_get_root(setting.doc);

	reymm_read_json_file();

	reymm_toupper(setting.exchange);
}