# ********************************
# *** Author: PZH Gesellschaft ***
# *** Datum: 8 Mai 2026        ***
# *** Aktua: 8 Mai 2026        ***
# *** Lizenz: AGPL-3-or-later  ***
# ********************************

subdir('binance')
subdir('kraken')

# ***************
# *** Binance ***
# ***************

libpzhmm_exchange_binance = shared_library(
	'pzhmm_exchange_binance',

	files_exchange_binance,

	link_with: [
		libpzhmm_risk,
		libpzhmm_utils,
		libpzhmm_exchange_binance_callback
	],

	dependencies: dependencies,
	include_directories: directories,

	install: true
)

# **************
# *** Kraken ***
# **************

libpzhmm_exchange_kraken = shared_library(
	'pzhmm_exchange_kraken',

	files_exchange_kraken,

	link_with: [
		libpzhmm_risk,
		libpzhmm_utils,
		libpzhmm_exchange_kraken_callback
	],

	dependencies: dependencies,
	include_directories: directories,

	install: true
)

# ********************
# *** Bibliotheken ***
# ********************

libpzhmm_exchanges = [
	libpzhmm_exchange_binance,
	libpzhmm_exchange_kraken
]