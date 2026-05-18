// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <stdio.h>
#include <reymon/reymm/risk/mp.h>

double reymm_imbalance(reymm_stoikov* mp) {
	
	/**
	 * Den Compiler so lenken,
	 * dass er Code mit maximaler
	 * Effizienz generiert
	*/

	double imbalance = 2.0;

	double bid_size = mp -> bid_size;
	double ask_size = mp -> ask_size;

	double sum = bid_size + ask_size;

	if (sum != 0)
		imbalance = bid_size / sum;

	return imbalance;	
}