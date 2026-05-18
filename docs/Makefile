# ********************************
# *** Author: PZH Gesellschaft ***
# *** Date: 13 Mai 2026        ***
# *** Update: 13 Mai 2026      ***
# *** License: AGPL-3-or-later ***
# ********************************

# ***************
# *** Folders ***
# ***************

FOLDER_TS := ts
FOLDER_JS := js

FOLDER_PDF   := pdf
FOLDER_LATEX := latex

# *************
# *** Files ***
# *************

FILES_TS := $(shell find $(FOLDER_TS) -type f -iname '*.ts')
FILES_JS := $(patsubst $(FOLDER_TS)/%.ts, $(FOLDER_JS)/%.js, $(FILES_TS))

FILES_LATEX := $(shell find $(FOLDER_LATEX) -type f -iname '*.tex')
FILES_PDF   := $(patsubst $(FOLDER_LATEX)/%.tex, $(FOLDER_PDF)/%.pdf, $(FILES_LATEX))

# *************
# *** Tools ***
# *************

TEX := pdflatex
TSC := npx tsc

# *******************
# *** Tools Flags ***
# *******************

TEXFLAGS := -output-format=pdf
TSCFLAGS := --strict --target es2020

# *************
# *** PHONY ***
# *************

.PHONY: all clean distclean

# *************
# *** Rules ***
# *************

all: $(FILES_JS) $(FILES_PDF)

clean:
	rm -f $(FILES_JS) $(FILES_PDF)

distclean:
	rm -rf js pdf

# ***************
# *** Pattern ***
# ***************

$(FOLDER_JS)/%.js: $(FOLDER_TS)/%.ts
	@mkdir -p $(dir $@)
	$(TSC) $(TSCFLAGS) $< --OutDir $(FOLDER_JS)

$(FOLDER_PDF)/%.pdf: $(FOLDER_LATEX)/%.tex
	@mkdir -p $(dir $@)
	$(TEX) $(TEXFLAGS) -output-directory=$(dir $@)  $<