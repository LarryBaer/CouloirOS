CFLAGS = -c -g -std=gnu99 -ffreestanding -O2 -Wall -Wextra

all:
	i686-elf-as ./kernel/boot.s -o boot.o
	i686-elf-as ./kernel/gdt.s -o gdt_asm.o
	i686-elf-as ./kernel/idt.s -o idt_asm.o
	i686-elf-gcc $(CFLAGS) ./kernel/kernel.c -o kernel.o
	i686-elf-gcc $(CFLAGS) ./kernel/gdt.c -o gdt.o
	i686-elf-gcc $(CFLAGS) ./kernel/idt.c -o idt.o
	i686-elf-gcc $(CFLAGS) ./kernel/util.c -o util.o
	i686-elf-gcc -T ./kernel/linker.ld -o couloiros.bin -ffreestanding -O2 -nostdlib -lgcc util.o idt.o idt_asm.o gdt.o gdt_asm.o boot.o kernel.o
	mkdir -p isodir/boot/grub
	cp couloiros.bin isodir/boot/couloiros.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o couloiros.iso isodir

clean:
	rm -f *.o
	rm -f couloiros.iso
	rm -f couloiros.bin
	rm -rf isodir