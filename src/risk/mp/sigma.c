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

double reymm_sigma(reymm_stoikov* p_stoikov) {
	
	double sigma;
	reymm_volume(p_stoikov);

	if (p_stoikov -> sigma.count < 2) {
		sigma = 0.0;
		return sigma;
	}

	double sum = 0.0;

	for (size_t i = 0; i < p_stoikov -> sigma.count; i++)
		sum += p_stoikov -> sigma.midprice[i];
	
	double mean = sum / p_stoikov -> sigma.count;

	double var = 0.0;
	
	for (size_t i = 1; i < p_stoikov -> sigma.count; i++) {

		double ret = log(p_stoikov -> sigma.midprice[i] / p_stoikov -> sigma.midprice[i - 1]);
		
		var += (ret - mean) * (ret - mean);
	}
	
	sigma = sqrt(var / (p_stoikov -> sigma.count - 1));

	return sigma;
}