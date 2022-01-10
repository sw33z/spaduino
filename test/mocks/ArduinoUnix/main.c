/*
  main.c - Initialization of the ArduinoUnix environment

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

// wiring.c
void wiring_init(void);

/**
 * Initialization routine that should be called early in the main()
 * function.
 */
void init(void)
{
  wiring_init();
}

/*
 * Define all of setup(), loop() and main() weakly, so that any or all
 * of them can be overridden in the sketch.
 */

#define UNUSED(x) (void)(x)

void setup(void) __attribute__((__weak__));
void setup(void) { }

void loop(void) __attribute__((__weak__));
void loop(void) { }

int main(int argc, char **argv) __attribute__((__weak__));
int main(int argc, char **argv) {
  UNUSED(argv);
  UNUSED(argc);
  init();
  setup();
  while(1)
    loop();
}

/* vim: set sw=2 sts=2 expandtab: */
