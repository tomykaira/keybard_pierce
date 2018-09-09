#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "./keyboard.h"
#include "keymap.h"

#define TCNT_RESET_VAL 15 // 1 kHz?
#define LONG_PRESS_THRESHOLD 200
#define PAUSE_PRESS_THRESHOLD 200

#define DIFF_UINT16(x, y) ((x) > (y) ? ((x) - (y)) : (0xffff - (y) - (x) + 1))
#define LOW_IF(b, n) (PORTB = (b) ? (PORTB ^ (1 << (n))) : (PORTB | (1 << (n))))
#define RESETENABLED 1
#if RESETENABLED
#  define SWITCH_PIN 2
#else
#  define SWITCH_PIN 5
#endif

volatile static uint16_t tick = 0;
volatile static uint16_t last_down = 0;
volatile static uint16_t last_up = 0;


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

volatile uint8_t i = 0;

static inline void initTimer1(void)
{
  TCCR1 |= (1 << CTC1);  // clear timer on compare match
  TCCR1 |= (1 << CS13) | (1 << CS12) | (1 << CS11); //clock prescaler 8192
  OCR1C = 122; // compare match value 
  TIMSK |= (1 << OCIE1A); // enable compare match interrupt
}
ISR(TIMER1_COMPA_vect)
{
  char switch_pressed = (PINB & (1 << SWITCH_PIN));
  if (last_down == 0 && switch_pressed) {
    last_down = tick;
  }
  if (last_down != 0 && !switch_pressed) {
    last_up = tick;
  }
  tick ++;
}



int main(void)
{
  wdt_enable(WDTO_1S);
	// initializations 
	DDRB = 0x0F;         // enable PB0-PB3 as outputs
	initTimer1();        // initialize timer registers
  DDRB ^= 0 << SWITCH_PIN;
  PORTB |= 1 << SWITCH_PIN;
  MCUCR ^= 0 << 6;
  init_signal();

  // keyboard (HID USB) init
  keyboard_init();
	sei();               // enable interrupts
	
	while(1)
	{
    wdt_reset(); // keep the watchdog happy

    if (last_down > 0 && last_up > 0) {
      put_signal(DIFF_UINT16(last_up, last_down) > LONG_PRESS_THRESHOLD ? L : S);
      last_down = 0;
    }
    if (last_up > 0 && last_down == 0 && DIFF_UINT16(tick, last_up) > PAUSE_PRESS_THRESHOLD) {
      send_key();
      last_up = 0;
    }

    LOW_IF(last_down > 0, 0);
    LOW_IF(DIFF_UINT16(tick, last_down) > LONG_PRESS_THRESHOLD, 1);
#if !RESETENABLED
    LOW_IF(val > 0, 2);
#endif
      keyboard_output(29);
	}
	
	return 0;
}
