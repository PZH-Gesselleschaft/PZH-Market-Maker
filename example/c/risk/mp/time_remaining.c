// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <stdio.h>
#include <reymon/reymm/risk/mp.h>

double reymm_time_remaining(reymm_stoikov *tp) {

    time_t exchange_sec  = tp->exchange_time / 1000; // ms → seg
    tp->session_open     = tp->session_close - 86400;
    double elapsed       = difftime(exchange_sec, tp->session_open);
    double total         = difftime(tp->session_close, tp->session_open);
    double T             = 1.0 - elapsed / total;

    return T < 0.0 ? 0.0 : T;
}