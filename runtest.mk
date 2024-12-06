# FILE: test_runtest.mk

TESTS := $(wildcard ./bin/test_*.x)

runtests:
	for test in $(TESTS); do \
		$$test; \
	done