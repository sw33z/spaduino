#include "TimerMock.h"

TimerMock* TimerMock::instance_ = nullptr;
TimerMock *TimerMock::getInstance()
{
    if (!instance_)
    {
        instance_ = new TimerMock();
    }
    return instance_;
} 

void TimerMock::setMillis(unsigned long time)
{
    sendMockValue = true;
    this->time = time;
}