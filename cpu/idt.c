#include "idt.h"
#include "../kernel/utils.h"

/*
 * n: Interrupt vector.
 * handler: address to the handler function.
 */
void set_idt_gate(int n, u32 handler) {
    idt[n].low_offset = low_16(handler);
    idt[n].sel = KERNEL_CS;
    idt[n].always0 = 0;
    idt[n].flags = 0x8E;
    idt[n].high_offset = high_16(handler);
}

void set_idt() {
    //Pointer to IDT array.
    idt_reg.base = (u32) &idt;
    // Size of IDT array - 1
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;

    //Calling the lidt after filling up the struct.
    //'r' constraint: A register operand is allowed provied that it is in a general resgister.
    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}
