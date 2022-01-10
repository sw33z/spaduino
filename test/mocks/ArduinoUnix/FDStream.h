/*
  FDStream.h - Implementation of the Arduino Stream interface that
               writes to and reads from Unix file descriptors.

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

#ifndef FDStream_h
#define FDStream_h

#include <Stream.h>

/**
 * Stream class that reads and writes from a given FD
 */
class FDStream : public Stream {
  public:
    /**
     * Create a stream with the given FDs. Pass an FD of -1 to not
     * support reading (never has bytes available) or printing
     * (drops all bytes).
     */
    FDStream(int infd, int outfd);
    virtual size_t write(uint8_t);
    virtual int available();
    virtual int read();
    virtual int peek();
    virtual void flush();

    /**
     * Dummy begin method, for compatibility with sketches using
     * Serial.begin(baud_rate) */
    void begin(int) { };

  protected:
    int infd;
    int outfd;

    /**
     * A character read by peek() but not returned yet. -1 when nothing
     * was peeked yet.
     */
    int peeked;

    /**
     * The method to find out about available bytes.
     */
    enum {
      USE_IOCTL,
      USE_STAT,
      USE_POLL,
    } available_method;
};

extern FDStream StdioStream;

#endif // FDStream_h

/* vim: set sw=2 sts=2 expandtab: */
