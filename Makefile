include common.mk
include runtest.mk
# Root directory path
ROOT_DIR := $(realpath $(CURDIR))
export ROOT_DIR

# Create bin directory
$(shell mkdir -p bin)

SUBDIRS = src tests

.PHONY: run run_driver build clean $(SUBDIRS)

default: run_driver

build: $(SUBDIRS)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

tests: src # Tests depend on src files

run_driver: build
	.$(BIN_DIR)/test_driver.x

run_%:
	.$(BIN_DIR)/$*.x

clean:
	$(MAKE) -C src clean; 
	rm -rf ./bin

