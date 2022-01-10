/*
  wiring_digital.c - Functions related to digital I/O on normal Arduino
                     boards.

  This file is licensed under the the MIT License.

  Copyright (c) 2014 Matthijs Kooijman <matthijs@stdin.nl>

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "Arduino.h"

struct pin_state {
	uint8_t mode;
	uint8_t value;
};

struct pin_state pins[NUM_DIGITAL_PINS];

void pinMode(uint8_t pin, uint8_t mode)
{
  if (pin >= NUM_DIGITAL_PINS)
    return;
  pins[pin].mode = mode;
}

void digitalWrite(uint8_t pin, uint8_t val)
{
  if (pin >= NUM_DIGITAL_PINS)
    return;

  switch (pins[pin].mode) {
    case INPUT:
    case INPUT_PULLUP:
      pins[pin].mode = (val ? INPUT_PULLUP : INPUT);
      break;
    case OUTPUT:
      pins[pin].value = val ? HIGH : LOW;
      break;
  }
}

int digitalRead(uint8_t pin)
{
  if (pin >= NUM_DIGITAL_PINS)
    return LOW;

  switch (pins[pin].mode) {
    case INPUT:
      return LOW;
    case INPUT_PULLUP:
      return HIGH;
    case OUTPUT:
      return pins[pin].value;
  }
  return LOW;
}

/* vim: set sw=2 sts=2 expandtab: */
