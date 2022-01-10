#include "InputRotaryEncoder.h"
#include "SpadNextCmd.h"

#include <CmdMessenger.h>

#include <Arduino.h>

namespace spaduino
{
namespace input
{

constexpr int CLICK_DEBOUNCE = 200;

constexpr int ENCODER_DEBOUNCE = 5;
constexpr int SHORT_TIME = 25;

constexpr uint8_t LATCH_STATE = 3; // 3 is the initial state

constexpr int8_t KNOBDIR[] = {
  0, -1,  1,  0,
  1,  0,  0, -1,
  -1,  0,  0,  1,
   0,  1, -1,  0  };

struct InputRotaryEncoderImpl
{
    InputRotaryEncoderImpl(InputRotaryEncoder *parent, 
                            int pinA, 
                            int pinB, 
                            int pinC, 
                            FSDataID rotaryID, 
                            bool accelerated,
                            std::shared_ptr<FSAction> onClockwise,
                            std::shared_ptr<FSAction> onClockWiseAccelerated,
                            std::shared_ptr<FSAction> onCounterClockWise, 
                            std::shared_ptr<FSAction> onCounterClockWiseAccelerated,
                            std::shared_ptr<FSAction> eventClick);
    ~InputRotaryEncoderImpl();

    void init();
    void loopOnce();

    void handleEncoderRotation();
    void handleClick();

    void sendIncrement(int32_t clicks);
    void sendButtonClick();

    InputRotaryEncoder * parent_ = nullptr;
    int pinA_ = 0;
    int pinB_ = 0;
    int pinC_ = 0;
    FSDataID rotaryID_ = FSDataID::NONE;
    bool accelerated_ = true;
    std::shared_ptr<FSAction> onClockwise_ = nullptr;
    std::shared_ptr<FSAction> onClockWiseAccelerated_ = nullptr;
    std::shared_ptr<FSAction> onCounterClockWise_ = nullptr;
    std::shared_ptr<FSAction> onCounterClockWiseAccelerated_ = nullptr;
    std::shared_ptr<FSAction> eventClick_ = nullptr;

    uint8_t oldState_ = LATCH_STATE; 
    long position_ = 0;        // Internal position (4 times _positionExt)
    long positionExt_ = 0;     // External position
    long positionExtPrev_ = 0;  // External position (used only for direction checking)
    long position_div_4_ = 0;

    unsigned long positionExtTimePrev_ = 0;

    unsigned long lastClick_ = 0;
    uint8_t lastClickState_ = HIGH;
};

InputRotaryEncoderImpl::InputRotaryEncoderImpl(InputRotaryEncoder * parent, 
                                                int pinA, 
                                                int pinB, 
                                                int pinC, 
                                                FSDataID rotaryID, 
                                                bool accelerated,
                                                std::shared_ptr<FSAction> onClockwise,
                                                std::shared_ptr<FSAction> onClockWiseAccelerated,
                                                std::shared_ptr<FSAction> onCounterClockWise, 
                                                std::shared_ptr<FSAction> onCounterClockWiseAccelerated,
                                                std::shared_ptr<FSAction> eventClick)
    : parent_(parent)
    , pinA_(pinA)
    , pinB_(pinB)
    , pinC_(pinC)
    , rotaryID_(rotaryID)
    , accelerated_(accelerated)
    , onClockwise_(onClockwise)
    , onClockWiseAccelerated_(onClockWiseAccelerated)
    , onCounterClockWise_(onCounterClockWise)
    , onCounterClockWiseAccelerated_(onCounterClockWiseAccelerated)
    , eventClick_(eventClick)
{
    init();
}

void InputRotaryEncoderImpl::init()
{
    pinMode(pinA_, INPUT_PULLUP);
    pinMode(pinB_, INPUT_PULLUP);
    pinMode(pinC_, INPUT_PULLUP);
}

InputRotaryEncoderImpl::~InputRotaryEncoderImpl()
{
}

void InputRotaryEncoderImpl::loopOnce()
{
    parent_->handleEncoderRotation();
    parent_->handleClick();  
}

void InputRotaryEncoderImpl::handleEncoderRotation()
{
    uint8_t sig1 = digitalRead(pinA_);
    uint8_t sig2 = digitalRead(pinB_);
    uint8_t thisState = sig1 | (sig2 << 1);

    if (oldState_ != thisState) 
    {
        long positionPrev = position_;
        position_ += KNOBDIR[thisState | (oldState_<<2)];

        if (thisState == LATCH_STATE && ((position_<<2) != position_div_4_)) 
        {
            position_div_4_ = position_<<2;
            unsigned long now = millis();
            if (now - positionExtTimePrev_ > ENCODER_DEBOUNCE)
            {
                int32_t increment = 1;
                if ((now - positionExtTimePrev_ < SHORT_TIME) && (accelerated_))
                {
                    increment = 10;
                }
                if (position_ < positionPrev)
                {  
                    increment = increment * -1;
                }
                positionExt_ += increment;
                positionExtTimePrev_ = now;
                positionExtPrev_ = positionExt_;

                sendIncrement(increment);
            }
        }
        oldState_ = thisState;
    } 
}

void InputRotaryEncoderImpl::handleClick()
{
    unsigned long now = millis();
    uint8_t state = digitalRead(pinC_);
    //std::cout << "CLICK STATE: " << (int)state << std::endl;
    if ((state != lastClickState_)&& (now - lastClick_ > CLICK_DEBOUNCE))
    {
        if (state == LOW)
        {
            sendButtonClick();
            lastClick_ = now;
        }
        lastClickState_ = state;
    }
}

void InputRotaryEncoderImpl::sendIncrement(int32_t clicks)
{
    if (clicks > 0)
    {
        if (clicks == 1)
        {
            parent_->runAction(onClockwise_);
        }
        else
        {
            parent_->runAction(onClockWiseAccelerated_);
        }
    }
    else
    {
        if (clicks == -1)
        {
            parent_->runAction(onCounterClockWise_);
        }
        else
        {
            parent_->runAction(onCounterClockWiseAccelerated_);
        }
    }
}

void InputRotaryEncoderImpl::sendButtonClick()
{
    parent_->runAction(eventClick_);
}

// ---------------------------------------------------------------------------------------

InputRotaryEncoder::InputRotaryEncoder(int pinA, 
                       int pinB, 
                       int pinC, 
                       FSDataID id, 
                       std::shared_ptr<FSAction> onClockwise,
                       std::shared_ptr<FSAction> onClockWiseAccelerated,
                       std::shared_ptr<FSAction> onCounterClockWise, 
                       std::shared_ptr<FSAction> onCounterClockWiseAccelerated,
                       std::shared_ptr<FSAction> eventClick,
                       bool accelerated )
    : Input(id)
    , pimpl_(new InputRotaryEncoderImpl(this, 
                                        pinA, 
                                        pinB, 
                                        pinC, 
                                        id, 
                                        accelerated, 
                                        onClockwise,
                                        onClockWiseAccelerated,
                                        onCounterClockWise, 
                                        onCounterClockWiseAccelerated,
                                        eventClick))
{
}

InputRotaryEncoder::~InputRotaryEncoder()
{
}

void InputRotaryEncoder::loopOnce()
{
    pimpl_->loopOnce();
}

void InputRotaryEncoder::requestState()
{
}

void InputRotaryEncoder::handleEncoderRotation()
{
    pimpl_->handleEncoderRotation();
}

void InputRotaryEncoder::handleClick()
{
    pimpl_->handleClick();
}

} // namespace input
} // namespace spaduino