#pragma once

#include "FSValueDisplay.h"
#include "unique_ptr.h"

namespace spaduino
{
namespace display
{

struct FSValueLedDisplayImpl;

class FSValueLedDisplay : public FSValueDisplay
{
public:
    FSValueLedDisplay(FSDataID id, int pin, bool initialON = false);
    virtual ~FSValueLedDisplay();

protected:
    virtual void display(int32_t value) override;

private:
    arduino::memory::unique_ptr<FSValueLedDisplayImpl> pimpl_;
};

} // namespace display
} // namespace spaduino