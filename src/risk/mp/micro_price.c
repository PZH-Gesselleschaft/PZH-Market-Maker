// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <reymon/reymm/risk/mp.h>

double reymm_mp(reymm_mp_t* p_mp) {

	/**
	 * Den Compiler so lenken,
	 * dass er Code mit maximaler
	 * Effizienz generiert
	*/

	double M = reymm_mid_price(p_mp);
	double G = reymm_adjustment(p_mp);

	double mp = M + G;

	return mp;
}