#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "v-usb/usbdrv/usbdrv.h"


PROGMEM const char usbDescriptorHidReport[63] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)(Key Codes)
    0x19, 0x00,                    //   USAGE_MINIMUM
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)(231)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs) ; Modifier byte
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x01,                    //   INPUT (Cnst,Var,Abs) ; Reserved byte
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs) ; LED report
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x91, 0x01,                    //   OUTPUT (Cnst,Var,Abs) ; LED report padding
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)(Key Codes)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))(0)
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)(101)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0                           // END_COLLECTION
};

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
    usbPoll();
  }
  usbSetInterrupt((void *)&key_report, sizeof(key_report));

  while (!usbInterruptIsReady()) {
    _delay_ms(2);
    // wait;
    usbPoll();
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
