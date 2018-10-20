# This is the part of the KeyPierce code that is usually written in C.
#
# Copyright (c) 2018 Megumi Tomita
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
PHONY = flash

CC = avr-gcc
OBJCOPY = avr-objcopy

CFLAGS = -Wall -Os -Iusbdrv -mmcu=attiny85 -DF_CPU=16000000 -I.
OBJFLAGS = -j .text -j .data -O ihex
DUDEFLAGS = -p attiny85 -c arduino -v

HEADERS = usbconfig.h keymap.h morse/TrinketKeyboardC.h
OBJECTS = v-usb/usbdrv/usbdrv.o v-usb/usbdrv/usbdrvasm.o morse/main.o morse/TrinketKeyboardC.o

all: main.hex

keymap.h: helper/makelut.rb
	ruby $<

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
$(OBJECTS): $(HEADERS)

# From C source to .o object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# From assembler source to .o object file
%.o: %.S
	$(CC) $(CFLAGS) -x assembler-with-cpp -c $< -o $@
