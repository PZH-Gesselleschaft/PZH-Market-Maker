// **************************
// *** Author: Reymon Dev ***
// *** Date: 2 Mai 2026   ***
// *** Update: 2 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <reymon/reymm/utilities/io.h>

reymm_st_io reymm_io(reymm_type type, const char* name, const char* mode, FILE* file) {

	switch(type) {

	case REYMM_TYPE_IO_OPEN:

		return reymm_open_file(name, mode, file);

	case REYMM_TYPE_IO_CLOSE:

		return reymm_close_file(file);

	default:
		reymm_close_file(file);
		return REYMM_ST_IO_NULL;
	}

}