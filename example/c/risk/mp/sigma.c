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

double reymm_sigma(reymm_stoikov *p_stoikov) {

	reymm_volume(p_stoikov);
	
	if (p_stoikov->sigma.count < 2)
		return 0.0;

	// ── Calcular retornos log ─────────────────────────
	size_t n = p_stoikov->sigma.count;
	double ret_sum = 0.0;
	size_t valid   = 0;

	double rets[REYMM_SIGMA_WINDOW];
	for (size_t i = 1; i < n; i++) {
		double prev = p_stoikov->sigma.midprice[i - 1];
		double curr = p_stoikov->sigma.midprice[i];
		if (prev <= 0.0 || curr <= 0.0) continue;
		rets[valid] = log(curr / prev);
		ret_sum    += rets[valid];
		valid++;
	}

	if (valid < 2) return 0.0;

	// ── Media de retornos ─────────────────────────────
	double mean = ret_sum / valid;

	// ── Varianza de retornos ──────────────────────────
	double var = 0.0;
	for (size_t i = 0; i < valid; i++)
		var += (rets[i] - mean) * (rets[i] - mean);

	double sigma = sqrt(var / (valid - 1));

	return sigma;
}