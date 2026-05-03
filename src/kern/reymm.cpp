// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <cstdlib>
#include <reymon/reymm/reymm.h>

int main(void) {

	reymm_kern(
		reymm_initialiseren,
		reymm_aktualisieren,
		reymm_freigeben
	);
	
	return EXIT_SUCCESS;
}
