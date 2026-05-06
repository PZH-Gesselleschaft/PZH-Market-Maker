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

double reymm_kappa(reymm_stoikov* p_stoikov) {

	double kappa;
	time_t exchange_time = p_stoikov -> exchange_time;
	
	p_stoikov -> kappa.times[p_stoikov -> kappa.idx % REYMM_KAPPA_WINDOW] = exchange_time;
	p_stoikov -> kappa.idx++;
	
	if (p_stoikov -> kappa.count < REYMM_KAPPA_WINDOW)
		p_stoikov -> kappa.count++;

	if (p_stoikov -> kappa.count < 2) {
		kappa = 1.0;
		return kappa;
	}

	double sum = 0.0;
	int n = p_stoikov -> kappa.count < REYMM_KAPPA_WINDOW ? p_stoikov -> kappa.count : REYMM_KAPPA_WINDOW;
	
	for (int i = 1; i < n; i++) {

		int a = (p_stoikov -> kappa.idx - i)     % REYMM_KAPPA_WINDOW;
		int b = (p_stoikov -> kappa.idx - i - 1) % REYMM_KAPPA_WINDOW;
		
		sum += difftime(p_stoikov -> kappa.times[a], p_stoikov -> kappa.times[b]);
	}

	double mean = sum / (n - 1);
	
	kappa = mean > 0.0 ? 1.0 / mean : 1.0;

	return kappa;
}