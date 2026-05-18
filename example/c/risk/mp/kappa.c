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

double reymm_kappa(reymm_stoikov *p_stoikov) {

	if (p_stoikov->exchange_time == 0) return 1.0;

	time_t exchange_sec = p_stoikov->exchange_time / 1000;
	p_stoikov->kappa.times[p_stoikov->kappa.idx % REYMM_KAPPA_WINDOW] = exchange_sec;
	p_stoikov->kappa.idx++;
	
	if (p_stoikov->kappa.count < REYMM_KAPPA_WINDOW) p_stoikov->kappa.count++;
	if (p_stoikov->kappa.count < 2) return 1.0;

	double sum = 0.0;
	int valid  = 0;
	int n = p_stoikov->kappa.count < REYMM_KAPPA_WINDOW 
		  ? p_stoikov->kappa.count : REYMM_KAPPA_WINDOW;

	for (int i = 1; i < n; i++) {
		int a = (p_stoikov->kappa.idx - i     + REYMM_KAPPA_WINDOW) % REYMM_KAPPA_WINDOW;
		int b = (p_stoikov->kappa.idx - i - 1 + REYMM_KAPPA_WINDOW) % REYMM_KAPPA_WINDOW;
		double diff = difftime(p_stoikov->kappa.times[a], p_stoikov->kappa.times[b]);

		// Ignorar intervalos imposibles — máximo 10 segundos entre ticks
		if (diff <= 0.0 || diff > 10.0) continue;
		sum += diff;
		valid++;
	}

	if (valid < 2) return 1.0;
	double mean = sum / valid;
	return mean > 0.0 ? 1.0 / mean : 1.0;
}