// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <stdio.h>
#include <reymon/reymm/risk/mp.h>

void reymm_quotes(reymm_stoikov* p_stoikov, double* pd_bid, double* pd_ask) {

	if (p_stoikov -> kappa.count < 2)             return;
	if (p_stoikov -> sigma.count < 2)             return;
	if (p_stoikov -> exchange_time == 0)          return;
	if (p_stoikov -> bid == 0.0 || p_stoikov -> ask == 0.0) return;

	double rp   = reymm_reservation_price(p_stoikov);
	double half = reymm_optimal_spread(p_stoikov) / 2.0;

	if (!isfinite(rp)   || !isfinite(half)) return;
	if (half <= 0.0)                         return;

	*pd_bid = rp - half;
	*pd_ask = rp + half;
}