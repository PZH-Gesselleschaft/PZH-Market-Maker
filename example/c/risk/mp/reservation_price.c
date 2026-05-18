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

double reymm_reservation_price(reymm_stoikov *rp) {

	// r = P_micro(2018) - q·γ·σ²·T(2008)

	double tms       = reymm_time_remaining(rp);
	double mp        = reymm_mp(rp);
	double sigma     = rp -> sigma_value;
	double inventory = rp -> inventory;
	double gamma     = setting.gamma;

	double skew = inventory * gamma * sigma * sigma * tms;

	return mp - skew;
}