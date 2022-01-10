#pragma once

struct TimerMock
{
   TimerMock() = default;

   void setMillis(unsigned long time);
   unsigned long time = 0;
   bool sendMockValue = false;

   static TimerMock *instance_;
   static TimerMock *getInstance();
};
