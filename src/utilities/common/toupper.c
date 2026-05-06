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


void reymm_toupper(char* p_upper) {
	for (size_t i = 0; i < strlen(p_upper); i++) {
		char c = p_upper[i];
		p_upper[i] = toupper(c);
	}
}