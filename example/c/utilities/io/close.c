// **************************
// *** Author: Reymon Dev ***
// *** Date: 2 Mai 2026   ***
// *** Update: 2 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <stdio.h>
#include <reymon/reymm/utilities/io.h>

reymm_st_io reymm_close_file(FILE** file) {

	if (!*file) return REYMM_ST_IO_CLOSE_FAILURE;

	free(*file);
	*file = NULL;

	return REYMM_ST_IO_CLOSE_SUCCESS;
}