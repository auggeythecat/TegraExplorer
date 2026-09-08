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
DECOMPDIR := decompressor
DECOMPINC := -I./$(DECOMPDIR)

LDRDIR    := $(wildcard loader)
TOOLSLZMA := $(wildcard tools/tinyLZMA)
TOOLSB2C  := $(wildcard tools/bin2c)
TOOLS     := $(TOOLSLZMA) $(TOOLSB2C)

VPATH  = $(dir ./$(SOURCEDIR)/)           $(dir $(wildcard ./$(SOURCEDIR)/*/))  $(dir $(wildcard ./$(SOURCEDIR)/*/*/))
VPATH += $(dir $(wildcard ./$(BDKDIR)/))  $(dir $(wildcard ./$(BDKDIR)/*/))     $(dir $(wildcard ./$(BDKDIR)/*/*/))
VPATH += $(dir $(wildcard ./$(DECOMPDIR)/))

OBJS  = start exception_handlers main heap gfx menu testMenu sdMenu

OBJS += hw_init di vic joycon touch se bpmp clock fuse gpio i2c pinmux pmc uart timer mc minerva sdram \
        bq24193 max7762x regulator_5v fan tmp451 util irq hid btn utils sprintf math vector unlzma_tiny

OBJS += ff ffsystem ffunicode diskio nx_emmc_bis ramdisk emmc sdmmc sdmmc_driver sd

OBJS := $(addsuffix .o, $(OBJS))
OBJS := $(addprefix $(BUILDDIR)/$(TARGET)/, $(OBJS))

GFX_INC   := '"../$(SOURCEDIR)/gfx/gfx.h"'
FFCFG_INC := '"../$(SOURCEDIR)/libs/fatfs/ffconf.h"'

################################################################################

CUSTOMDEFINES := -DIPL_LOAD_ADDR=$(IPL_LOAD_ADDR) -DTE_MAGIC=$(IPL_MAGIC)
CUSTOMDEFINES += -DTE_VER_MJ=$(TEVERSION_MAJOR) -DTE_VER_MN=$(TEVERSION_MINOR) -DTE_VER_HF=$(TEVERSION_BUGFX) -DTE_VER=$(TEVERSION)
CUSTOMDEFINES += -DGFX_INC=$(GFX_INC) -DFFCFG_INC=$(FFCFG_INC)

WARNINGS := -Wall -Wextra -Wno-implicit-fallthrough

ARCH   := -march=armv4t -mtune=arm7tdmi -mthumb -mthumb-interwork
CFLAGS  = $(ARCH) -Os -g -gdwarf-4 -nostdlib -ffunction-sections -fdata-sections -fomit-frame-pointer -std=gnu11 $(CUSTOMDEFINES) $(WARNINGS)
LDFLAGS = $(ARCH) -nostartfiles -lgcc -Wl,--nmagic,--gc-sections -Xlinker --defsym=IPL_LOAD_ADDR=$(IPL_LOAD_ADDR)

################################################################################

.PHONY: all clean $(LDRDIR) $(TOOLS)

all: $(OUTPUTDIR)/$(TARGET)Big.bin $(LDRDIR)
	@echo "--------------------------------------"
	$(eval BIN_SIZE = $(shell wc -c < $(OUTPUTDIR)/$(TARGET)Big.bin))
	@if [ ${BIN_SIZE} -gt 140288 ]; then echo "\e[1;33mUncompr size exceeds limit!\e[0m"; fi
	@echo -n "Uncompressed size: "
	@echo $(BIN_SIZE)" Bytes"
	$(eval BIN_SIZE = $(shell wc -c < $(OUTPUTDIR)/$(TARGET).bin))
	@echo -n "Compressed size:   "
	@echo $(BIN_SIZE)" Bytes"
	@if [ ${BIN_SIZE} -gt 126296 ]; then echo -e "\e[1;33m Payload size exceeds limit!\e[0m"; fi
	@echo "--------------------------------------"

clean:
	@rm -rf $(BUILDDIR)
	@rm -rf $(OUTPUTDIR)
	@$(MAKE) --no-print-directory -C $(LDRDIR) $(MAKECMDGOALS) -$(MAKEFLAGS)

$(LDRDIR): $(OUTPUTDIR)/$(TARGET)Big.bin $(TOOLS)
	@$(TOOLSLZMA)/tinyLZMA $(OUTPUTDIR)/$(TARGET)Big.bin
	@mv $(OUTPUTDIR)/$(TARGET)Big.bin.lzma payload
	@$(TOOLSB2C)/bin2c payload > $(LDRDIR)/payload.h
	@rm payload
	@$(MAKE) --no-print-directory -C $@ $(MAKECMDGOALS) -$(MAKEFLAGS) PAYLOAD_NAME=$(TARGET)

$(TOOLS):
	@$(MAKE) --no-print-directory -C $@ $(MAKECMDGOALS) -$(MAKEFLAGS)

$(OUTPUTDIR)/$(TARGET)Big.bin: $(BUILDDIR)/$(TARGET)/$(TARGET).elf
	@mkdir -p "$(@D)"
	$(OBJCOPY) -S -O binary $< $@

$(BUILDDIR)/$(TARGET)/$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) -T $(SOURCEDIR)/link.ld $^ -o $@

$(BUILDDIR)/$(TARGET)/%.o: %.c
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) $(BDKINC) $(DECOMPINC) -c $< -o $@

$(BUILDDIR)/$(TARGET)/%.o: %.S
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -c $< -o $@