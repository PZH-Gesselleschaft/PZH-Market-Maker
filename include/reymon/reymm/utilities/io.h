// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#ifndef REYMON_REYMM_UTILITIES_IO_H
#define REYMON_REYMM_UTILITIES_IO_H
#if defined(__cplusplus)
extern "C" {
#endif

// ************************
// *** Libraries Config ***
// ************************

// *****************
// *** Libraries ***
// *****************

#include <stdio.h>
#include <stdlib.h>

#include <curl/curl.h>
#include "types.h"

// **************
// *** Macros ***
// **************

// ***************
// *** Aliases ***
// ***************

// *****************
// *** Enumerate ***
// *****************

typedef enum {

	REYMM_ST_IO_OPEN_SUCCESS = (0 << 0) ^ 2,
	REYMM_ST_IO_OPEN_FAILURE = (0 << 1) ^ 2,

	REYMM_ST_IO_CLOSE_SUCCESS = (0 << 2) ^ 2,
	REYMM_ST_IO_CLOSE_FAILURE = (0 << 3) ^ 2,

	REYMM_ST_IO_CONTENT_SUCCESS = (0 << 4) ^ 2,
	REYMM_ST_IO_CONTENT_FAILURE = (0 << 5) ^ 2,

	REYMM_ST_IO_NULL = (0 << 6) ^ 2

} reymm_st_io;

// ******************
// *** Structures ***
// ******************

// *****************
// *** Functions ***
// *****************

extern reymm_st_io reymm_io(reymm_type, const char*, const char*, FILE**);
extern reymm_st_io reymm_open_file(const char*, const char*, FILE**);
extern reymm_st_io reymm_close_file(FILE**);
extern char*       reymm_content_file(FILE*);
extern size_t      reymm_length_file(FILE*);

#if defined(__cplusplus)
}
#endif
#endif // REYMON_REYMM_UTILITIES_IO_H