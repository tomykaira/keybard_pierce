#ifndef _MORSE_CODE_H_
#define _MORSE_CODE_H_

#define N 0
#define S 1
#define L 2

#include <avr/pgmspace.h>
#include "./usb_hid_keys.h"

const uint8_t morse[][5] PROGMEM = {
  [KEY_A] = {S, L, N, N, N},
  [KEY_B] = {L, S, S, S, N},
  [KEY_C] = {L, S, L, S, N},
  [KEY_D] = {L, S, S, N, N},
  [KEY_E] = {S, N, N, N, N},
  [KEY_F] = {S, S, L, S, N},
  [KEY_G] = {L, L, S, N, N},
  [KEY_H] = {S, S, S, S, N},
  [KEY_I] = {S, S, N, N, N},
  [KEY_J] = {S, L, L, L, N},
  [KEY_K] = {L, S, L, N, N},
  [KEY_L] = {S, L, S, S, N},
  [KEY_M] = {L, L, N, N, N},
  [KEY_N] = {L, S, N, N, N},
  [KEY_O] = {L, L, L, N, N},
  [KEY_P] = {S, L, L, S, N},
  [KEY_Q] = {L, L, S, L, N},
  [KEY_R] = {S, L, S, N, N},
  [KEY_S] = {S, S, S, N, N},
  [KEY_T] = {L, N, N, N, N},
  [KEY_U] = {S, S, L, N, N},
  [KEY_V] = {S, S, S, L, N},
  [KEY_W] = {S, L, L, N, N},
  [KEY_X] = {L, S, S, L, N},
  [KEY_Y] = {L, S, L, L, N},
  [KEY_Z] = {L, L, S, S, N},
  [KEY_1] = {S, L, L, L, L},
  [KEY_2] = {S, S, L, L, L},
  [KEY_3] = {S, S, S, L, L},
  [KEY_4] = {S, S, S, S, L},
  [KEY_5] = {S, S, S, S, S},
  [KEY_6] = {L, S, S, S, S},
  [KEY_7] = {L, L, S, S, S},
  [KEY_8] = {L, L, L, S, S},
  [KEY_9] = {L, L, L, L, S},
  [KEY_0] = {L, L, L, L, L},

  // extension
  [KEY_ENTER] = {L, S, L, S, L},
  [KEY_BACKSPACE] = {S, S, L, S, S},
};

#endif /* _MORSE_CODE_H_ */
