# **************************
# *** Author: Reymon Dev ***
# *** Date: 1 Mai 2026   ***
# *** Update: 1 Mai 2026 ***
# *** License: MIT       ***
# **************************

subdir('mp')
subdir('inventory')

libreymm_risk = shared_library(
	'reymm_risk',

	FILES_RISK_MP,
	FILES_RISK_INVENTORY,

	link_with: libreymm_utilities,
	dependencies: dependencies,
	include_directories: directories,

	link_args: [ '-lm' ],

	install: true
)