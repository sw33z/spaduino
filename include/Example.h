#pragma once

#include <config/FSConfig.h>

namespace spaduino
{

class Example : public FSConfig
{
public:
    Example() = default;
    ~Example() = default;

    void setup(std::shared_ptr<FSControls> fsControls, CmdMessenger & comm) override;
};

} // namespace spaduino
