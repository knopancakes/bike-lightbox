BIN=$(BUILDDIR)/lightbox
PWD=$(`pwd`)
SOURCEDIR=$(PWD)src
HEADERDIR=$(PWD)inc
BUILDDIR=$(PWD)bin
CSOURCES := $(shell find $(SOURCEDIR) -name '*.c')
CXXSOURCES := $(shell find $(SOURCEDIR) -name '*.cpp')
OBJECTS := $(addprefix $(BUILDDIR)/,$(CSOURCES:%.c=%.c.o)) $(addprefix $(BUILDDIR)/,$(CXXSOURCES:%.cpp=%.cpp.o))

MCU=atmega32u4
CC=avr-gcc
CXX=avr-g++
OBJCOPY=avr-objcopy
CFLAGS=-g -Wall -Os -DF_CPU=16000000UL -mmcu=atmega32u4 -DDEBUG=y
CXXFLAGS=-g -Wall -Os -DF_CPU=16000000UL -mmcu=atmega32u4 -DDEBUG=y
PORT=/dev/ttyACM0
DEBUG_FLAG=

dir_guard=@mkdir -p $(@D)       

${BIN}.hex: ${BIN}.elf
	${OBJCOPY} -O ihex -R .eeprom $< $@

${BIN}.elf: $(OBJECTS)
	${CXX} -mmcu=atmega32u4 -o $@ $^

$(BUILDDIR)/%.c.o: %.c
	$(dir_guard)
	$(CC) ${CFLAGS} -I$(HEADERDIR) -I$(dir $<) -c $< -o $@

$(BUILDDIR)/%.cpp.o: %.cpp
	$(dir_guard)
	$(CXX) ${CXXFLAGS} -I$(HEADERDIR) -I$(dir $<) -c $< -o $@

echo:
	@echo $(CSOURCES)
	@echo $(CXXSOURCES)
	@echo $(OBJECTS)


install: ${BIN}.hex
	python reset.py -P ${PORT}
	avrdude -patmega32u4 -cavr109 -P$(PORT) -b9600 -D -Uflash:w:$<

clean:
	rm -f ${BIN}.elf ${BIN}.hex ${OBJECTS}
