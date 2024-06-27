ENTRY(_kstart)
SECTIONS {
    . = 0x20000; /* Set the text section to start at address 0x20000 */

    .text : {
        *(.text)        /* .text sections (code) */
    }

    .rodata : {
        *(.rodata)      /* .rodata section (read-only data) */
    }

    .data : {
        *(.data)        /* .data section (initialized data) */
    }

    .bss : {
        *(.bss)         /* .bss section (zero-initialized data) */
    }
}
