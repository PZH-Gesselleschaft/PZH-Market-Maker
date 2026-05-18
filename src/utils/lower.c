// *********************************
// *** Author: PZH Geselleschaft ***
// *** Datum: 14 Mai 2026         ***
// *** Aktua: 14 Mai 2026        ***
// *** Lizenz: AGPL-3-or-later   ***
// *********************************

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <pzh/pzhmm/utils/string.h>

void pzh_lower(char* pc_lower) {

	for (size_t i = 0; i < strlen(pc_lower); i++) {

		char c = pc_lower[i];
		pc_lower[i] = tolower(c);

	}

}