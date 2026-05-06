// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************


#include <reymon/reymm/risk/mp.h>

void reymm_quotes(reymm_stoikov* p_stoikov, double* pd_bid, double* pd_ask) {

	double rp   = reymm_reservation_price(p_stoikov);
	double half = reymm_optimal_spread(p_stoikov);

	if (half == 0) return;

	half = half / 2.0;

	*pd_bid = rp - half;
	*pd_ask = rp + half; 
}