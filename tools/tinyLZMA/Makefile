NATIVE_CC ?= gcc

ifeq (, $(shell which $(NATIVE_CC) 2>/dev/null))
$(error "Native GCC is missing. Please install it first. If it's path is custom, set it with export NATIVE_CC=<path to native gcc toolchain>")
endif

.PHONY: all clean

all: tinyLZMA
	@echo > /dev/null

clean:
	@rm -f LZMA

tinyLZMA: TinyLZMACompress.c tinyLZMA.c
	@$(NATIVE_CC) -o $@ TinyLZMACompress.c tinyLZMA.c