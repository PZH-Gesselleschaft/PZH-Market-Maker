// **************************
// *** Author: Reymon Dev ***
// *** Date: 2 Mai 2026   ***
// *** Update: 2 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <stdio.h>
#include <reymon/reymm/utilities/io.h>

reymm_st_io reymm_open_file(const char* name, const char* mode, FILE** file) {

	*file = fopen(name, mode);

	if (!*file) {
		printf("%p\n", (void*) *file);
		*file = NULL;
		return REYMM_ST_IO_OPEN_FAILURE;
	}

	return REYMM_ST_IO_OPEN_SUCCESS;
}