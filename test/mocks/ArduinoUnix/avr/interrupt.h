/*
  avr/interrupt.h - Dummy version of avr-specific interrupt macros.

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

#ifndef AvrInterrupts_h
#define AvrInterrupts_h

#define sei()
#define cli()

#define ISR(vector, ...) void vector (void)
#define SIGNAL(vector) void vector (void)
#define EMPTY_INTERRUPT(vector) void vector (void) { }
#define ISR_ALIAS(vector, target_vector)

#define reti() return

#endif // AvrInterrupts_h
