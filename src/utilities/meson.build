# **************************
# *** Author: Reymon Dev ***
# *** Date: 1 Mai 2026   ***
# *** Update: 1 Mai 2026 ***
# *** License: MIT       ***
# **************************

subdir('curl')
subdir('io')
subdir('json')
subdir('openssl')

libreymm_utilities = shared_library(
	'reymm_utilities',

	FILES_UTILITIES_CURL,
	FILES_UTILITIES_IO,
	FILES_UTILITIES_JSON,
	FILES_UTILITIES_OPENSSL,

	dependencies: dependencies,
	include_directories: directories,

	install: true
)