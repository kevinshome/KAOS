/*
*  link.ld
*/
ENTRY(_start)
SECTIONS
 {
   . = 0x100000;
   .text : { *(.multiboot) *(.text) }
   .data : { *(.data) }
   .bss  : { *(.bss)  }
 }
