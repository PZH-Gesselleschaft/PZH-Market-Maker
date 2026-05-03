// **************************
// *** Author: Reymon Dev ***
// *** Date: 1 Mai 2026   ***
// *** Update: 1 Mai 2026 ***
// *** License: MIT       ***
// **************************

#include <cstdio>
#include <cstring>
#include <cctype>

#include <reymon/reymm/reymm.h>

#include "kern.hpp"

void reymm_initialiseren() {

	reymm_io(REYMM_TYPE_IO_OPEN, "setting.json", "rb", setting.file);

	setting.content = reymm_content_file(setting.file);

	setting.exchange = reymm_get_json(setting.content, "exchange");

	for (size_t i = 0; i < strlen(setting.exchange); i++) {
		char c = setting.exchange[i];
		setting.exchange[i] = toupper(c);
	}

	reymm_io(REYMM_TYPE_IO_CLOSE, "NOTHING", "NOTHING", setting.file);

}