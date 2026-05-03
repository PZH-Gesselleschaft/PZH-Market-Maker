// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#ifndef REYMON_REYMM_REYMM_H
#define REYMON_REYMM_REYMM_H
#if defined(__cplusplus)
extern "C" {
#endif

// *****************
// *** Libraries ***
// *****************

#include <stdio.h>

#include <curl/curl.h>

#include <reymon/reymm/connect/connect.h>
#include <reymon/reymm/risk/mp.h>
#include <reymon/reymm/utilities/types.h>
#include <reymon/reymm/utilities/io.h>
#include <reymon/reymm/utilities/json.h>

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

	char* exchange;
	char* symbol;
	char* order_type;
	size_t bid;
	size_t ask;
	size_t size;
	char* apikey;
	char* apisecret;

} reymm_setting;

// *****************
// *** Functions ***
// *****************

extern void             reymm_kern(void (reymm_initialiseren)(), reymm_st_connect (reymm_aktualisieren)(), void (reymm_freigeben)());
extern void             reymm_initialiseren();
extern reymm_st_connect reymm_aktualisieren();
extern void             reymm_freigeben();

#if defined(__cplusplus)
}
#endif
#endif // REYMON_REYMM_REYMM_H