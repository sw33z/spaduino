#pragma once

#include "InputRotaryEncoder.h"
#include "unique_ptr.h"

namespace spaduino
{
namespace input
{

struct InputRotaryEncoderLongPressButtonImpl;

class InputRotaryEncoderLongPressButton : public InputRotaryEncoder
{
public:
    InputRotaryEncoderLongPressButton(int pinA, 
                                        int pinB, 
                                        int pinC, 
                                        FSDataID id, 
                                        std::shared_ptr<FSAction> onClockwise,
                                        std::shared_ptr<FSAction> onClockWiseAccelerated,
                                        std::shared_ptr<FSAction> onCounterClockWise, 
                                        std::shared_ptr<FSAction> onCounterClockWiseAccelerated,
                                        std::shared_ptr<FSAction> eventClick,
                                        std::shared_ptr<FSAction> eventLongPress,
                                        bool accelerated=true );
    virtual ~InputRotaryEncoderLongPressButton();

protected:
    virtual void handleClick() override;
    
private:
    arduino::memory::unique_ptr<InputRotaryEncoderLongPressButtonImpl> pimpl_;
    friend struct InputRotaryEncoderLongPressButtonImpl;
};

} // namespace input
} // namespace spaduino
