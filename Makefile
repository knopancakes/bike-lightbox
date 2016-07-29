BIN=$(BUILDDIR)/lightbox
PWD=$(`pwd`)
SOURCEDIR=$(PWD)src
HEADERDIR=$(PWD)inc
BUILDDIR=$(PWD)bin
CSOURCES := $(shell find $(SOURCEDIR) -name '*.c')
CXXSOURCES := $(shell find $(SOURCEDIR) -name '*.cpp')
OBJECTS := $(addprefix $(BUILDDIR)/,$(CSOURCES:%.c=%.c.o)) 		\
	$(addprefix $(BUILDDIR)/,$(CXXSOURCES:%.cpp=%.cpp.o))	\
	lib/usbdrv/usbdrv.c.o lib/usbdrv/usbdrvasm.c.o lib/usbdrv/oddebug.c.o

MCU=atmega32u4
CC=avr-gcc
CXX=avr-g++
OBJCOPY=avr-objcopy
CFLAGS=-g -Wall -Os -DF_CPU=16000000UL -mmcu=atmega32u4 -DDEBUG=y -Iusbdrv -I./lib/usbdrv
CXXFLAGS=-g -Wall -Os -DF_CPU=16000000UL -mmcu=atmega32u4 -DDEBUG=y -Iusbdrv -I./lib/usbdrv
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

# Generic rule for compiling C files:
.c.o:
	$(COMPILE) -c $< -o $@

# Generic rule for assembling Assembler source files:
.S.o:
	$(CC) ${CFLAGS} -I$(HEADERDIR) -I$(dir $<) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

# Generic rule for compiling C to assembler, used for debugging only.
.c.s:
	$(CC) ${CFLAGS} -I$(HEADERDIR) -I$(dir $<) -S $< -o $@

echo:
	@echo $(CSOURCES)
	@echo $(CXXSOURCES)
	@echo $(OBJECTS)


install: ${BIN}.hex
	python reset.py -P ${PORT}
	avrdude -patmega32u4 -cavr109 -P$(PORT) -b9600 -D -Uflash:w:$<

clean:
	rm -f ${BIN}.elf ${BIN}.hex ${OBJECTS}
