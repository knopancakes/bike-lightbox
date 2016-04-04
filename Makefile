BIN=$(BUILDDIR)/lightbox
PWD=$(`pwd`)
SOURCEDIR=$(PWD)src
HEADERDIR=$(PWD)inc
BUILDDIR=$(PWD)bin
SOURCES := $(shell find $(SOURCEDIR) -name '*.c')
OBJECTS := $(addprefix $(BUILDDIR)/,$(SOURCES:%.c=%.o))

MCU=atmega32u4
CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-Os -DF_CPU=16000000 -mmcu=atmega32u4 -g
PORT=/dev/ttyACM0

dir_guard=@mkdir -p $(@D)       

${BIN}.hex: ${BIN}.elf
	${OBJCOPY} -O ihex -R .eeprom $< $@

${BIN}.elf: $(OBJECTS)
	${CC} -mmcu=atmega32u4 -o $@ $^

$(BUILDDIR)/%.o: %.c
	$(dir_guard)
	$(CC) ${CFLAGS} -I$(HEADERDIR) -I$(dir $<) -c $< -o $@

install: ${BIN}.hex
#	python reset.py -P ${PORT}
#	sleep 2
#	avrdude -patmega32u4 -cavr109 -P$(PORT) -b9600 -D -Uflash:w:$<
	avrdude -patmega32u4 -cavr109 -P$(PORT) -b9600 -D -Uflash:w:$<

clean:
	rm -f $(BUILDDIR)/${BIN}.elf $(BUILDDIR)/${BIN}.hex ${OBJECTS}
