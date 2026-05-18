# *********************************************
# *** Author: PZH Gesellschaft, SAS de CV   ***
# *** Date: 8 Mai 2026                      ***
# *** Update: 8 Mai 2026                    ***
# *** License: AGPL-3-or-later              ***
# *********************************************

subdir('inventory')
subdir('market')
subdir('microprice')

libpzhmm_risk = static_library(
	'pzhmm_risk',

	files_risk_inventory,
	files_risk_market,
	files_risk_microprice,

	dependencies: dependencies,
	include_directories: directories,

	install: false
)