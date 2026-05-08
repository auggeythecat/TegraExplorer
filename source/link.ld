ENTRY(_start)

SECTIONS {
	PROVIDE(__ipl_start = IPL_LOAD_ADDR);
	. = __ipl_start;
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
	__ipl_end = .;
	.bss : {
		__bss_start = .;
		*(COMMON)
		*(.bss*)
		__bss_end = .;
	}
	__end__ = .;
}

/*ASSERT(!DEFINED(_vfiprintf_r), "newlib is attempting to sneak back in, _vfiprintf_r addition")*/
/*ASSERT(!DEFINED(_malloc_r), "newlib is attempting to sneak back in, _malloc_r addition")*/
/*ASSERT(!DEFINED(_strtod_l), "newlib is attempting to sneak back in, _strtod_l addition")*/
/*ASSERT(!DEFINED(__gethex), "newlib is attempting to sneak back in, __gethex addition")*/
