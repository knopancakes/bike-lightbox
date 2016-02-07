BIN=lightbox
OBJS=lightbox.o 74hc595.o leds.o

CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-Os -DF_CPU=16000000UL -mmcu=atmega32u4
PORT=/dev/ttyACM0

${BIN}.hex: ${BIN}.elf
	${OBJCOPY} -O ihex -R .eeprom $< $@

${BIN}.elf: ${OBJS}
	${CC} -o $@ $^

install: ${BIN}.hex
	avrdude -F -V -c avr109 -p ATMEGA32U4 -P ${PORT} -b 57600 -D -U flash:w:$<


clean:
	rm -f ${BIN}.elf ${BIN}.hex ${OBJS}
