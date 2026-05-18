// **************************
// *** Author: Reymon Dev ***
// *** Date: 2 Mai 2026   ***
// *** Update: 2 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include <reymon/reymm/utilities/io.h>

char* reymm_content_file(FILE* file) {

	fseek(file, 0, SEEK_END);
	long length = ftell(file);

	char* buffer = malloc(length + 1);
	if (!buffer) return NULL;

	fseek(file, 0, SEEK_SET);
	fread(buffer, 1, length, file);

	buffer[length] = '\0';

	return buffer;
}