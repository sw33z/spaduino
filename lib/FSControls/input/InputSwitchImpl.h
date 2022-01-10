#pragma once

#include "InputSwitch.h"

namespace spaduino
{
namespace input
{
struct InputSwitchImpl
{
    InputSwitchImpl(InputSwitch * parent, int pin, FSDataID id, std::shared_ptr<FSAction> onAction, std::shared_ptr<FSAction> offAction);

    void init();
    void getInitialState();
    void loopOnce();
    void requestState();

    InputSwitch * parent_ = nullptr;
    int pin_ = 0;
    FSDataID id_ = FSDataID::NONE;
    const __FlashStringHelper * eventOff_;
    bool sendValue_ = false;
    std::shared_ptr<FSAction> onAction_ = nullptr;
    std::shared_ptr<FSAction> offAction_ = nullptr;

    bool prevState_ = true; // because it is a pull up pin
    unsigned long prevChange_ = 0;
    int32_t refValue_ = -1;
};
}
}