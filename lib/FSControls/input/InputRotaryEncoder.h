#pragma once

#include "Input.h"
#include "unique_ptr.h"

namespace spaduino
{

class FSAction;

namespace input
{

struct InputRotaryEncoderImpl;

class InputRotaryEncoder : public Input
{
public:
    InputRotaryEncoder(int pinA, 
                       int pinB, 
                       int pinC, 
                       FSDataID id, 
                       std::shared_ptr<FSAction> onClockwise,
                       std::shared_ptr<FSAction> onClockWiseAccelerated,
                       std::shared_ptr<FSAction> onCounterClockWise, 
                       std::shared_ptr<FSAction> onCounterClockWiseAccelerated,
                       std::shared_ptr<FSAction> eventClick,
                       bool accelerated=true );

    virtual ~InputRotaryEncoder();

    void loopOnce() override;
    void requestState() override;

protected:
    virtual void handleEncoderRotation();
    virtual void handleClick();
    
private:
    arduino::memory::unique_ptr<InputRotaryEncoderImpl> pimpl_;
    friend struct InputRotaryEncoderImpl;
};

} // namespace input
} // namespace spaduino