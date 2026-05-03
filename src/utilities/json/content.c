// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <reymon/reymm/utilities/json.h>

char* reymm_get_json(char* msg, const char* key) {

	jsmn_parser* jsmn = reymm_open_json();
	jsmntok_t    token[REYMM_TOKEN_SIZE];

	size_t kLength = strlen(key);

	size_t n = jsmn_parse(jsmn, msg, strlen(msg), token, REYMM_TOKEN_SIZE);

	for (size_t i = 1; i < n - 1; i++) {

		if (token[i].type != JSMN_STRING) continue;
		if (token[i].end - token[i].start != ((int) kLength)) continue;
		if(strncmp(msg + token[i].start, key, kLength) != 0) continue;

		free(jsmn);

		return msg + token[i + 1].start;
	}

	free(jsmn);
	return NULL;
}