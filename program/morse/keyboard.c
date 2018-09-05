#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "v-usb/usbdrv/usbdrv.h"

typedef struct {
  uint8_t modifier;
  uint8_t reserved;
  uint8_t key_1;
  uint8_t key_2;
  uint8_t key_3;
  uint8_t key_4;
  uint8_t key_6;
  uint8_t key_7;
} key_report_t;

static key_report_t key_report = {0, 0, 0, 0, 0, 0, 0, 0};
const key_report_t ZERO PROGMEM = {0, 0, 0, 0, 0, 0, 0, 0};
static uchar    idleRate;   /* repeat rate for keyboards, never used for mice */

void keyboard_init() {
  int i;
  for(i=0; i<sizeof(key_report); i++) // clear report initially
    ((uchar *)&key_report)[i] = 0;

  usbInit();

  usbDeviceDisconnect(); // enforce re-enumeration
  for(i = 0; i<250; i++) { // wait 500 ms
    wdt_reset(); // keep the watchdog happy
    _delay_ms(2);
  }
  usbDeviceConnect();
}

void keyboard_output(uint8_t key) {
  usbPoll();

  while (!usbInterruptIsReady()) {
    _delay_ms(2);
    // wait;
  }
  usbSetInterrupt((void *)&key_report, sizeof(key_report));

  while (!usbInterruptIsReady()) {
    _delay_ms(2);
    // wait;
  }
  usbSetInterrupt((void *)&ZERO, sizeof(ZERO));
}

// callback defined in usbdrv.h
usbMsgLen_t usbFunctionSetup(uchar data[8])
{
  usbRequest_t    *rq = (void *)data;

  /* The following requests are never used. But since they are required by
   * the specification, we implement them in this example.
   */
  if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* class request type */
    if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
      /* we only have one report type, so don't look at wValue */
      usbMsgPtr = (void *)&ZERO;
      return sizeof(ZERO);
    }else if(rq->bRequest == USBRQ_HID_GET_IDLE){
      usbMsgPtr = &idleRate;
      return 1;
    }else if(rq->bRequest == USBRQ_HID_SET_IDLE){
      idleRate = rq->wValue.bytes[1];
    }
  }else{
    /* no vendor specific requests implemented */
  }
  return 0;   /* default for not implemented requests: return no data back to host */
}
