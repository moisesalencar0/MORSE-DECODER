CROSS_COMPILE = arm-none-eabi-

CC      = $(CROSS_COMPILE)gcc
LD      = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy

COMMON_FLAGS := \
	-mcpu=cortex-a8 \
	-Wall \
	-Wextra \
	-Werror \
	-Iapp \
	-Iboard \
	-Idrivers \
	-Iinc \
	-fno-builtin

CFLAGS_DEBUG  = -g -O0
CFLAGS_DEPLOY = -O2

LDFLAGS = -T boot/memmap.ld

SRCS = \
	boot/startup.s \
	app/main.c \
	drivers/timer.c \
	drivers/watchdog.c \
	drivers/uart_io.c \
	drivers/gpio.c \
	drivers/intc.c \
	board/board.c

OBJS = $(addprefix obj/, $(notdir $(SRCS:.c=.o)))
OBJS := $(OBJS:.s=.o)

TARGET = bin/temp_monitor

all: debug

debug: CFLAGS := $(COMMON_FLAGS) $(CFLAGS_DEBUG)
debug: $(TARGET).bin

release: CFLAGS := $(COMMON_FLAGS) $(CFLAGS_DEPLOY)
release: $(TARGET).bin


obj/%.o: app/%.c
	$(CC) $(CFLAGS) -c $< -o $@

obj/%.o: drivers/%.c
	$(CC) $(CFLAGS) -c $< -o $@

obj/%.o: board/%.c
	$(CC) $(CFLAGS) -c $< -o $@

obj/%.o: boot/%.s
	$(CC) $(CFLAGS) -c $< -o $@


$(TARGET).elf: $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@


deploy: release
	sudo cp $(TARGET).bin /srv/tftp/temp_monitor.bin

clean:
	rm -f obj/*.o
	rm -f bin/*.elf
	rm -f bin/*.bin
	rm -f bin/*.dump