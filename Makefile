CC = gcc -m32 -fno-stack-protector -nostdlib -I include
ASM =  nasm -f elf32
LD = ld -m elf_i386 -T link.ld

all:
	$(ASM) boot.S -o boot.o
	$(CC) -c kmain.c -o kmain.o
	$(CC) -c stdlib.c -o stdlib.o
	$(CC) -c video.c -o video.o
	$(CC) -c keyboard.c -o keyboard.o
	$(CC) -c cmdline.c -o cmdline.o
	$(LD) boot.o video.o stdlib.o kmain.o cmdline.o -o kern

boot:
	qemu-system-x86_64 -kernel kern -append "nosplash"

clean:
	rm *.o
	rm kern
