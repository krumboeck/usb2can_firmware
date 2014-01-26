CPU=arm7tdmi-s

CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar
AS=arm-none-eabi-as
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy

CFLAGS += -mcpu=$(CPU) -ffunction-sections -fdata-sections
CFLAGS += -Os -Wall
CFLAGS += -I./inc -I../include


SRCS = src/75x_adc.c src/75x_can.c src/75x_cfg.c src/75x_dma.c src/75x_eic.c src/75x_extit.c src/75x_gpio.c src/75x_i2c.c src/75x_lib.c src/75x_mrcc.c src/75x_pwm.c src/75x_rtc.c src/75x_smi.c src/75x_ssp.c src/75x_tb.c src/75x_tim.c src/75x_uart.c src/75x_wdg.c


OBJS = $(SRCS:.c=.o)

all: str75xlib.a

str75xlib.a: $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -rf str75xlib.a
	rm -rf src/*.o

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

