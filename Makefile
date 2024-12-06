include common.mk

# Root directory path
ROOT_DIR := $(realpath $(CURDIR))
export ROOT_DIR

# Create bin directory
$(shell mkdir -p $(BIN_DIR))

SUBDIRS = src

.PHONY: build clean $(SUBDIRS)

build: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done