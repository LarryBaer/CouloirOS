CC = i686-elf-gcc
LD = i686-elf-gcc
AS = nasm
CFLAGS = -c -g -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -ffreestanding -O2 -nostdlib -lgcc

CFILES := $(shell find ./ -type f -name '*.c')
ASMFILES := $(shell find ./ -type f -name '*.s')

COBJS := $(patsubst %.c,%.c.o,$(CFILES))
AOBJS := $(patsubst %.s,%.s.o,$(ASMFILES))

OBJFILES := $(COBJS) $(AOBJS)

all: couloiros.iso

%.c.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.s.o: %.s
	$(AS) -f elf32 $< -o $@

couloiros.bin: $(OBJFILES)
	$(LD) -T ./kernel/linker.ld $(LDFLAGS) -o $@ $(OBJFILES)

couloiros.iso: couloiros.bin
	mkdir -p isodir/boot/grub
	cp couloiros.bin isodir/boot/couloiros.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $@ isodir

clean:
	find . -type f -name '*.o' -delete
	rm -f couloiros.iso
	rm -f couloiros.bin
	rm -rf isodir

.PHONY: all clean