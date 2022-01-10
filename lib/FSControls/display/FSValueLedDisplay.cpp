#include "FSValueLedDisplay.h"

#include <Arduino.h>
#include <CmdMessenger.h>
#include <SpadNextCmd.h>

namespace spaduino
{
namespace display
{

struct FSValueLedDisplayImpl
{
    FSValueLedDisplayImpl(FSDataID id, int pin, bool initialON);
    ~FSValueLedDisplayImpl();

    void display(int32_t value);

    FSDataID id_ = FSDataID::NONE;
    int pin_ = 0;
    int32_t initialValue_ = 0;
};

FSValueLedDisplayImpl::FSValueLedDisplayImpl(FSDataID id, int pin, bool initialON)
    : id_(id)
    , pin_(pin)
{
    pinMode(pin_, OUTPUT);
    if (initialON)
    {
        digitalWrite(pin_, HIGH);
        initialValue_ = 1;
    }
    else
    {
        digitalWrite(pin_, LOW);
        initialValue_ = 0;
    }
}

FSValueLedDisplayImpl::~FSValueLedDisplayImpl()
{
}

void FSValueLedDisplayImpl::display(int32_t value)
{
    if (value == 1)
    {
        digitalWrite(pin_, HIGH);
    }
    else
    {
        digitalWrite(pin_, LOW);
    }
}

FSValueLedDisplay::FSValueLedDisplay(FSDataID id, 
                                     int pin, 
                                     bool initialON)
    : FSValueDisplay(id)
    , pimpl_(new FSValueLedDisplayImpl(id, pin, initialON))
{
    setInitialValue(pimpl_->initialValue_);
}

FSValueLedDisplay::~FSValueLedDisplay()
{
}

void FSValueLedDisplay::display(int32_t value)
{
    pimpl_->display(value);
}

} // namspace display
} // namespace spaduino
