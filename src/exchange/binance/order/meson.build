# ********************************
# *** Author: PZH Gesellschaft ***
# *** Datum: 14 Mai 2026       ***
# *** Aktua: 14 Mai 2026       ***
# *** Lizenz: AGPL-3-or-later  ***
# ********************************

# *************
# *** Datei ***
# *************

files_exchange_binance_order = files(
	'create_order.c'
)

# ******************
# *** Bibliothek ***
# ******************

libpzhmm_exchange_binance_order = static_library(
	'pzhmm_exchange_binance_order',

	files_exchange_binance_order,

	link_with: [
		libpzhmm_risk,
		libpzhmm_utils
	],

	dependencies: dependencies,
	include_directories: directories,

	install: true
)