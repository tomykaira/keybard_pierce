#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <string.h>

#include "./TrinketKeyboardC.h"
#include "keymap.h"

#define LONG_PRESS_THRESHOLD 3
#define PAUSE_PRESS_THRESHOLD 6

#define DIFF_UINT16(x, y) ((x) >= (y) ? ((x) - (y)) : ((0xffff - (y) + (x)) + 1))
#define LOW_IF(b, n) (PORTB = ((b) ? (PORTB & ~_BV(n)) : (PORTB | _BV(n))))
#define RESETENABLED 1
#if RESETENABLED
#  define SWITCH_PIN 2
#  define LED_1 0
#  define LED_2 1
#else
#  define SWITCH_PIN 5
#  define LED_1 1
#  define LED_2 2
#  define LED_3 0
#endif

volatile static uint16_t tick = 0;
volatile static uint16_t last_down = 0;
volatile static uint16_t last_up = 0;
volatile static uint16_t last_duration = 0;
volatile static uint8_t flush = 0;

volatile static uint16_t val;
volatile static uint8_t offset;

void init_signal() {
  // init morse state
  val = 0;
  offset = 0;
  tick = 0;
  last_down = 0;
  last_up = 0;
  last_duration = 0;
  flush = 0;
}

void reset_signals() {
  val = 0;
  offset = 0;
}

void put_signal(uint8_t signal) {
  val = (signal << offset) | val;
  offset += 2;
  if (offset / 2 >  6) { // not supported
    reset_signals();
  }
}

void send_key() {
  unsigned char key = keymap_morse_to_hid_key(val);
  if (key) {
    reset_signals();
    usbReportSendKey(key);
    usbReportSendKey(0);
  }
}

void send_nyan() {
  usbReportSendKey(0x11);
  usbReportSendKey(0);
  usbReportSendKey(0x1c);
  usbReportSendKey(0);
  usbReportSendKey(0x04);
  usbReportSendKey(0);
  usbReportSendKey(0x2d);
  usbReportSendKey(0);
  usbReportSendKey(0x11);
  usbReportSendKey(0);
  usbReportSendKey(0x11);
  usbReportSendKey(0);
  usbReportSendKey(0x28);
  usbReportSendKey(0);
}

static inline void initTimer1(void)
{
  TCCR1 |= (1 << CTC1);  // clear timer on compare match
  TCCR1 |= (1 << CS13) | (1 << CS12) | (1 << CS11); //clock prescaler 8192
  TCCR1 &= ~(1 << CS10); //clock prescaler 8192
  OCR1C = 122; // compare match value
  TIMSK |= (1 << OCIE1A); // enable compare match interrupt
}

ISR(TIMER1_COMPA_vect)
{
  char switch_pressed = (PINB & _BV(SWITCH_PIN)) == 0;
  if (last_down == 0 && switch_pressed) {
    last_down = tick;
    last_up = 0;
  }
  if (last_down != 0 && !switch_pressed) {
    last_up = tick;
    last_duration = DIFF_UINT16(last_up, last_down);
    last_down = 0;
  }
  if (last_up > 0) {
    if (!flush) {
      flush = DIFF_UINT16(tick, last_up) > PAUSE_PRESS_THRESHOLD;
    }
    if (flush) {
      last_up = 0;
    }
  }
  tick++;
}



int main(void)
{
  wdt_enable(WDTO_1S);
	// initializations
	DDRB |= _BV(LED_1) | _BV(LED_2);
#if !RESETENABLED
	DDRB |= _BV(LED_3);
#endif
	initTimer1();        // initialize timer registers
  DDRB &= ~_BV(SWITCH_PIN);
  MCUCR &= ~_BV(6);
  init_signal();

  // keyboard (HID USB) init
  usbBegin();
	sei();               // enable interrupts

  while(1)
	{
    wdt_reset(); // keep the watchdog happy
    usbTick();

    if (last_duration > 0) {
      send_nyan();
      /* put_signal(last_duration > LONG_PRESS_THRESHOLD ? L : S); */
      last_duration = 0;
    }
    /* if (flush) { */
    /*   send_key(); */
    /*   last_up = 0; */
    /*   flush = 0; */
    /* } */
    LOW_IF(last_down == 0, LED_1);
    LOW_IF(val > 0, LED_2);
    PORTB |= _BV(SWITCH_PIN);

#if !RESETENABLED
    LOW_IF(val > 0, LED_3);
#endif
	}
	return 0;
}
