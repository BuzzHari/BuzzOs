#ifndef UTILS_H
#define UTILS_H

#include "../cpu/types.h"

void mem_copy(unsigned char *source, unsigned char *dest, int no_bytes);
void int_to_ascii(int n, char str[]);
#endif
