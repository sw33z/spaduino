#include "InputRotaryEncoderLongPressButton.h"

#include <Arduino.h>

#include <CmdMessenger.h>

namespace spaduino
{

namespace input
{

constexpr int CLICK_DEBOUNCE = 200;
constexpr int LONG_PRESS = 1000;

struct InputRotaryEncoderLongPressButtonImpl
{
    InputRotaryEncoderLongPressButtonImpl(InputRotaryEncoderLongPressButton * parent, int pin, 
                                          std::shared_ptr<FSAction> eventClick, std::shared_ptr<FSAction> eventLongPress)
        : parent_(parent)
        , pin_(pin)
        , pressID_(eventClick)
        , longPressID_(eventLongPress)
    {
    }

    void handlePress(long now)
    {
        if (isPressed_ == false)
        {
            isPressed_ = true;
            lastTimePressed_ = now;
        }
        else
        {
            if ( ((now - lastTimePressed_) > LONG_PRESS ) && !sentPress_)
            {
                sendLongPress();
            }
        } 
    }

    void handleRelease(long now)
    {
        if ((now - lastTimePressed_) < LONG_PRESS)
        {
            sendNormalPress();
        }
        isPressed_ = false;
        sentPress_ = false;
    }

    void sendLongPress()
    {
        parent_->runAction(longPressID_);
        sentPress_ = true;
    }

    void sendNormalPress()
    {
        parent_->runAction(pressID_);
    }

    void handleClick()
    {
        unsigned long now = millis();
        uint8_t state = digitalRead(pin_);

        if ((state != stateBefore_) && (now - lastStateChangeTime_) > CLICK_DEBOUNCE)
        {
            if (state == LOW)
            {
                handlePress(now);
            }
            else
            {
                handleRelease(now);
            }
            stateBefore_ = state;
            lastStateChangeTime_ = now;
        }
        else if ( (state == stateBefore_) && (state == LOW))
        {
            handlePress(now);
        }
    }

    InputRotaryEncoderLongPressButton * parent_;
    int pin_ = 0;
    std::shared_ptr<FSAction> pressID_ = nullptr;
    std::shared_ptr<FSAction> longPressID_ = nullptr;

    uint8_t stateBefore_ = HIGH; // pullup
    bool isPressed_ = false;
    long lastStateChangeTime_ = 0;
    long lastTimePressed_ = 0;
    bool sentPress_ = false;

};

InputRotaryEncoderLongPressButton::InputRotaryEncoderLongPressButton(int pinA, 
                                                                    int pinB, 
                                                                    int pinC, 
                                                                    FSDataID id, 
                                                                    std::shared_ptr<FSAction> onClockwise,
                                                                    std::shared_ptr<FSAction> onClockWiseAccelerated,
                                                                    std::shared_ptr<FSAction> onCounterClockWise, 
                                                                    std::shared_ptr<FSAction> onCounterClockWiseAccelerated,
                                                                    std::shared_ptr<FSAction> eventClick,
                                                                    std::shared_ptr<FSAction> eventLongPress,
                                                                    bool accelerated) 
    : InputRotaryEncoder(pinA, pinB, pinC, id, onClockwise, onClockWiseAccelerated, 
                        onCounterClockWise, onCounterClockWiseAccelerated, eventClick, accelerated)
    , pimpl_(new InputRotaryEncoderLongPressButtonImpl(this, pinC, eventClick, eventLongPress))
{
}

InputRotaryEncoderLongPressButton::~InputRotaryEncoderLongPressButton()
{
}

void InputRotaryEncoderLongPressButton::handleClick()
{
    pimpl_->handleClick();
}


} // namespace input

} // namespace spaduino
