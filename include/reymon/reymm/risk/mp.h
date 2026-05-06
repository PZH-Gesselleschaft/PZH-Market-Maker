// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#ifndef REYMON_REYMM_RISK_MP_H
#define REYMON_REYMM_RISK_MP_H
#if defined(__cplusplus)
extern "C" {
#endif

// *****************
// *** Libraries ***
// *****************

#include <math.h>
#include <time.h>

// **************
// *** Macros ***
// **************

#define REYMM_SIGMA_WINDOW  100
#define REYMM_KAPPA_WINDOW  50
#define REYMM_VOLUME_WINDOW 86400

// ***************
// *** Aliases ***
// ***************

// *****************
// *** Enumerate ***
// *****************

// ******************
// *** Structures ***
// ******************

typedef struct {

	size_t idx;
	size_t count;
	double midprice[REYMM_VOLUME_WINDOW];

} reymm_sigma_t;

typedef struct {

	size_t idx;
	size_t count;
	time_t times[REYMM_KAPPA_WINDOW];

} reymm_kappa_t;

typedef struct {

	double bid; // precio del mejor bid
	double ask; // precio del mejor ask

	double bid_size; // volumen en el bid
	double ask_size; // volumen en el ask

	double gamma; // aversión al riesgo
	
	reymm_kappa_t kappa; // frecuencia de ticks del book (órdenes/segundo)
	reymm_sigma_t sigma; // volatilidad
	
	double inventory; // inventario actual

	time_t exchange_time; // timestamp del exchange en milisegundos
	time_t session_open;  // inicio de sesión en milisegundos
	time_t session_close; // cierre de sesión en milisegundos

} reymm_stoikov;

// *****************
// *** Functions ***
// *****************

extern void   reymm_volume(reymm_stoikov*);
extern double reymm_sigma(reymm_stoikov*);
extern double reymm_kappa(reymm_stoikov*);

extern double reymm_mid_price(reymm_stoikov*);
extern double reymm_market_spread(reymm_stoikov*);

extern double reymm_imbalance(reymm_stoikov*);
extern double reymm_adjustment(reymm_stoikov*);
extern double reymm_mp(reymm_stoikov*);

extern double reymm_time_remaining(reymm_stoikov*);
extern double reymm_optimal_spread(reymm_stoikov*);
extern double reymm_reservation_price(reymm_stoikov*);

extern void   reymm_quotes(reymm_stoikov*, double*, double*);

#if defined(__cplusplus)
}
#endif
#endif // REYMON_REYMM_REYMM_H