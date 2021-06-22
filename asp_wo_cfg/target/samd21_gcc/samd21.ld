MEMORY
{
  rom      (rx)  : ORIGIN = 0x00002000, LENGTH = 0x00040000-0x2000
  ram      (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00008000
}

PROVIDE(software_term_hook = 0);

ENTRY(_kernel__start)

SECTIONS
{
	.text :
	{
		__text = .;
		*(.rodata.vector)
		*(.text)
		*(.text.*)
	} > rom
	_etext = .;
	PROVIDE(etext = .);

	.rodata :
	{
		*(.rodata)
		*(.rodata.*)
	} > rom

	.ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) }
	__exidx_start = .;
	.ARM.exidx   : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) }
	__exidx_end = .;

	. = ALIGN(4);
	__idata_start = .;
	.data : AT (__idata_start)
	{
		__data_start = .;
		*(.data)
		*(.data.*)
	} > ram
	__idata_end = __idata_start + SIZEOF(.data);
	_edata = .;
	PROVIDE(edata = .);

	. = ALIGN(4);

	.bss :
	{
		__bss_start  = .;
		*(.bss)
		*(.bss.*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end = .;
	} > ram
	. = ALIGN(4);
	_end = .;
	PROVIDE(end = .);
}
