// *********************************
// *** Author: PZH Geselleschaft ***
// *** Datum: 9 Mai 2026         ***
// *** Aktua: 14 Mai 2026        ***
// *** Lizenz: AGPL-3-or-later   ***
// *********************************

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <pzh/pzhmm/utils/string.h>

void pzh_upper(char* pc_upper) {

	for (size_t i = 0; i < strlen(pc_upper); i++) {

		char c = pc_upper[i];
		pc_upper[i] = toupper(c);

	}

}