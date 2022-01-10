#include "FSValueTM1637Display.h"

#include <Arduino.h>
#include <TM1637.h>
#include <shared_ptr.h>

namespace spaduino
{
namespace display
{

struct FSValueTM1637DisplayImpl
{
    FSValueTM1637DisplayImpl(FSDataID id, FSValueDisplay::eDisplayRound round, 
                             int pin1, int pin2, bool padding, 
                             FSValueTM1637Display::NumDigits numDigits = FSValueTM1637Display::NumDigits::SIX);
    ~FSValueTM1637DisplayImpl();

    void setShowMinus1AsDashes(bool value);
    void display(int32_t value);
    static TM1637::NumDigits convertNumDigits(FSValueTM1637Display::NumDigits digits)
    {
        TM1637::NumDigits res;
        switch (digits)
        {
        case FSValueTM1637Display::NumDigits::FOUR:
            res = TM1637::NumDigits::FOUR;
            break;
        case FSValueTM1637Display::NumDigits::SIX:
            res = TM1637::NumDigits::SIX;
            break;
        }
        return res;
    }

    //oid avionicsPower(bool state);

    FSDataID id_;
    FSValueDisplay::eDisplayRound round_;
    int pin1_ = 0;
    int pin2_ = 0;
    bool padding_ = false;
    FSValueTM1637Display::NumDigits numDigits_;
    // /int maxChars_ = -1;
    int32_t lastValue_ = 0;
    bool avionics_power_ = true;
    arduino::memory::unique_ptr<TM1637> display_;

    bool showMinus1AsDashes_ = false;

    const char dashes_4[5] = { '-', '-', '-', '-', '\0' };
    const char dashes_6[7] = { '-', '-', '-', '-', '-', '-', '\0' };
};

FSValueTM1637DisplayImpl::FSValueTM1637DisplayImpl(FSDataID id, FSValueDisplay::eDisplayRound round, 
                                                   int pin1, int pin2, bool padding, 
                                                   FSValueTM1637Display::NumDigits numDigits)
    : id_(id)
    , round_(round)
    , pin1_(pin1)
    , pin2_(pin2)
    , padding_(padding)
    , numDigits_(numDigits)
#ifdef UNIT_TEST
    , display_(TM1637::getInstance())
#else
    , display_(new TM1637(pin1, pin2, FSValueTM1637DisplayImpl::convertNumDigits(numDigits)))
#endif
{
    display_->setBrightness(TM1637::Brightness::BRIGHT_7);
}

FSValueTM1637DisplayImpl::~FSValueTM1637DisplayImpl()
{
#ifdef UNIT_TEST
    TM1637::resetInstance();
#endif
}

void FSValueTM1637DisplayImpl::setShowMinus1AsDashes(bool value)
{
    showMinus1AsDashes_ = value;
}

void FSValueTM1637DisplayImpl::display(int32_t displayValue)
{
    switch(round_)
    {
        case FSValueDisplay::eDisplayRound::e1:
        break;
        case FSValueDisplay::eDisplayRound::e10:
            displayValue = displayValue - (displayValue % 10);
        break;
        case FSValueDisplay::eDisplayRound::e100:
            displayValue = displayValue - (displayValue % 100);
        break;
        case FSValueDisplay::eDisplayRound::e1000:
            displayValue = displayValue - (displayValue % 1000);
        break;
    };
    //if (avionics_power_)
    //{
    if (displayValue == -1 && showMinus1AsDashes_)
    {
        switch (numDigits_)
        {
            case FSValueTM1637Display::NumDigits::FOUR:
            display_->showString(dashes_4, 4);
            break;
            case FSValueTM1637Display::NumDigits::SIX:
            display_->showString(dashes_6, 6);
            break;
        }
    }
    else
    {
        display_->showNumber(displayValue, padding_);
    }
    //}
    //else
    //{
    //    display_->clearDisplay();
    //}
    lastValue_ = displayValue;
}

/*
void FSValueTM1637DisplayImpl::avionicsPower(bool state)
{
    avionics_power_ = state;
    if (!avionics_power_)
    {
        display_->clearDisplay();
    }
    else
    {
        display_->displayInteger(lastValue_, padding_);
    }
}
*/

// ----------------------------------------------------------------------------------------

FSValueTM1637Display::FSValueTM1637Display(FSDataID id, 
                                           eDisplayRound round, 
                                           int pin1, 
                                           int pin2, 
                                           bool padding, 
                                           NumDigits numDigits)
    : FSValueDisplay(id, round)
    , pimpl_(new FSValueTM1637DisplayImpl(id, round, pin1, pin2, padding, numDigits))
{
}

FSValueTM1637Display::~FSValueTM1637Display()
{
}

void FSValueTM1637Display::setShowMinus1AsDashes(bool value)
{
    pimpl_->setShowMinus1AsDashes(value);
}

void FSValueTM1637Display::display(int32_t value)
{
    pimpl_->display(value);
}

//void FSValueTM1637Display::avionicsPower(bool state)
//{
//    pimpl_->avionicsPower(state);
//}

FSValueTM1637Display4Digits::FSValueTM1637Display4Digits(FSDataID id, eDisplayRound round, int pin1, int pin2, bool padding)
    : FSValueTM1637Display(id, round, pin1, pin2, padding, FSValueTM1637Display::NumDigits::FOUR)
{
}

FSValueTM1637Display4Digits::~FSValueTM1637Display4Digits()
{
}

FSValueTM1637Display6Digits::FSValueTM1637Display6Digits(FSDataID id, eDisplayRound round, int pin1, int pin2, bool padding)
    : FSValueTM1637Display(id, round, pin1, pin2, padding, FSValueTM1637Display::NumDigits::SIX)
{

}

FSValueTM1637Display6Digits::~FSValueTM1637Display6Digits()
{
}

} // namespace display
} // namespace spaduino