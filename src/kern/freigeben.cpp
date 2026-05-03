// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <reymon/reymm/reymm.h>

#include "kern.hpp"

void reymm_freigeben() {
	
	free(setting.content);
	free(setting.exchange);

}