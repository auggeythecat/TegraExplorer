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
LOADERDIR := ./loader
LZ77DIR   := ./tools/nrv
BIN2CDIR  := ./tools/bin2c

VPATH  = $(dir ./$(SOURCEDIR)/)          $(dir $(wildcard ./$(SOURCEDIR)/*/)) $(dir $(wildcard ./$(SOURCEDIR)/*/*/))
VPATH += $(dir $(wildcard ./$(BDKDIR)/)) $(dir $(wildcard ./$(BDKDIR)/*/))    $(dir $(wildcard ./$(BDKDIR)/*/*/))

# !TODO: add objs when needed

OBJS  = start exception_handlers main heap gfx

OBJS += bpmp ccplex clock di irq timer \
		mc sdram minerva smmu \
		gpio pinmux pmc se tsec uart \
		fuse kfuse \
		sdmmc sdmmc_driver emmc sd nx_emmc_bis \
		bq24193 max17050 max7762x \
		hw_init regulator_5v joycon

OBJS += btn dirlist util ianos ini \
		i2c ramdisk sprintf

#OBJS += arrayClass arrayReferenceClass dictionaryClass \
#		else eval functionClass garbageCollector genericClass \
#		intClass model parser saveClass StringClass scriptError \
#		standardLibrary unsolvedArrayClass
#

OBJS += emummc config
#OBJS += keyfile nca emmcfile emummc hid config menu tools err gfxutils utils vector \
#		fsutils mountmanager fscopy folderReader \
#		keys
#
#OBJS += mainmenu filemenu foldermenu gptmenu explorer

OBJS += diskio ff ffunicode ffsystem elfload elfreloc_arm

OBJS := $(addsuffix .o, $(OBJS))
OBJS := $(addprefix $(BUILDDIR)/$(TARGET)/, $(OBJS))

GFX_INC   := '"../$(SOURCEDIR)/gfx/gfx.h"'
FFCFG_INC := '"../$(SOURCEDIR)/libs/fatfs/ffconf.h"'

################################################################################

CUSTOMDEFINES := -DIPL_LOAD_ADDR=$(IPL_LOAD_ADDR) -DTE_MAGIC=$(IPL_MAGIC)
CUSTOMDEFINES += -DTE_VER_MJ=$(TEVERSION_MAJOR) -DTE_VER_MN=$(TEVERSION_MINOR) -DTE_VER_HF=$(TEVERSION_BUGFX) -DTE_VER=$(TEVERSION)
# CUSTOMDEFINES += -DBDK_WATCHDOG_FIQ_ENABLE -DBDK_RESTART_BL_ON_WDT -DBDK_MALLOC_NO_DEFRAG #TODO: Look into these options from hekate and see if they should be
CUSTOMDEFINES += -DGFX_INC=$(GFX_INC) -DFFCFG_INC=$(FFCFG_INC)

#CUSTOMDEFINES += -DDEBUG

# UART Logging: Max baudrate 12.5M.
# DEBUG_UART_PORT - 0: UART_A, 1: UART_B, 2: UART_C.
#CUSTOMDEFINES += -DDEBUG_UART_BAUDRATE=115200 -DDEBUG_UART_INVERT=0 -DDEBUG_UART_PORT=1

# TODO: Check out warnings from hekate
#WARNINGS := -Wall -Wsign-compare -Wtype-limits -Wno-array-bounds -Wno-stringop-overread -Wno-stringop-overflow
#WARNINGS += -fno-delete-null-pointer-checks -fstack-usage

ARCH   := -march=armv4t -mtune=arm7tdmi -mthumb -mthumb-interwork
CFLAGS  = $(ARCH) -Os -nostdlib -ffunction-sections -fdata-sections -fomit-frame-pointer -fno-inline -std=gnu11 $(CUSTOMDEFINES)
LDFLAGS = $(ARCH) -nostartfiles -lgcc -Wl,--nmagic,--gc-sections -Xlinker --defsym=IPL_LOAD_ADDR=$(IPL_LOAD_ADDR)

################################################################################

.PHONY: all clean

all: $(OUTPUTDIR)/$(TARGET)_small.bin
	@echo "--------------------------------------"
	$(eval BIN_SIZE = $(shell wc -c < $(OUTPUTDIR)/$(TARGET).bin))
	@echo "Payload size is $(BIN_SIZE)"
	$(eval COMPR_BIN_SIZE = $(shell wc -c < $(OUTPUTDIR)/$(TARGET)_small.bin))
	@echo "Compressed Payload size is $(COMPR_BIN_SIZE)"

	@echo "Max size is 126296 Bytes."
	@if [ ${BIN_SIZE} -gt 140288 ]; then echo "\e[1;33mUncompressed Payload size exceeds limit!\e[0m"; fi
	@if [ ${COMPR_BIN_SIZE} -gt 126296 ]; then echo "\e[1;33mCompressed Payload size exceeds limit!\e[0m"; fi
	@echo "--------------------------------------"

clean:
	@rm -rf $(BUILDDIR)
	@rm -rf $(OUTPUTDIR)
	@rm -rf $(LOADERDIR)/payload.h

$(OUTPUTDIR)/$(TARGET)_small.bin: $(OUTPUTDIR)/$(TARGET).bin
	@$(MAKE) -C $(LZ77DIR)
	@$(LZ77DIR)/nrv2e $(OUTPUTDIR)/$(TARGET).bin
	@$(MAKE) -C $(BIN2CDIR)
	@$(BIN2CDIR)/bin2c $(OUTPUTDIR)/$(TARGET).bin.nrv payload > $(LOADERDIR)/payload.h
	@rm -rf $(OUTPUTDIR)/$(TARGET).bin.nrv

	$(MAKE) -C $(LOADERDIR) PAYLOAD_NAME=$(TARGET)_small

$(OUTPUTDIR)/$(TARGET).bin: $(BUILDDIR)/$(TARGET)/$(TARGET).elf
	@mkdir -p "$(@D)"
	$(OBJCOPY) -S -O binary $< $@

$(BUILDDIR)/$(TARGET)/$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) -T $(SOURCEDIR)/link.ld $^ -o $@

$(BUILDDIR)/$(TARGET)/%.o: %.c
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) $(BDKINC) -c $< -o $@

$(BUILDDIR)/$(TARGET)/%.o: %.S
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/$(TARGET)/script/builtin.o: $(BUILDDIR)/$(TARGET)/script/builtin.c
	@mkdir -p "$(@D)"
	$(CC) $(CFLAGS) $(BDKINC) -c $< -o $@
    
$(BUILDDIR)/$(TARGET)/script/builtin.c: scripts/*.te
	@mkdir -p "$(@D)"
	@mkdir -p "$(BUILDDIR)/$(TARGET)/scripts"

ifeq ($(OS),Windows_NT)
	@py ts-minifier.py --such-meme -d "$(BUILDDIR)/$(TARGET)/scripts" $(wildcard scripts/*.te)
	@py te2c.py "$(BUILDDIR)/$(TARGET)/script/builtin" "$(BUILDDIR)/$(TARGET)/scripts"
else
	@python3 ts-minifier.py --such-meme -d "$(BUILDDIR)/$(TARGET)/scripts" $(wildcard scripts/*.te)
	@python3 te2c.py "$(BUILDDIR)/$(TARGET)/script/builtin" "$(BUILDDIR)/$(TARGET)/scripts"
endif