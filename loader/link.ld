ENTRY(_start)

SECTIONS {
	PROVIDE(__iplStart = LDR_LOAD_ADDR);
	. = __iplStart;
	.text : {
		*(.text._start);
		*(.text*);
	}
	.data : {
		*(.data*);
		*(.rodata*);
		*(._payload);

		/*
		 * To mitigate bad injectors/chainloaders,
		 * miss-align binary size to account for version info.
		 * !If version text is not appended, then use ". = ALIGN(4)"!
		 */
		 dataEndUa = .;
		 . = ((dataEndUa + 0x6 + 4 - 1) & ~(4 - 1)) - 6;
	}
	__ldrEnd = .;
	. = ALIGN(0x10);
	__iplEnd = .;
}