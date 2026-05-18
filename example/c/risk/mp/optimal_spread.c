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
	double sigma  = p_stoikov -> sigma_value;
	double gamma  = setting.gamma;
	double kappa  = p_stoikov -> kappa_value;

	double risk = gamma * sigma * sigma * tms;
	
	double selection = (2.0 / gamma) * log(1.0 + gamma / kappa);

	double spread = risk + selection;

	double min_spread = 0.00001;
	double max_spread = 0.00002;

	if (spread < min_spread) return min_spread;
	if (spread > max_spread) return max_spread;

	return spread;
}