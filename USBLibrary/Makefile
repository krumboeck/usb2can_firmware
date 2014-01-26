CPU=arm7tdmi-s

CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar
AS=arm-none-eabi-as
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy

CFLAGS += -mcpu=$(CPU) -ffunction-sections -fdata-sections
CFLAGS += -Os -Wall
CFLAGS += -I./inc -I../include


SRCS = src/usb_core.c src/usb_init.c src/usb_int.c src/usb_mem.c src/usb_regs.c



OBJS = $(SRCS:.c=.o)

all: usblib.a

usblib.a: $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -rf usblib.a
	rm -rf src/*.o

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

