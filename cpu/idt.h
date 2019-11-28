#ifndef IDT_H
#define IDT_H

#include "types.h"

// Kernel Code Segment selector. Offset is at 0x08.
// Look into /boot/gdt.asm for more info 
#define KERNEL_CS 0x08

// Structure of interrupt gate handler.
// Refer Intel Manual V3, page 197, figure 6.2
typedef struct {

    //Lower 16 bits of handler function address
    u16 low_offset;
    //Kernel segment selector
    u16 sel;
    //Must always be zero.
    u8 always0;

   /* First byte
    * Bit 7: "Interrupt is present"
    * Bits 6-5: Privilege level of caller (0 = kernel..3 = user)
    * Bit 4: Set to 0 for interrupt gates
    * Bit 3-0: bits 1110 = decimal 14 = "32 bit interrupt gate"
    */
    u8 flags;
    //Higer 16 bits of handler function address.
    u16 high_offset;

}__attribute__((packed)) idt_gate_t;


/*
 * A struct describing a pointer to the array of interrupt handlers.
 * Assembly instruction 'lidt' will read it
 */

typedef struct {
    u16 limit;
    u32 base; // The address of the first element in our idt_entry_t array.
}__attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256
idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

// Functions in idt.c
void set_idt_gate(int n, u32 handler);
void set_idt();

#endif


