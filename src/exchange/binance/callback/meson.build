# ********************************
# *** Author: PZH Gesellschaft ***
# *** Datum: 14 Mai 2026       ***
# *** Aktua: 14 Mai 2026       ***
# *** Lizenz: AGPL-3-or-later  ***
# ********************************

# *************
# *** Datei ***
# *************

files_exchange_binance_callback = files(
	'callback.c',
	'variables.c'
)

# ******************
# *** Bibliothek ***
# ******************

libpzhmm_exchange_binance_callback = shared_library(
	'pzhmm_exchange_binance_callback',

	files_exchange_binance_callback,

	link_with: [
		libpzhmm_risk,
		libpzhmm_utils,
		libpzhmm_exchange_binance_json,
		libpzhmm_exchange_binance_order
	],

	dependencies: dependencies,
	include_directories: directories,

	install: true
)