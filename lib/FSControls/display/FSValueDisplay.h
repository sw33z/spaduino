#pragma once

#include "FSObject.h"

namespace spaduino
{
namespace display
{

class FSValueDisplay : public spaduino::FSObject
{
public:
    enum class eDisplayRound
    {
        e1,
        e10,
        e100,
        e1000
    };

    FSValueDisplay(FSDataID id, eDisplayRound round = eDisplayRound::e1);
    virtual ~FSValueDisplay();

    virtual void loopOnce() override;
    
protected:
    virtual void display(int32_t value) = 0;
    void setInitialValue(int32_t value);

    eDisplayRound round_;
    int32_t value_ = 0;
};

} // namespace display
} // namespace spaduino