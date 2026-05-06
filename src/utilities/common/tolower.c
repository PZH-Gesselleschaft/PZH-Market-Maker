// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <reymon/reymm/utilities/common.h>

void reymm_tolower(char* p_lower) {

	for (size_t i = 0; i < strlen(p_lower); i++) {
		char c = p_lower[i];
		p_lower[i] = tolower(c);
	}
}