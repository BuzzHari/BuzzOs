[org 0x7c00]

KERNEL_OFFSET equ 0x1000    ; This is the memory offset to which we will load our kernel

    xor ax, ax
    mov ds, ax
    mov es, ax
    
    mov[BOOT_DRIVE], dl     ; BIOS sets us the boot drive in 'dl' on boot
    mov bp, 0x9000
    mov sp, bp

    mov si, MSG_REAL_MODE
    call print_string
    call print_newline

    call load_kernel ; read the kernel from disk
    call switch_to_pm; disable interrupts, load GDT, etc. Finally jumps to 'BEGIN_PM'
    jmp $ ; never executed, cause we will never return


%include "boot/print.asm"
%include "boot/print_hex.asm"
%include "boot/disk.asm"
%include "boot/gdt.asm"
%include "boot/32bit_print.asm"
%include "boot/switch_pm.asm"


[bits 16]
load_kernel:
    mov si, MSG_LOAD_KERNEL
    call print_string
    call print_newline

    mov bx, KERNEL_OFFSET   ; Set-up parameters for our disk_load routine,
    mov dh, 15              ; so that we load the first 15 sectors (excluding 
    mov dl, [BOOT_DRIVE]    ; the boot sector) from the boot disk (i.e. our
    call disk_load          ; kernel code) to address KERNEL_OFFSET.
    ret

[bits 32]
BEGIN_PM:
    mov esi, MSG_PROT_MODE
    call print_string_pm
    call KERNEL_OFFSET ; Give control to the kernel
    jmp $ ; Stay here if the kernel returns control to us (if ever)

; Global variables
BOOT_DRIVE db 0;
MSG_REAL_MODE db "Started in 16-bit Real Mode", 0
MSG_PROT_MODE db "Landed in 32-bit Protected Mode", 0
MSG_LOAD_KERNEL db "Loading kernel into memory", 0

;Bootsector padding
times 510 - ($-$$) db 0
dw 0xaa55
