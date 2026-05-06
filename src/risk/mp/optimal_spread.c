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

double reymm_optimal_spread(reymm_stoikov* p_stoikov) {

	// δ* = γ·σ²·T + (2/γ)·ln(1 + γ/κ)
	double tms    = reymm_time_remaining(p_stoikov);
	double sigma = reymm_sigma(p_stoikov);
	double gamma  = setting.gamma;
	double kappa = reymm_kappa(p_stoikov);

	double risk = gamma * sigma * sigma * tms;
	
	double selection = (2.0 / gamma) * log(1.0 + gamma / kappa);
	
	return risk + selection;
}