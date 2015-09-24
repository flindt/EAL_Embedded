XCC     = arm-none-eabi-gcc
LD      = arm-none-eabi-ld
CFLAGS  = -mthumb             \
       -mcpu=cortex-m3     \
       -Os                 \
       -ffunction-sections \
       -fdata-sections     \
       -MD                 \
       -std=c99            \
       -Wall               \
       -pedantic           \
       -DPART_${PART}      \
	-g -c -Wall -I/shared/toolchains/SW-EK-LM3S6965/inc/ -I/shared/toolchains/SW-EK-LM3S6965/driverlib -I/shared/toolchains/SW-EK-LM3S6965/
LDFLAGS =   --gc-sections -Map tryBuzzer.map -T standalone.ld -N \
	-L/shared/toolchains/SW-EK-LM3S6965/driverlib/gcc-cm3/ -ldriver-cm3

all: tryBuzzer.elf

rit128x96x4.o: rit128x96x4.c rit128x96x4.h
	$(XCC) $(CFLAGS) rit128x96x4.c

startup.o: startup.c 
	$(XCC) $(CFLAGS) startup.c

main.o: main.c
	$(XCC) $(CFLAGS) main.c

tryBuzzer.elf: main.o startup.o rit128x96x4.o standalone.ld
	$(LD) -o $@ main.o startup.o rit128x96x4.o $(LDFLAGS)

clean:
	-rm -f tryBuzzer.elf *.o tryBuzzer.map
