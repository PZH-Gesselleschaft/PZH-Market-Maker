project(
	'reymm',
	'c', 'cpp',
	meson_version : '>=1.9.0',
	version : '0.1',
	default_options : [
		'c_std=gnu23',
		'cpp_std=gnu++23',
		'warning_level=3',
		'buildtype=debug',
		'strip=true'
  	]
)

# ************************
# *** Global Arguments ***
# ************************

if get_option('buildtype') == 'debug'

add_project_arguments(
	'-DREYMM_DEMO=1',
	language: [ 'c', 'cpp' ]
)

else

add_project_arguments(
	'-DREYMM_RELEASE=1',
	language: [ 'c', 'cpp' ]
)

endif

# ****************
# *** Programs ***
# ****************

gdb = find_program('gdb')

# ********************
# *** Dependencies ***
# ********************

curl    = dependency('libcurl')
openssl = dependency('openssl')

dependencies = [
	curl,
	openssl
]

# ***************
# *** Headers ***
# ***************

directories = include_directories(
	'include',
	'include/reymon',
	'include/reymon/reymm'
)

# **********************
# *** Subdirectories ***
# **********************

subdir('src/utilities')

subdir('src/kern')
subdir('src/risk')
subdir('src/connect')

libraries = [
	libreymm_utilities,
	libreymm_risk,
	libreymm_connect_binance,
	libreymm_connect_kraken
]

# *******************
# *** Executables ***
# *******************

reymm_exe = executable(
	'reymm',
	
	FILES_KERN,

	link_with: libraries,
	dependencies : dependencies,
	include_directories: directories,

	install: true
)

# *******************
# *** Run Targets ***
# *******************

run_target('run-debug',   command: [ gdb, reymm_exe ])
run_target('run-release', command: [ reymm_exe ])