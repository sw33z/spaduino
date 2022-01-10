/*
  xtoa.c - Functions for converting numbers to ascii, that are normally
           implemented by avr-libc.

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

#include "xtoa.h"
#include <stdio.h>

// These functions are provided in stdlib.h by avr-libc, but not on (all) other
// architectures.
//
// Documentation is copied from avr-libc, implmentation is done from
// scratch (since avr-libc only contains an assembly implementation).

char* ultoa(unsigned long val, char* s, int radix) {
  static const char dig[] = "0123456789abcdefghijklmnopqrstuvwxyz";
  char* p, *q;

  q = s;
  do {
    *q++ = dig[val % radix];
    val /= radix;
  } while (val);
  *q = '\0';

  // Reverse the string (but leave the \0)
  p = s;
  q--;

  while (p < q) {
    char c = *p;
    *p++ = *q;
    *q-- = c;
  }

  return s;
}

char* utoa(int val, char* s, int radix) {
  return ultoa(val, s, radix);
}

char* ltoa(long val, char* s, int radix) {
  if (val < 0) {
    *s = '-';
    utoa(-val, s + 1, radix);
  } else {
    utoa(val, s, radix);
  }
  return s;
}

char* itoa(int val, char* s, int radix) {
  return ltoa(val, s, radix);
}

char *dtostrf(double val, signed char width,
                     unsigned char prec, char *s) {
  sprintf(s, "%*.*f", width, prec, val);
  return s;
}

/* vim: set sw=2 sts=2 expandtab: */
