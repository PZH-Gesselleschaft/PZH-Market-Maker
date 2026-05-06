// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#ifndef REYMON_REYMM_UTILITIES_JSON_H
#define REYMON_REYMM_UTILITIES_JSON_H
#if defined(__cplusplus)
extern "C" {
#endif

// ************************
// *** Libraries Config ***
// ************************

#define JSMN_STATIC

// *****************
// *** Libraries ***
// *****************

#include <yyjson.h>
#include <libwebsockets.h>

// **************
// *** Macros ***
// **************

// ***************
// *** Aliases ***
// ***************

// *****************
// *** Enumerate ***
// *****************

// ******************
// *** Structures ***
// ******************

// *****************
// *** Functions ***
// *****************

extern void reymm_read_json_file();

#if defined(__cplusplus)
}
#endif
#endif // REYMON_REYMM_UTILITIES_JSON_H