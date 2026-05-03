// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <reymon/reymm/risk/mp.h>

double reymm_adjustment(reymm_mp_t* mp) {

	/**
	 * Den Compiler so lenken,
	 * dass er Code mit maximaler
	 * Effizienz generiert
	*/

	double I = reymm_imbalance(mp);
	double S = reymm_spread(mp);

	double adjustment = I - 0.5;
	adjustment = adjustment * S;

	return adjustment;
}