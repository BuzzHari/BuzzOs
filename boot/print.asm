print_string:
    
    ; When a function is called, there is a good chance that, the function might
    ; change the values of several registers, as registers are scarce.
    ; To to care of this, it is better if we push all the current value of the
    ; registers onto the stack. This can be achieved by the instruction pusha.
    ; Before returning from the function, we restore the registers, using the
    ; instruction popa.
   
    ; 'pusha' and 'popa', shouldn't really be used as, it only makes sense,
    ; to store the register values which are going to be changed, instead of 
    ; pushing all the register onto the stack.
    pusha
    
    print_loop:
        mov al, [si] ; 'si' is the pointer to the string, which needs to be printed.
        cmp al, 0
        je done

        ;The part where we print
        mov ah, 0x0e
        int 0x10 ; al, already contains the char

        inc si
        jmp print_loop 


done:
    popa ; restoring the intial register contents
    ret  ; returning to the main function.


print_newline:
    
    pusha

    mov ah, 0x0e
    mov al, 0x0a ; newline char
    int 0x10
    mov al, 0x0d ; carriage return
    int 0x10

    popa
    ret
