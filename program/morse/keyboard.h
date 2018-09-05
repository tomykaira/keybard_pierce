#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <avr/io.h>

void keyboard_init();

void keyboard_output(uint8_t key);

#endif /* _KEYBOARD_H_ */
