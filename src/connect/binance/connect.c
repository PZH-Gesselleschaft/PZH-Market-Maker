// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <reymon/reymm/connect/connect.h>
#include <reymon/reymm/connect/binance.h>

void binance_connect(void (binance_initialiseren)(), reymm_st_connect (binance_aktualisieren)(), void (binance_freigeben)()) {

	binance_initialiseren();
	while(binance_aktualisieren() == REYMM_ST_CONNECT_SUCCESS);
	binance_freigeben();
}