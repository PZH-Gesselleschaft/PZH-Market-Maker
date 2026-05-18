// *********************************
// *** Author: PZH Geselleschaft ***
// *** Datum: 09 Mai 2026        ***
// *** Aktua: 14 Mai 2026        ***
// *** Lizenz: AGPL-3-or-later   ***
// *********************************

#ifndef PZH_PZHMM_PZHMM_H
#define PZH_PZHMM_PZHMM_H
#ifdef __cplusplus
extern "C" {
#endif

// ********************
// *** Bibliotheken ***
// ********************

#include "utils/common.h"
#include "utils/json.h"
#include "utils/string.h"
#include "utils/security.h"

#include "risk/inventory.h"
#include "risk/market.h"
#include "risk/microprice.h"

// ******************
// *** Funktionen ***
// ******************

extern void* binance_connect(void*);
extern void* kraken_connect(void*);

#ifdef __cplusplus
}
#endif
#endif // PZH_PZHMM_PZHMM_H