#include "InputSwitch.h"
#include "SpadNextCmd.h"
#include "FSState.h"

#include <Arduino.h>

#include <CmdMessenger.h>

namespace spaduino
{
namespace input
{

constexpr int REBOUNCE = 10;

InputSwitchImpl::InputSwitchImpl(InputSwitch * parent, int pin, FSDataID id, std::shared_ptr<FSAction> onAction, std::shared_ptr<FSAction> offAction)
    : parent_(parent)
    , pin_(pin)
    , id_(id)
    , eventOff_(nullptr)
    , sendValue_(false)
    , onAction_(onAction)
    , offAction_(offAction)
{
    init();
    getInitialState();
}

void InputSwitchImpl::init()
{
    pinMode(pin_, INPUT_PULLUP);
}

void InputSwitchImpl::getInitialState()
{
    prevState_ = digitalRead(pin_);
}

void InputSwitchImpl::loopOnce()
{
    bool state = digitalRead(pin_);
    auto now = millis();
    if (state != prevState_ && (now - prevChange_ > REBOUNCE))
    {
        prevState_ = state;
        prevChange_ = now;
        if (state == LOW)
        {
            parent_->runAction(onAction_);
        }
        else
        {
            parent_->runAction(offAction_);
        }
    }
}

void InputSwitchImpl::requestState()
{
    bool state = digitalRead(pin_);
    if (state == LOW)
    {
        parent_->runAction(onAction_);
    }
    else
    {
        parent_->runAction(offAction_);
    }
}

// ------------------------------------------------------------------------------------

InputSwitch::InputSwitch(int pin,
                         FSDataID id, 
                         std::shared_ptr<FSAction> onAction, 
                         std::shared_ptr<FSAction> offAction)
     : Input(id)
     , pimpl_( new InputSwitchImpl(this, pin, id, onAction, offAction))
{
}

void InputSwitch::loopOnce()
{
    pimpl_->loopOnce();
}

void InputSwitch::requestState()
{
    pimpl_->requestState();
}

} // namespace input
} // namspace spaduino