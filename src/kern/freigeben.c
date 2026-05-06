// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <reymon/reymm/reymm.h>

void reymm_freigeben() {

	free(setting.content);
	yyjson_doc_free(setting.doc);
	reymm_io(REYMM_TYPE_IO_CLOSE, "NOTHING", "NOTHING", &setting.file);

}