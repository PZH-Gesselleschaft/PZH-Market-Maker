// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <openssl/hmac.h>

#include <reymon/reymm/connect/binance.h>

int64_t binance_get_timestamp_ms() {
	
	struct timespec ts;
	
	clock_gettime(CLOCK_REALTIME, &ts);
	
	return (int64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}