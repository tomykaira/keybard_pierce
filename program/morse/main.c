#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>

#include "./keyboard.h"
#include "keymap.h"

#define TCNT_RESET_VAL 15 // 1 kHz?
#define LONG_PRESS_THRESHOLD 200
#define PAUSE_PRESS_THRESHOLD 200

#define DIFF_UINT16(x, y) ((x) > (y) ? ((x) - (y)) : (0xffff - (y) - (x) + 1))
#define HIGH_IF(b, n) (PORTB = (b) ? (PORTB | (1 << (n))) : (PORTB ^ (1 << (n))))

static uint16_t tick = 0;
static uint16_t last_down = 0;
static uint16_t last_up = 0;

IVR(TIMER1_OVF_vect) {
  if (last_down == 0 && PINB5) {
    last_down = tick;
  }
  if (last_down != 0 && !PINB5) {
    last_up = tick;
  }

  tick ++;
	TCNT1 = TCNT_RESET_VAL;
  return 0;
}

static uint16_t val;
static uint8_t offset;

void init_signal() {
  // init morse state
  val = 0;
  offset = 0;
}

void put_signal(uint8_t signal) {
  val = signal << offset | val;
}

void send_key() {
  for (int i = 0; i < SZ_KEYMAP; ++i) {
    if (val == KEYMAP[i][0]) {
      keyboard_output(KEYMAP[i][1]);
      val = 0;
      offset = 0;
    }
  }
}

int main() {
  // enable 1s watchdog timer
  wdt_enable(WDTO_1S);

  // Clock init
	TCNT1 = TCNT_RESET_VAL; // 1 kHz?
	TCCR1 = (TCCR1 & 0xf0) | 0x0b; // PCK/1024
	TIMSK = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)

  // switch port init
  DDRB &= 0 << 5;
  PORTB |= 1 << 5;
  MCUCR &= 0 << 6;

  // LED init
  DDRB |= 1 << 0; // B
  DDRB |= 1 << 1; // G
  DDRB |= 1 << 2; // R

  init_signal();

  // keyboard (HID USB) init
  keyboard_init();

  sei();

  while (1) {
    wdt_reset(); // keep the watchdog happy

    if (last_down > 0 && last_up > 0) {
      put_signal(DIFF_UINT16(last_up, last_down) > LONG_PRESS_THRESHOLD ? L : S);
      last_down = 0;
    }
    if (last_up > 0 && last_down == 0 && DIFF_UINT16(tick, last_up) > PAUSE_PRESS_THRESHOLD) {
      send_key();
      last_up = 0;
    }

    HIGH_IF(val > 0, 0);
    HIGH_IF(DIFF_UINT16(tick, last_down) > LONG_PRESS_THRESHOLD, 1);
    HIGH_IF(last_down > 0, 2);
  }


  return 0;
}
