#include "utils.h"

void mem_copy(unsigned char *source,unsigned char *dest, int no_bytes){
    for (int i = 0; i < no_bytes; i++){
        *(dest + i) = *(source + i);
    }
}
