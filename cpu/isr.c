#include "isr.h"
#include "idt.h"
#include "../drivers/screen.h"
#include "../kernel/utils.h"
#include "../drivers/ports.h"

isr_t interrupt_handlers[256];

void isr_install() {
    set_idt_gate(0, (u32)isr0);
    set_idt_gate(1, (u32)isr1);
    set_idt_gate(2, (u32)isr2);
    set_idt_gate(3, (u32)isr3);
    set_idt_gate(4, (u32)isr4);
    set_idt_gate(5, (u32)isr5);
    set_idt_gate(6, (u32)isr6);
    set_idt_gate(7, (u32)isr7);
    set_idt_gate(8, (u32)isr8);
    set_idt_gate(9, (u32)isr9);
    set_idt_gate(10, (u32)isr10);
    set_idt_gate(11, (u32)isr11);
    set_idt_gate(12, (u32)isr12);
    set_idt_gate(13, (u32)isr13);
    set_idt_gate(14, (u32)isr14);
    set_idt_gate(15, (u32)isr15);
    set_idt_gate(16, (u32)isr16);
    set_idt_gate(17, (u32)isr17);
    set_idt_gate(18, (u32)isr18);
    set_idt_gate(19, (u32)isr19);
    set_idt_gate(20, (u32)isr20);
    set_idt_gate(21, (u32)isr21);
    set_idt_gate(22, (u32)isr22);
    set_idt_gate(23, (u32)isr23);
    set_idt_gate(24, (u32)isr24);
    set_idt_gate(25, (u32)isr25);
    set_idt_gate(26, (u32)isr26);
    set_idt_gate(27, (u32)isr27);
    set_idt_gate(28, (u32)isr28);
    set_idt_gate(29, (u32)isr29);
    set_idt_gate(30, (u32)isr30);
    set_idt_gate(31, (u32)isr31);
    
    
    /*
    * Remap the PIC.
    * I/O port 0x20 : Master PIC-Command.
    * I/O port 0x21 : Master PIC-Data.
    * I/O port 0xA0 : Slave PIC - Command.
    * I/O port 0xA1 : Slave PIC - Data.
    */

    /* 
     * When we enter protected mode, the first command to the two PICs is
     * the intialize command (code 0x11). This command makes the PIC wait
     * for 3 extra "initalisation words" on the data port.
     */
    port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);
    
    /*
     *  These bytes give the PIC:
     *  -> Its vector offset. 
     *      -> Master PIC's offset : 0x20.
     *      -> Slave PIC's offset : 0x28.
     */
    port_byte_out(0x21, 0x20);
    port_byte_out(0xA1, 0x28);
    /*
     *  -> Tell it how it is wired to master/slaves.
     *      -> (Ox21 -> 0x04) Tell Master PIC that there is a slave PIC(0x04) at IRQ2 (0000 0100).
     *      -> (0xA1 -> 0x02) Tell Slave PIC it's cascade at IRQ2 of Master.
     */
    port_byte_out(0x21, 0x04);
    port_byte_out(0xA1, 0x02);
    /*
     * -> Gives additional information about the environment.
     *      ->0x01 : 8086/88 mode.
     *      ->0x0  : Buffered mode.
     */
    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);
    port_byte_out(0x21, 0x0);
    port_byte_out(0xA1, 0x0);

     // Install the IRQs
    set_idt_gate(32, (u32)irq0);
    set_idt_gate(33, (u32)irq1);
    set_idt_gate(34, (u32)irq2);
    set_idt_gate(35, (u32)irq3);
    set_idt_gate(36, (u32)irq4);
    set_idt_gate(37, (u32)irq5);
    set_idt_gate(38, (u32)irq6);
    set_idt_gate(39, (u32)irq7);
    set_idt_gate(40, (u32)irq8);
    set_idt_gate(41, (u32)irq9);
    set_idt_gate(42, (u32)irq10);
    set_idt_gate(43, (u32)irq11);
    set_idt_gate(44, (u32)irq12);
    set_idt_gate(45, (u32)irq13);
    set_idt_gate(46, (u32)irq14);
    set_idt_gate(47, (u32)irq15);


    set_idt(); 
}

// Array of exception messages.
// Refer Understanding the Linux Kernel, pg 138 for more info.
// or the Intel manual v3 page 208.

char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "Device not avialable",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved by Intel",

    "Floating-point erro (Fault)",
    "Alignment Check",
    "Machine Check",
    "SIMD floating point exception(Fault)",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_handler(registers_t r) {
    kprint("Recevied interrupt: ");
    char s[3];
    int_to_ascii(r.int_no, s);
    kprint(s);
    kprint("\n");
    kprint(exception_messages[r.int_no]);
    kprint("\n");
}

void register_interrupt_handler(u8 n, isr_t handler) {
    /*
     *char s[3];
     *int_to_ascii(n,s);
     *kprint(s);
     */
    interrupt_handlers[n] = handler;
}

void irq_handler(registers_t r) {
    /*
     * After every iterrupt we need to send an End of Interrup (EOI 0x20) to the PICs
     * or they will not send another interrupt again.
     */
    
    if(r.int_no >= 40)
        port_byte_out(0xA0, 0x20); // To Slave.
    port_byte_out(0x20, 0x20); // To master.

    if(interrupt_handlers[r.int_no] != 0) {
        isr_t handler = interrupt_handlers[r.int_no];
        handler(r);
    }
}
