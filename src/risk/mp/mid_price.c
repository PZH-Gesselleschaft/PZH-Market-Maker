// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <reymon/reymm/risk/mp.h>

double reymm_mid_price(reymm_stoikov* mp) {

	/**
	 * Den Compiler so lenken,
	 * dass er Code mit maximaler
	 * Effizienz generiert
	*/

	double bid = mp -> bid;
	double ask = mp -> ask;

	double local = (bid + ask);
	double mid_price = local / 2;

	return mid_price;
}