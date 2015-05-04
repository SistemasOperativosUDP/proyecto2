# This needs gcc-4.9 or higher
CFLAGS = -c -g -Os -march=i386 -ffreestanding -Wall -Werror -m32
ASFLAGS =
#LDFLAGS = -Ttext 0x7c00 --oformat=binary

SOURCES = \
	snakeHash.c
EXECUTABLES = $(SOURCES:.S=.bin) $(SOURCES:.c=.bin)
IMAGES = $(EXECUTABLES:.bin=.img)

all: objects binary boot

objects: $(SOURCES)

binary: $(EXECUTABLES)

boot: $(IMAGES)

%.img: %.bin
	dd if=/dev/zero of=$@ bs=512 count=2880
	dd if=$< of=$@

%.elf: %.c.o
	ld $(LDFLAGS) $< -o $@

%.bin: %.elf
	objcopy -O binary $< $@

%.bin: %.asm.o
	ld $(LDFLAGS) $< -o $@

%.asm.o: %.S
	$(eval LDFLAGS = -Ttext 0x7c00 --oformat=binary)
	as $(ASFLAGS) $< -o $@

%.c.o: %.c
	$(eval LDFLAGS = -m elf_i386 -static -T$(<:.c=.ld) -nostdlib --nmagic )
	gcc $(CFLAGS) $< -o $@

clean:
	$(RM) *.o *.bin *.img

