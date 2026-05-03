// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <stdlib.h>
#include <reymon/reymm/utilities/json.h>

jsmn_parser* reymm_open_json() {

	jsmn_parser* jsmn = malloc(sizeof(jsmn_parser));
	jsmn_init(jsmn);

	return jsmn;
}