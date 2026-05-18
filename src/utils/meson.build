# ********************************
# *** Author: PZH Gesellschaft ***
# *** Datum: 8 Mai 2026        ***
# *** Aktua: 8 Mai 2026        ***
# *** Lizenz: AGPL-3-or-later  ***
# ********************************

files_utils = files(
	'upper.c',
	'lower.c',
	'signatur.c',
	'init_json.c',
	'read_json.c',
	'free_json.c',
)

libpzhmm_utils = static_library(
	'pzhmm_utils',

	files_utils,

	dependencies: dependencies,
	include_directories: directories,

	install: true
)