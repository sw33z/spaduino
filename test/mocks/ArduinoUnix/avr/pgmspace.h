/*
  avr/pgmspace.h - Dummy implementation of progmem-related types and
		   functions (that on avr take care of storing data in
		   the program flash)

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

#ifndef AvrPgmspace_h
#define AvrPgmspace_h

#include <inttypes.h>

#define __ATTR_CONST__
#define __ATTR_PROGMEM__
#define __ATTR_PURE__
#define PROGMEM __ATTR_PROGMEM__

#define PGM_P const char *
#define PGM_VOID_P const void *

#if defined(__PROG_TYPES_COMPAT__)
typedef void prog_void;
typedef char prog_char;
typedef unsigned char prog_uchar;
typedef int8_t    prog_int8_t;
typedef uint8_t   prog_uint8_t;
typedef int16_t   prog_int16_t;
typedef uint16_t  prog_uint16_t;
typedef int32_t   prog_int32_t;
typedef uint32_t  prog_uint32_t;
typedef int64_t   prog_int64_t;
typedef uint64_t  prog_uint64_t;
#endif

#define PSTR(s) (__extension__({static const char __c[] = (s); &__c[0];}))

#define pgm_read_byte_near(addr) (*(uint8_t*) (addr))
#define pgm_read_word_near(addr) (*(uint16_t *) (addr))
#define pgm_read_dword_near(addr) (*(uint32_t *) (addr))
#define pgm_read_float_near(addr) (*(float *) (addr))

#define pgm_read_byte_far(addr) (*(uint8_t*) (addr))
#define pgm_read_word_far(addr) (*(uint16_t *) (addr))
#define pgm_read_dword_far(addr) (*(uint32_t *) (addr))
#define pgm_read_float_far(addr) (*(float *) (addr))

#define pgm_read_byte(addr) (*(uint8_t*) (addr))
#define pgm_read_word(addr) (*(uint16_t *) (addr))
#define pgm_read_dword(addr) (*(uint32_t *) (addr))
#define pgm_read_float(addr) (*(float *) (addr))

#define memchr_P memchr
#define memcmp_P memcmp
#define memccpy_P memccpy
#define memcpy_P memcpy
#define memmem_P memmem
#define memrchr_P memrchr
#define strcat_P strcat
#define strchr_P strchr
#define strchrnul_P strchrnul
#define strcmp_P strcmp
#define strcpy_P strcpy
#define strcasecmp_P strcasecmp
#define strcasestr_P strcasestr
#define strcspn_P strcspn
#define strlcat_P strlcat
#define strlcpy_P strlcpy
#define strlen_P strlen
#define strnlen_P strnlen
#define strncmp_P strncmp
#define strncasecmp_P strncasecmp
#define strncat_P strncat
#define strncpy_P strncpy
#define strpbrk_P strpbrk
#define strrchr_P strrchr
#define strsep_P strsep
#define strspn_P strspn
#define strstr_P strstr
#define strtok_P strtok
#define strtok_rP strtok_r
#define strlen_PF strlen
#define strnlen_PF strnlen
#define memcpy_PF memcpy
#define strcpy_PF strcpy
#define strncpy_PF strncpy
#define strcat_PF strcat
#define strlcat_PF strlcat
#define strncat_PF strncat
#define strcmp_PF strcmp
#define strncmp_PF strncmp
#define strcasecmp_PF strcasecmp
#define strncasecmp_PF strncasecmp
#define strstr_PF strstr
#define strlcpy_PF strlcpy
#define memcmp_PF memcmp

#endif // AvrPgmspace_h
