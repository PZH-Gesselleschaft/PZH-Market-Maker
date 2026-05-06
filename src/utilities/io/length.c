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

size_t reymm_length_file(FILE* file) {

	if (fseek(file, 0, SEEK_END) != 0) {
		return 0; // error al mover el puntero
	}

	long length = ftell(file);
	if (length < 0) {
		return 0; // error al obtener posición
	}

	if (fseek(file, 0, SEEK_SET) != 0) {
		return 0; // error al resetear puntero
	}

	return (size_t) length;
}