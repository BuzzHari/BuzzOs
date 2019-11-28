gdt_start: ;labels are needed to compute sizes and jumps
   ; the GDT starts with a null 8-byte
    dd 0x0  
    dd 0x0 

;8 byte segment descriptors

; GDT for code segment. base = 0x00000000, lenght = 0xfffff
; base = 0x0, limit = 0xfffff
; 1st flags: (present)1 (privilege)00 (descriptor type)1 -> 1001b
; type flags: (code)1 (conforming)0 (readable)1 (accessed)0 -> 1010b
; 2nd flags: (granularity)1 (32-bit default)1 (64-bit seg)0 (AVL)0 -> 1100b
gdt_code:
    dw 0xffff   ; segment length, bits 0-15
    dw 0x0      ; segment base, bits 0-15
    db 0x0      ; segment base, bits 16-23
    db 10011010b; flags (8 bits)
    db 11001111b; flags (8 bits)
    db 0x0      ; segment base, bits 24-31


; GDT for data segment. base and length identical to code segment
; Same as code segment except for the type flags:
; type flags: (code)0 (expand down)0 (writable)1 (accessed)0 -> 0010b
gdt_data:
    dw 0xffff   ; Limit (bits 0-15)
    dw 0x0      ; Base (bits 0-15)
    db 0x0      ; Base (bits 16-23)
    db 10010010b; 1st flags, type flags
    db 11001111b; 2nd flags, Limit (bits 16-19)
    db 0x0      ; Base (bits 24-31)

gdt_end:    ; The reason for putting a label at the end of the 
            ; GDT is so we can have the assembler calculate
            ; the size of the GDT for the GDT descriptor 

;GDT descriptor
gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; size (16 bit), always one less of its true size
    dd gdt_start ; address (32 bit)

;Address of the code segment and data segment.
;CODE_SEG: offset at 8-byte, ie 0x08
;DATA_SEG: offset at 16-byte, ie 0x10
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
