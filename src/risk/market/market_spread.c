// ********************************
// *** Author: PZH Gesellschaft ***
// *** Datum: 14. Mai. 2026     ***
// *** Aktua: 14. Mai. 2026     ***
// *** Lizenz: AGPL-3-or-later  ***
// ********************************

// ********************
// *** Bibliotheken ***
// ********************

#include <pzh/pzhmm/risk/market.h>

double pzhmm_market_spread(double bid, double ask) {

	return ask - bid;
}