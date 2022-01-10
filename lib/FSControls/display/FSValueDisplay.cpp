#include "FSValueDisplay.h"
#include "FSState.h"

#include <CmdMessenger.h>

namespace spaduino
{
namespace display
{

FSValueDisplay::FSValueDisplay(FSDataID id, 
                               eDisplayRound round)
    : spaduino::FSObject(id)
    , round_(round)
{
}

FSValueDisplay::~FSValueDisplay()
{
}

void FSValueDisplay::loopOnce()
{
    if (FSState::getInstance().getValue(getID()) != value_)
    {
        value_ = FSState::getInstance().getValue(getID());
        display(value_);
    }
}

void FSValueDisplay::setInitialValue(int32_t value)
{
    value_ = value;
}

} // namespace display
} // namespace spaduino