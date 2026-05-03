// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>

#include <jsmn.h>
#include <reymon/reymm/reymm.h>

#include "kern.hpp"

reymm_st_connect reymm_aktualisieren() {

	if (strcmp("BINANCE", setting.exchange) == 0)
		binance_connect(
			binance_initialiseren,
			binance_aktualisieren,
			binance_freigeben
		);

	else if (strcmp("KRAKEN", setting.exchange) == 0)
		kraken_connect(
			kraken_initialiseren,
			kraken_aktualisieren,
			kraken_freigeben
		);

	return REYMM_ST_CONNECT_SUCCESS;
}