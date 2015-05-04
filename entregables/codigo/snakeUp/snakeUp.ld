ENTRY(main);
SECTIONS
{
	. = 0x7C00;
	.text : AT(0x7C00)
	{
		*(.text);
	}
	.sig : AT(0x7DFE)
	{
		SHORT(0xaa55);
	}
	.eh_frame : AT(0x7E00) /*0x7E00 = 0x7C00 + 0x200*/
	{
	}
}
