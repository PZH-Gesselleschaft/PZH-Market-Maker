// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <reymon/reymm/connect/connect.h>
#include <reymon/reymm/connect/kraken.h>

void kraken_connect(void (kraken_initialiseren)(), reymm_st_connect (kraken_aktualisieren)(), void (kraken_freigeben)()) {

	kraken_initialiseren();
	while(kraken_aktualisieren() == REYMM_ST_CONNECT_SUCCESS);
	kraken_freigeben();

}