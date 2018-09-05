PHONY = flash

CC = avr-gcc
OBJCOPY = avr-objcopy
DUDE = avrdude

CFLAGS = -Wall -Os -Iusbdrv -mmcu=attiny85 -DF_CPU=16500000 -I.
OBJFLAGS = -j .text -j .data -O ihex
DUDEFLAGS = -p attiny85 -c arduino -v

OBJECTS = v-usb/usbdrv/usbdrv.o v-usb/usbdrv/usbdrvasm.o morse/main.o morse/keyboard.o

all: main.hex

flash: main.hex
	$(DUDE) $(DUDEFLAGS) -U flash:w:$<

eeprom: main.eep
	$(DUDE) $(DUDEFLAGS) -U eeprom:w:$<

# Housekeeping if you want it
clean:
	$(RM) *.o *.hex *.elf usbdrv/*.o

# From .elf file to .hex
%.hex: %.elf
	$(OBJCOPY) $(OBJFLAGS) $< $@

# Main.elf requires additional objects to the firmware, not just main.o
main.elf: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

# Without this dependance, .o files will not be recompiled if you change
# the config! I spent a few hours debugging because of this...
$(OBJECTS): usbconfig.h

# From C source to .o object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# From assembler source to .o object file
%.o: %.S
	$(CC) $(CFLAGS) -x assembler-with-cpp -c $< -o $@
