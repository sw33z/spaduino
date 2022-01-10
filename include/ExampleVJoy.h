#pragma once

#include <config/FSConfig.h>

namespace spaduino
{

class ExampleVJoy : public FSConfig
{
public:
    ExampleVJoy() = default;
    ~ExampleVJoy() = default;

    void setup(std::shared_ptr<FSControls> fsControls, CmdMessenger & comm) override;
};

} // namespace spaduino
