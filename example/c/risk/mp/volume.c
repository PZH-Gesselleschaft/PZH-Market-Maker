// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <math.h>
#include <time.h>
#include <reymon/reymm/reymm.h>
#include <reymon/reymm/risk/mp.h>

void reymm_volume(reymm_stoikov *p_stoikov) {

	if (p_stoikov->exchange_time == 0) return;
	if (p_stoikov->bid == 0.0)         return;

	p_stoikov->sigma.midprice[p_stoikov->sigma.idx % REYMM_SIGMA_WINDOW] = reymm_mid_price(p_stoikov);
	
	p_stoikov->sigma.idx++;
	
	if (p_stoikov->sigma.count < REYMM_SIGMA_WINDOW)
		p_stoikov->sigma.count++;
}