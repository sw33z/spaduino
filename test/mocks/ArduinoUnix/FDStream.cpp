/*
  FDStream.cpp - Implementation of the Arduino Stream interface that
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

#include "FDStream.h"

#ifndef _WIN32
#include <sys/ioctl.h>
#include <poll.h>
#endif
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

FDStream StdioStream(0, 1);

FDStream::FDStream(int infd, int outfd) : infd(infd), outfd(outfd) {
  // Always first try the ioctl method
  this->available_method = USE_IOCTL;
}

int FDStream::available() {
#ifndef _WIN32
  if (this->infd < 0)
    return 0;

  if (this->available_method == USE_IOCTL) {
    int available = 0;
    if (ioctl(this->infd, FIONREAD, &available) != -1) {
      return available;
    } else {
      if (errno == ENOTTY) {
        // This was probably not a TTY but a file, try using stat
        this->available_method = USE_STAT;
      } else {
        // Other error, complain
        fprintf(stderr, "FIONREAD ioctl failed: %s\n", strerror(errno));
        return 0;
      }
    }
  }

  if (this->available_method == USE_STAT) {
    struct stat s;
    // If the file is a regular file, calculate available from the
    // filesize and current position
    if ((fstat (this->infd, &s) == 0) && S_ISREG (s.st_mode)) {
      off_t n = lseek (this->infd, 0, SEEK_CUR);
      if (n != -1) {
        return s.st_size - n;
      } else {
        fprintf(stderr, "lseek failed: %s\n", strerror(errno));
      }
    } else {
      // Not a regular file, try using select
      this->available_method = USE_POLL;
    }
  }

  if (this->available_method == USE_POLL) {
    struct pollfd pfd = {this->infd, POLLIN, 0 };

    if (poll(&pfd, 1, 0) != -1)
      return pfd.revents & POLLIN ? 1 : 0;
    else
      fprintf(stderr, "select failed: %s\n", strerror(errno));
  }
#endif
  // Everything failed? Nothing available then.
  return 0;
}

int FDStream::read() {
  if (this->infd < 0)
    return -1;

  if (this->peeked >= 0) {
    int p = this->peeked;
    this->peeked = -1;
    return p;
  }

  if (!this->available())
    return -1;

  // This should not block, since available says there is data
  // available... (but setting O_NONBLOCK might cause problems if our
  // caller also reads from this file descriptor...).
  uint8_t b;
  int res = ::read(this->infd, &b, 1);
  if (res == 1)
    return b;

  fprintf(stderr, "read failed: %s\n", strerror(errno));
  return -1;
}

int FDStream::peek() {
  return this->peeked = read();
}

void FDStream::flush() {
#ifndef _WIN32
  if (this->infd >= 0) {
#if _POSIX_SYNCHRONIZED_IO > 0
    fdatasync(this->infd);
#else
    // OSX doesn't have fdatasync
    fsync(this->infd);
#endif
  }
#endif
}

size_t FDStream::write(uint8_t b) {
  if (this->outfd < 0)
    return 0;
  return ::write(this->outfd, &b, 1);
}

/* vim: set sw=2 sts=2 expandtab: */
