print_hex:
    
    pusha

    mov cx, 0 ; Index variable

    ; Basically what we do is that, we take the data, stored in dx
    ; and convert it into ASCII.
    ; Numeric ASCII values: '0' (ASCII:0x30) to '9' (0x39), we'll add 0x30
    ; For alphabtic characters A-F: 'A' (ASCII 0x41) to 'F' (ASCII  0x46), we'll add 0x40
    ; Finally we move the ASCII byte to the correct positon on the resulting string
    ; i.e. HEX_OUT

    hex_loop:
        cmp cx, 4 ; loop 4 times
        je end 

        mov ax, dx ; using 'ax' ar our working  register
        and ax, 0x000f ; 0x1234 -> 0x0004, by masking first three to zeros.
        add al, 0x30 ; add 0x30
        cmp al, 0x39 ; if > 9, add 7 to it.
        jle step2
        add al, 7

    step2:
        ; bx <- base adress + string length - index of char
        mov bx, HEX_OUT + 5; base + length
        sub bx, cx ; index variable
        mov [bx], al ; copying the ASCII char on 'al' to the position by 'bx'
        ror dx, 4; rotate 4 bits to right, 0x1234 -> 0x4321 and so on.

        add cx, 1
        jmp hex_loop

end:
   ;print HEX_OUT and return

   mov bx, HEX_OUT
   call print_string

   popa
   ret


; Reserving memory for our new string.
HEX_OUT:
    db '0x0000', 0
