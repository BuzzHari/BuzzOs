#Exapnds to list of files that match the patterns.
C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h)

#Creates a list of objects files to build,
#by replacing the '.c' extensions in C_SOURCES with '.o'
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}

CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb

CFLAGS = -g

#Creates os-image by concatinating the binary files.
os-image.bin: boot/bootsect.bin kernel.bin
		cat $^ > os-image.bin

kernel.bin: boot/kernel_entry.o ${OBJ}
		i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

#for debug
kernel.elf: boot/kernel_entry.o ${OBJ}
		i386-elf-ld -o $@ -Ttext 0x1000 $^


run: os-image.bin
		qemu-system-i386 -fda os-image.bin
		#qemu-system-i386  -drive file=os-image.bin,format=raw,index=0,media=disk

debug: os-image.bin kernel.elf
		qemu-system-i386 -s -fda os-image.bin &
		${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"


# Generic rule for building 'somefile.o' from 'somefile.c'
%.o: %.c ${HEADERS}
		${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
		nasm $< -f elf -o $@
%.bin: %.asm
		nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o os-image.bin *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o



