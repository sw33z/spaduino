#pragma once

#include "Input.h"
#include "unique_ptr.h"

namespace spaduino
{

class FSAction;

namespace input
{

struct InputSwitchImpl;

class InputSwitch : public Input
{
public: 
    InputSwitch(int pin, FSDataID id, std::shared_ptr<FSAction> onAction,  std::shared_ptr<FSAction> offAction);
    
    virtual void loopOnce() override;
    virtual void requestState() override;
private:
    arduino::memory::unique_ptr<InputSwitchImpl> pimpl_;

    friend struct InputSwitchImpl;
};
} // namespace input
} // namespace spaduino

#include "InputSwitchImpl.h"