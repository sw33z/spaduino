#pragma once

#include "FSValueDisplay.h"
#include "unique_ptr.h"

namespace spaduino
{
namespace display
{

struct FSValueTM1637DisplayImpl;

class FSValueTM1637Display : public FSValueDisplay
{
public:
    enum class NumDigits
    {
        FOUR,
        SIX
    };
    FSValueTM1637Display(FSDataID id, eDisplayRound round, int pin1, int pin2, bool padding, NumDigits numDigits = NumDigits::SIX);
    virtual ~FSValueTM1637Display();

    void setShowMinus1AsDashes(bool value);

protected:
    virtual void display(int32_t value) override;

private:
    arduino::memory::unique_ptr<FSValueTM1637DisplayImpl> pimpl_;
};

// helper classes
class FSValueTM1637Display4Digits : public FSValueTM1637Display
{
public: 
    FSValueTM1637Display4Digits(FSDataID id, eDisplayRound round, int pin1, int pin2, bool padding);
    virtual ~FSValueTM1637Display4Digits();
};

class FSValueTM1637Display6Digits : public FSValueTM1637Display
{
public: 
    FSValueTM1637Display6Digits(FSDataID id, eDisplayRound round, int pin1, int pin2, bool padding);
    virtual ~FSValueTM1637Display6Digits();
};

} // namespace display
} // namespace spaduino