ENTRY(_start)

SECTIONS {
	PROVIDE(__iplStart = IPL_LOAD_ADDR);
	. = __iplStart;
	.text : {
		*(.text._start);
		KEEP(*(._ipl_version));
		*(.text._irq_setup);
		*(.text*);
	}
	.data : {
		*(.data*);
		*(.rodata*);
	}
	. = ALIGN(0x10);
	__iplEnd = .;
	.bss : {
		__bss_start = .;
		*(COMMON)
		*(.bss*)
		__bss_end = .;
	}
	__end__ = .;
}

ASSERT(!DEFINED(vfiprintf), "newlib is attempting to sneak back in, vfiprintf addition")
ASSERT(!DEFINED(strtod), "newlib is attempting to sneak back in, strtod addition")
