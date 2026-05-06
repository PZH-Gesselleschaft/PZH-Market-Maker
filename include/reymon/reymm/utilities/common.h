// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#ifndef REYMON_REYMM_UTILITIES_COMMON_H
#define REYMON_REYMM_UTILITIES_COMMON_H
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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <termios.h>
#include <yyjson.h>

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

// ******************
// *** Structures ***
// ******************

typedef struct {

	FILE* file;
	char* content;
	yyjson_doc* doc;
	yyjson_val* root;

	char* exchange;
	char* symbol;
	char* order_type;

	size_t bid;
	size_t ask;
	size_t size;
	double gamma;
	
	char* apikey;
	char* apisecret;

	bool exit;

} reymm_setting;

// ***********************
// *** Globa Variables ***
// ***********************

extern reymm_setting setting;

// *****************
// *** Functions ***
// *****************

extern bool reymm_exit();
extern void reymm_toupper(char*);
extern void reymm_tolower(char*);

#if defined(__cplusplus)
}
#endif
#endif // REYMON_REYMM_UTILITIES_IO_H