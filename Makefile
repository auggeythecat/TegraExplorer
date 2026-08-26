ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/base_rules

################################################################################

IPL_LOAD_ADDR   := 0x40008000
IPL_MAGIC       := 0x54454558 # TEEX
TEVERSION_MAJOR := 5
TEVERSION_MINOR := 0
TEVERSION_BUGFX := 0
TEVERSION       := \"$(TEVERSION_MAJOR).$(TEVERSION_MINOR).$(TEVERSION_BUGFX)\"

################################################################################

TARGET    := TegraExplorer
BUILDDIR  := build
OUTPUTDIR := output
SOURCEDIR  = source
BDKDIR    := bdk
BDKINC    := -I./$(BDKDIR)

VPATH  = $(dir ./$(SOURCEDIR)/)           $(dir $(wildcard ./$(SOURCEDIR)/*/))  $(dir $(wildcard ./$(SOURCEDIR)/*/*/))
VPATH += $(dir $(wildcard ./$(BDKDIR)/))  $(dir $(wildcard ./$(BDKDIR)/*/))     $(dir $(wildcard ./$(BDKDIR)/*/*/))

OBJS  = start exception_handlers main heap gfx menu

OBJS += hw_init di vic joycon touch se bpmp clock fuse gpio i2c pinmux pmc uart timer mc minerva sdram \
        bq24193 max7762x regulator_5v sd sdmmc fan tmp451 util irq

OBJS := $(addsuffix .o, $(OBJS))
OBJS := $(addprefix $(BUILDDIR)/$(TARGET)/, $(OBJS))

GFX_INC   := '"../$(SOURCEDIR)/gfx/gfx.h"'

################################################################################

CUSTOMDEFINES := -DIPL_LOAD_ADDR=$(IPL_LOAD_ADDR) -DTE_MAGIC=$(IPL_MAGIC)
CUSTOMDEFINES += -DTE_VER_MJ=$(TEVERSION_MAJOR) -DTE_VER_MN=$(TEVERSION_MINOR) -DTE_VER_HF=$(TEVERSION_BUGFX) -DTE_VER=$(TEVERSION)
CUSTOMDEFINES += -DGFX_INC=$(GFX_INC)

WARNINGS := -Wall -Wextra -Wno-implicit-fallthrough

ARCH   := -march=armv4t -mtune=arm7tdmi -mthumb -mthumb-interwork $(WARNINGS)
CFLAGS  = $(ARCH) -Os -g -gdwarf-4 -nostdlib -ffunction-sections -fdata-sections -fomit-frame-pointer -std=gnu11 $(CUSTOMDEFINES)
LDFLAGS = $(ARCH) -nostartfiles -lgcc -Wl,--nmagic,--gc-sections -Xlinker --defsym=IPL_LOAD_ADDR=$(IPL_LOAD_ADDR)

################################################################################

.PHONY: all clean

all: $(OUTPUTDIR)/$(TARGET).bin
	@echo "--------------------------------------"
	$(eval BIN_SIZE = $(shell wc -c < $(OUTPUTDIR)/$(TARGET).bin))
	@echo "Payload size is $(BIN_SIZE)"

	@echo "Max size is 126296 Bytes."
	@if [ ${BIN_SIZE} -gt 126296 ]; then echo -e "\e[1;33m Payload size exceeds limit!\e[0m"; fi
	@echo "--------------------------------------"

clean:
	@rm -rf $(BUILDDIR)
	@rm -rf $(OUTPUTDIR)

$(OUTPUTDIR)/$(TARGET).bin: $(BUILDDIR)/$(TARGET)/$(TARGET).elf
	@mkdir -p "$(@D)"
	$(OBJCOPY) -S -O binary $< $@

$(BUILDDIR)/$(TARGET)/$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) -T $(SOURCEDIR)/link.ld $^ -o $@

$(BUILDDIR)/$(TARGET)/%.o: %.c
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) $(BDKINC) $(PIKAINC) -c $< -o $@

$(BUILDDIR)/$(TARGET)/%.o: %.S
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -c $< -o $@