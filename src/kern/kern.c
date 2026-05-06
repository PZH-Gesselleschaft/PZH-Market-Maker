// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <reymon/reymm/reymm.h>

void reymm_kern(void (reymm_initialiseren)(), reymm_st_connect (reymm_aktualisieren)(), void (reymm_freigeben)()) {

	reymm_initialiseren();
	reymm_aktualisieren();
	reymm_freigeben();

}