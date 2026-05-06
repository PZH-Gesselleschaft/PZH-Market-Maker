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

void reymm_volume(reymm_stoikov* p_stoikov) {

	p_stoikov -> sigma.midprice[p_stoikov -> sigma.idx % REYMM_VOLUME_WINDOW] = reymm_mid_price(p_stoikov);
	
	p_stoikov -> sigma.idx++;
	
	if (p_stoikov -> sigma.count < REYMM_VOLUME_WINDOW)
		p_stoikov -> sigma.count++;
}