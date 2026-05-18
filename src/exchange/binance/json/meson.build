# ********************************
# *** Author: PZH Gesellschaft ***
# *** Datum: 14 Mai 2026       ***
# *** Aktua: 14 Mai 2026       ***
# *** Lizenz: AGPL-3-or-later  ***
# ********************************

# *************
# *** Datei ***
# *************

files_exchange_binance_json = files(
	'binance_subscribe.c'
)

# ******************
# *** Bibliothek ***
# ******************

libpzhmm_exchange_binance_json = static_library(
	'pzhmm_exchange_binance_json',

	files_exchange_binance_json,

	link_with: [
		libpzhmm_risk,
		libpzhmm_utils
	],

	dependencies: dependencies,
	include_directories: directories,

	install: true
)