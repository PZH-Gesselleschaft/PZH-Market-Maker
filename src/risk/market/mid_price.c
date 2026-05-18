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

double pzhmm_mid_price(double bid, double ask) {

	return (bid + ask) / 2;
}