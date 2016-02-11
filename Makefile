BIN=lightbox
OBJS=lightbox.o 74hc595.o leds.o uart.o

CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-Os -DF_CPU=16000000 -mmcu=atmega32u4 -g
PORT=/dev/ttyACM0

${BIN}.hex: ${BIN}.elf
	${OBJCOPY} -O ihex -R .eeprom $< $@

${BIN}.elf: ${OBJS}
	${CC} -mmcu=atmega32u4 -g -o $@ $^

install: ${BIN}.hex
	python reset.py -P ${PORT}
#	sleep 2
	avrdude -patmega32u4 -cavr109 -P$(PORT) -b9600 -D -Uflash:w:$<


clean:
	rm -f ${BIN}.elf ${BIN}.hex ${OBJS}
