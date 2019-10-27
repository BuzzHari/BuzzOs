#include "../drivers/screen.h"

void main(){
    clear_screen();
    for(int i = 0; i < MAX_ROWS; i++)
        kprint("...\n");
    kprint("Scroll_1\n");
    kprint("Scroll_2\n");
}
