project(
	'PZH Market Maker',
	'c',
	meson_version : '>=1.9.0',
	version : '0.1',
	default_options : [
		'c_std=gnu23',
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
	'-DPZH_DEMO=1',
	language: 'c'
)

else

add_project_arguments(
	'-DPZH_RELEASE=1',
	language: 'c'
)

endif

add_project_link_arguments(
	
	'-lm',
	'-lpthread',

	language: 'c'
)

# ****************
# *** Programs ***
# ****************

gdb = find_program('gdb')

# ********************
# *** Dependencies ***
# ********************

curl       = dependency('libcurl')
json       = dependency('yyjson')
crypto     = dependency('libcrypto')
openssl    = dependency('openssl')
websockets = dependency('libwebsockets')

dependencies = [
	curl,
	json,
	crypto,
	openssl,
	websockets
]

# ***************
# *** Headers ***
# ***************

directories = include_directories(
	'include'
)

# *************
# *** Files ***
# *************

files_kern = files(
	'src/kern.c'
)

# **********************
# *** Subdirectories ***
# **********************

subdir('src/utils')
subdir('src/risk')
subdir('src/exchange')

libraries = [
	libpzhmm_utils,
	libpzhmm_risk,
	libpzhmm_exchanges
]

# *******************
# *** Executables ***
# *******************

pzhmm_exe = executable(
	'pzhmm',
	
	files_kern,

	link_with: libraries,
	dependencies : dependencies,
	include_directories: directories,

	install: true
)

# *******************
# *** Run Targets ***
# *******************

run_target('run-debug',   command: [ gdb, pzhmm_exe ])
run_target('run-release', command: [ pzhmm_exe ])