; load 'dh' sectors from drive 'dl' into ES:BX

disk_load:
    ; reading from disk requires setting specific values in all registers
    ; so we will overwrite our input parameters from 'dx'.
    pusha
    push dx
    

    mov ah, 0x02 ; 0x02 = read sectors mov al, dh mov cl, 0x02 ; cl <- sector (0x01...0x11) ; 0x01 is our boot sector, 0x02 is the first 'available' sector
    mov al, dh
    mov cl, 0x02
    mov ch, 0x00 ; ch <- cylinder (0x0 .. 0x3FF, upper 2 bits in 'cl')
    
    ; dl <- drive number. Our caller sets it as parameter and gets it from BIOS
    ;(0 = floppy, 1 = floppy2, 0x80 = hdd, 0x81 = hdd2)
    mov dh, 0x00 ; head = 0
    
    int 0x13 ; BIOS interrupt, triggers read, as ah = 0x02
    jc disk_error
    pop dx
    cmp al, dh ; BIOS sets al to the number of sectors read
    jne sectors_error 
    popa
    ret

disk_error:
    mov si, DISK_ERROR
    call print_string
    call print_newline
    mov dh, ah ; ah = error code, dl = disk dirve that dropped the error.
    call print_hex ;
    jmp disk_loop

sectors_error:
    mov si, SECTORS_ERROR
    call print_string

disk_loop:
    jmp $

; Global variables
SECTORS db 0
DISK_ERROR: db "Disk read error", 0
SECTORS_ERROR: db "Incorrect number of sectors read", 0
