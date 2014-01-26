
CPU=arm7tdmi-s

CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar
AS=arm-none-eabi-as
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy

CFLAGS += -mcpu=$(CPU) -ffunction-sections -fdata-sections
CFLAGS += -Os -Wall
CFLAGS += -I./str75x_lib/inc -I./USBLibrary/inc -I./include -I./

ASFLAGS += -mcpu=$(CPU)

LD_EXTRA_FLAGS = -Wl,-Map=main.map,--cref,--gc-sections -nostartfiles -nostdlib -mcpu=$(CPU)

CSRCS = 75x_it.c buffers.c Checksum.c commands.c configuration.c \
        i2c_ee.c leds.c lists.c low_level_init.c main.c messages.c \
        USBApp/hw_config.c USBApp/usb_prop.c USBApp/usb_desc.c \
	USBApp/usb_pwr.c USBApp/usb_istr.c USBApp/usb_endp.c
ASMSRCS = system/startup.s system/vector.s


COBJS = $(CSRCS:.c=.o)
ASMOBJS = $(ASMSRCS:.s=.o)

all: main.bin main.hex

str75xlib.a:
	make -C str75x_lib

usblib.a:
	make -C USBLibrary

main.out: str75xlib.a usblib.a $(ASMOBJS) $(COBJS)
	$(CC) $(LD_EXTRA_FLAGS) -T./system/target.ld $(ASMOBJS) $(COBJS) str75x_lib/str75xlib.a USBLibrary/usblib.a -o main.out

main.bin: main.out
	$(OBJCOPY) -R .configuration -O binary main.out main.bin

main.hex: main.out
	$(OBJCOPY) -R .configuration -O ihex main.out main.hex


clean:
	make -C str75x_lib clean
	make -C USBLibrary clean
	rm -rf main.out
	rm -rf main.bin
	rm -rf main.hex
	rm -rf main.map
	rm -rf *.o
	rm -rf system/*.o
	rm -rf USBApp/*.o

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o : %.s
	$(AS) $(ASFLAGS) -o $@ -g $<

