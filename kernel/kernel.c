#include "../drivers/screen.h"
#include "utils.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"

void main(){
    //clear_screen();
    //for(int i = 0; i < MAX_ROWS; i++)
    //    kprint("...\n");
    //kprint("Scroll_1\n");
    //kprint("Scroll_2\n");
    isr_install();
    asm volatile("sti");
    //init_timer(5);
    init_keyboard();
    /*
     *__asm__ __volatile__("int $2");
     *__asm__ __volatile__("int $3");
     *__asm__ __volatile__("int $19");
     */
}
