# Variables
SRC_DIR := .
BIN_DIR := compiled_pdfs
PANDOC := pandoc
HEADER := ./header.tex

# List all Markdown files in the source directory
MD_FILES := $(wildcard $(SRC_DIR)/*.md)

# Convert each Markdown file to a PDF in the bin directory
PDF_FILES := $(patsubst $(SRC_DIR)/%.md, $(BIN_DIR)/%.pdf, $(MD_FILES))

# Default target
all: $(PDF_FILES)

# Rule to create bin directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Rule to convert Markdown to PDF
$(BIN_DIR)/%.pdf: $(SRC_DIR)/%.md | $(BIN_DIR)
	$(PANDOC) -H $(HEADER) $< -o $@

# Clean target
clean:
	rm -rf $(BIN_DIR)

# Phony targets
.PHONY: all clean
