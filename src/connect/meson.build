# **************************
# *** Author: Reymon Dev ***
# *** Date: 1 Mai 2026   ***
# *** Update: 1 Mai 2026 ***
# *** License: MIT       ***
# **************************

subdir('binance')
subdir('kraken')

# *****************
# *** Libraries ***
# *****************

libraries_connect = [
	libreymm_utilities,
	libreymm_risk
]

# ***************
# *** Binance ***
# ***************

libreymm_connect_binance = shared_library(
	'reymm_connect_binance',

	FILES_CONNECT_BINANCE,

	link_with: libraries_connect,
	dependencies: dependencies,
	include_directories: directories,

	install: true
)

# **************
# *** Kraken ***
# **************

libreymm_connect_kraken = shared_library(
	'reymm_connect_kraken',

	FILES_CONNECT_KRAKEN,

	link_with: libraries_connect,
	dependencies: dependencies,
	include_directories: directories,

	install: true
)