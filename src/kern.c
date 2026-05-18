// *********************************
// *** Author: PZH Geselleschaft ***
// *** Date: 9 Mai 2026          ***
// *** Update 9 Mai 2026         ***
// *** License: AGPL-3-or-later  ***
// *********************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <yyjson.h>
#include <pzh/pzhmm/pzhmm.h>

static char       exchange[10][128];
static size_t     threads;
static pthread_t* p_threads;

int main(void) {

	// ********************************
	// *** Konfiguration von yyjson ***
	// ********************************

	yyjson_read_flag flg = YYJSON_READ_ALLOW_COMMENTS | YYJSON_READ_ALLOW_TRAILING_COMMAS;
	yyjson_read_err  err;

	yyjson_doc* doc  = yyjson_read_file("config/setting.json", flg, NULL, &err);

	if(!doc) return EXIT_FAILURE;

	yyjson_val* root = yyjson_doc_get_root(doc);

	if (!root) {
		yyjson_doc_free(doc);
		return EXIT_FAILURE;
	}

	// ****************************
	// *** Einstellugen abrufen ***
	// ****************************

	yyjson_val* p_exchange = yyjson_obj_get(root, "exchange");

	size_t array_size = yyjson_arr_size(p_exchange);

	if (!p_exchange) {

		yyjson_doc_free(doc);
		return EXIT_FAILURE;
	}

	for (size_t i = 0; i < array_size; i++) {

		yyjson_val* lp_exchange = yyjson_arr_get(p_exchange, i);

		snprintf(exchange[i], 128, "%s", yyjson_get_str(lp_exchange));
	}

	threads = yyjson_get_uint(yyjson_obj_get(root, "threads"));
	
	if (threads == 0 || threads < array_size) {

		yyjson_doc_free(doc);
		return EXIT_FAILURE;
	}

	yyjson_doc_free(doc);

	// **********************************
	// ***  Wählen sie eine Börse aus ***
	// **********************************

	for (size_t i = 0; i < threads; i++)
		pzh_upper(exchange[i]); // Format normalisieren

	// *********************************************
	// *** Speicherplatz für threads reservieren ***
	// *********************************************

	p_threads = malloc(sizeof(pthread_t) * threads);
	if (!p_threads) return EXIT_FAILURE;

	// *****************************
	// *** Erstellen von Threads ***
	// *****************************

	for (size_t i = 0; i < threads; i++) {

		int rc = 0;

		if (strcmp(exchange[i], "BINANCE") == 0)
			rc = pthread_create(&p_threads[i], NULL, binance_connect, NULL);
		
		else if (strcmp(exchange[i], "KRAKEN") == 0)
			rc = pthread_create(&p_threads[i], NULL, kraken_connect, NULL);

		if (rc != 0) {
			fprintf(stderr, "pthread_create falhou: %d\n", rc);
			free(p_threads);
			return EXIT_FAILURE;
		}
	}


	for (size_t i = 0; i < threads; i++) {
    	pthread_join(p_threads[i], NULL);
	}

	free(p_threads);

	return EXIT_SUCCESS;
}