#include "screen.h"
#include "ports.h"
#include "../kernel/utils.h"

int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col, int row, char attr);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);
int scroll(int curr_offset);

void kprint_at(char *message, int col, int row) {
    /* Set cursor if col/row are negative */
    int offset;
    if (col >= 0 && row >= 0)
        offset = get_offset(col, row);
    else {
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    /* Loop through message and print it */
    int i = 0;
    while (message[i] != 0) {
        offset = print_char(message[i++], col, row, WHITE_ON_BLACK);
        /* Compute row/col for next iteration */
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

void kprint(char *message) {
    kprint_at(message, -1, -1);
}


int print_char(char c, int col, int row, char attr) {
    unsigned char *vidmem = (unsigned char*) VIDEO_ADDRESS;
    if (!attr) attr = WHITE_ON_BLACK;

    /* Error control: print a red 'E' if the coords aren't right */
    if (col >= MAX_COLS || row >= MAX_ROWS) {
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-2] = 'E';
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-1] = RED_ON_WHITE;
        return get_offset(col, row);
    }

    int offset;
    if (col >= 0 && row >= 0) 
        offset = get_offset(col, row);
    else 
        offset = get_cursor_offset();

    if (c == '\n') {
        row = get_offset_row(offset);
        offset = get_offset(0, row+1);
    } else {
        vidmem[offset] = c;
        vidmem[offset+1] = attr;
        offset += 2;
    }
    
    //Scroll if needed.
    //offset = scroll(offset);
    
    //sets the cursor offset to the screen port.
    set_cursor_offset(offset);
    return offset;
}

int scroll(int curr_offset){
    
    if(curr_offset < (MAX_COLS*MAX_ROWS*2))
        return curr_offset;
    
    for(int i = 1; i < MAX_ROWS; i++){
        //TODO: Implement this better.
        mem_copy((unsigned char*)(get_offset(0, i)+ (unsigned char*)VIDEO_ADDRESS),
                 (unsigned char*)(get_offset(0, i-1)+(unsigned char*)VIDEO_ADDRESS),
                 MAX_COLS * 2);
    }
    unsigned char *last_line = get_offset(0, MAX_ROWS-1) + (unsigned char*)VIDEO_ADDRESS;
    for(int i = 0; i < MAX_COLS; i++)
        *(last_line+i) = 0;

    //Move the currsor offset to last line.
    curr_offset -= 2*MAX_COLS;

    return curr_offset;

}

int get_cursor_offset() {
    //We are telling the command port for the HIGH byte
    port_byte_out(REG_SCREEN_CTRL, 14);
    //getting the HIGH byte from the data port, and shifiting it 8 bits and storing it.
    int offset = port_byte_in(REG_SCREEN_DATA) << 8; /* High byte: << 8 */
    //now asking for the LOW byte
    port_byte_out(REG_SCREEN_CTRL, 15);
    //adding it to the offset.
    offset += port_byte_in(REG_SCREEN_DATA);
    //now we have the offset.
    return offset * 2; /* Position * size of character cell */
}

void set_cursor_offset(int offset) {
    offset /= 2;
    //telling the port, we setting the high bit.
    port_byte_out(REG_SCREEN_CTRL, 14);
    //setting the high bit
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    //telling the port, we setting the low bit.
    port_byte_out(REG_SCREEN_CTRL, 15);
    //setting the low bit.
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

void clear_screen() {
    int screen_size = MAX_COLS * MAX_ROWS;
    int i;
    char *screen = (char*) VIDEO_ADDRESS;

    for (i = 0; i < screen_size; i++) {
        screen[i*2] = ' ';
        screen[i*2+1] = WHITE_ON_BLACK;
    }
    set_cursor_offset(get_offset(0, 0));
}


int get_offset(int col, int row) { 
    return 2 * (row * MAX_COLS + col); 
}
int get_offset_row(int offset) { 
    return offset / (2 * MAX_COLS);
}
int get_offset_col(int offset) {
    return (offset - (get_offset_row(offset)*2*MAX_COLS))/2;
}
