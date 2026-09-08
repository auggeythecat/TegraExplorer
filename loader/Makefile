ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/base_rules

################################################################################

LDR_LOAD_ADDR   := 0x40007000
LDR_MAGIC       := 0x54454558 # TEEX

################################################################################

TARGET    := loader
BUILDDIR  := ../build
OUTPUTDIR := ../output
BDKDIR    := bdk
BDKINC    := -I../$(BDKDIR)
DECOMPDIR := decompressor
DECOMPINC := -I../$(DECOMPDIR)

VPATH  = $(dir $(wildcard ./*))
VPATH += $(dir $(wildcard ../$(DECOMPDIR)/*))
VPATH += $(dir $(wildcard ../$(BDKDIR)/*/)) $(dir $(wildcard ../$(BDKDIR)/*/*/))

# Main and graphics.
OBJS = $(addprefix $(BUILDDIR)/$(TARGET)/, \
	start.o loader.o unlzma_tiny.o \
)

################################################################################

CUSTOMDEFINES := -DBL_MAGIC=$(LDR_MAGIC)

#TODO: Considering reinstating some of these when pointer warnings have been fixed.
WARNINGS := -Wall -Wextra -Wno-implicit-fallthrough

ARCH := -march=armv4t -mtune=arm7tdmi -mthumb-interwork
CFLAGS = $(ARCH) -O2 -g -nostdlib -ffunction-sections -fdata-sections -fomit-frame-pointer -std=gnu11 $(WARNINGS) $(CUSTOMDEFINES)
LDFLAGS = $(ARCH) -nostartfiles -lgcc -Wl,--nmagic,--gc-sections -Xlinker --defsym=LDR_LOAD_ADDR=$(LDR_LOAD_ADDR)

################################################################################

.PHONY: all clean

all: $(TARGET).bin $(TOOLSLZMA) $(TOOLSB2C)

clean:
	@rm -f payload.h
	@rm -rf $(BUILDDIR)
	@rm -rf $(OUTPUTDIR)

$(TARGET).bin: $(BUILDDIR)/$(TARGET).elf
	@mkdir -p "$(@D)"
	@$(OBJCOPY) -S -O binary $< $(OUTPUTDIR)/$(PAYLOAD_NAME).bin

$(BUILDDIR)/$(TARGET).elf: $(OBJS)
	@$(CC) $(LDFLAGS) -T link.ld $^ -o $@

$(BUILDDIR)/$(TARGET)/%.o: %.c
	@mkdir -p "$(@D)"
	@$(CC) $(CFLAGS) $(BDKINC) $(DECOMPINC) -c $< -o $@

$(BUILDDIR)/$(TARGET)/%.o: %.S
	@mkdir -p "$(@D)"
	@$(CC) $(CFLAGS) -c $< -o $@