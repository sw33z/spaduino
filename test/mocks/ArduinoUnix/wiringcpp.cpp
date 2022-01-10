#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include "TimerMock.h"

struct timespec startup_time;

#ifndef _WIN32
#define CLOCK_TO_USE CLOCK_REALTIME
#endif

static int gettime_internal_mock(struct timespec *time)
{
#ifndef _WIN32
return clock_gettime(CLOCK_TO_USE, time);
#else
return 0;
#endif
}

struct timespec time_diff_mock(struct timespec start, struct timespec end) {
  struct timespec temp;
  if ((end.tv_nsec - start.tv_nsec) < 0) {
    temp.tv_sec = end.tv_sec-start.tv_sec-1;
    temp.tv_nsec = 1E9 +  end.tv_nsec-start.tv_nsec;
  } else {
    temp.tv_sec = end.tv_sec - start.tv_sec;
    temp.tv_nsec = end.tv_nsec - start.tv_nsec;
  }
  return temp;
}

unsigned long millis()
{
  
    TimerMock * mock = TimerMock::getInstance();
    if (mock->sendMockValue)
    {
        return mock->time;
    }

  struct timespec current_time;
  gettime_internal_mock(&current_time);
  struct timespec elapsed_time = time_diff_mock(startup_time, current_time);
  return (elapsed_time.tv_sec * 1000UL) + (elapsed_time.tv_nsec / 1E6);
}
