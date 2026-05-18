# ********************************
# *** Author: PZH Gesellschaft ***
# *** Datum: 14 Mai 2026       ***
# *** Aktua: 14 Mai 2026       ***
# *** Lizenz: AGPL-3-or-later  ***
# ********************************

# *************
# *** Datei ***
# *************

files_exchange_kraken_callback = files(
	'callback.c'
)

# ******************
# *** Bibliothek ***
# ******************

libpzhmm_exchange_kraken_callback = shared_library(
	'pzhmm_exchange_kraken_callback',

	files_exchange_kraken_callback,

	link_with: [
		libpzhmm_risk,
		libpzhmm_utils,
		libpzhmm_exchange_kraken_json,
		libpzhmm_exchange_kraken_order
	],

	dependencies: dependencies,
	include_directories: directories,

	install: true
)