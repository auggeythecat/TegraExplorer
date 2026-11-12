ENTRY(_start)

SECTIONS {
	PROVIDE(__iplStart = IPL_LOAD_ADDR);
	. = __iplStart;
	.text : {
		*(.text._start);
		KEEP(*(._iplVersion));
		*(.text._irqSetup);
		*(.text*);
	}
	.data : {
		*(.data*);
		*(.rodata*);
	}
	. = ALIGN(0x10);
	__iplEnd = .;
	.bss : {
		__bssStart = .;
		*(COMMON)
		*(.bss*)
		__bssEnd = .;
	}
}