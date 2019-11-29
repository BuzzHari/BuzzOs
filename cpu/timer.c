#include "timer.h"
#include "../drivers/screen.h"
#include "../kernel/utils.h"
#include "../drivers/ports.h"
#include "isr.h"

u32 tick = 0;

/*
 * A Little Theroy on PIT:
 * Programmable interval timer is a chip connected IRQ0. It can interrupt the CPU at a
 * user-defined rate, between 18.2 Hz and 1.1931 Mhz. Hz yields the approximate number of
 * timer interrupts per second.
 * Primary Method used for implementing a system clock.
 * Usually implemented by an 8254 CMOS chip using the 0x40-0x43 I/O ports.
 * 0x40 : Channel 0 data port (read/write).
 * 0x41 : Channel 1 data port (read/write).
 * 0x42 : Channel 2 data port (read/write).
 * 0x43 : Mode/Command register (write only, read is ignored).
 *
 * Channel 0 is tied to IRQ0, to interrupt the CPU at predictable and regular times.
 * Channel 1 is system specifc. Pretty much useless, was used to refresh DRAM.
 *      Now DRAM refresh is done with dedicated hardware.
 * Channel 2 is connected to the PC speakers, so the frequency of the output determins the
 *      frequency of the sound produced by the speaker.
 */

static void timer_callback(registers_t regs) {
    tick++;
    kprint("Tick: ");

    char tick_ascii[256];
    int_to_ascii(tick, tick_ascii);
    kprint(tick_ascii);
    kprint("\n");
}

void init_timer(u32 freq) {

    // Install the fucntion at IRQ0.
    // Basically tells our interrupt mechanism that handle IRQ0 with
    // this function.
    register_interrupt_handler(IRQ0, timer_callback);
    
    // The value we send to the PIT is the value to divide it's input clock, by
    // to get the desired freq.
    u32 divisor = 1193180 / freq;
    u8 low = (u8)(divisor & 0xFF);
    u8 high = (u8)((divisor >> 8) & 0xFF);

    // Send the command to the PIT's port.
    // 0x36 sets the PIT to repeating mode, so that when the divisor counter reached zero
    // it's automatically referesed. And it tells it we want to set the divisor value.
    port_byte_out(0x43, 0x36);
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}

