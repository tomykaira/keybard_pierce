#include <avr/wdt.h>

#include "./keyboard.h"
#include "./morse_code.h"
#include "./usb_hid_keys.h"

int main() {
  wdt_enable(WDTO_1S); // enable 1s watchdog timer

  keyboard_init();

  while (1) {
    wdt_reset(); // keep the watchdog happy
    keyboard_output(KEY_A);
  }

  return 0;
}
