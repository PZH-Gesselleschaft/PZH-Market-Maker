# ********************************
# *** Author: PZH Gesellschaft ***
# *** Datum: 14 Mai 2026       ***
# *** Aktua: 14 Mai 2026       ***
# *** Lizenz: AGPL-3-or-later  ***
# ********************************

# *************
# *** Datei ***
# *************

file_exchange_kraken_order = files(
	'create_order.c'
)

# ******************
# *** Bibliothek ***
# ******************

libpzhmm_exchange_kraken_order = static_library(
	'pzhmm_exchange_kraken_order',

	file_exchange_kraken_order,

	link_with: [
		libpzhmm_risk,
		libpzhmm_utils
	],

	dependencies: dependencies,
	include_directories: directories,

	install: true
)