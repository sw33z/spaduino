#pragma once

#include "unique_ptr.h"
#include "shared_ptr.h"

class CmdMessenger;

namespace spaduino
{
class FSControls;

struct FSConfigManagerImpl;

class FSConfig
{
public:
    FSConfig() = default;
    virtual ~FSConfig() = default;

    virtual void setup(std::shared_ptr<FSControls> fsControls, CmdMessenger & comm) = 0; 
};

class FSConfigManager
{
public:
    enum class FSConfigID
    {   
        CONFIG_0       = 0x00,
        CONFIG_1       = 0x01,
        CONFIG_2       = 0x02,
        CONFIG_3       = 0x03,
        CONFIG_4       = 0x04,
        CONFIG_5       = 0x05,
        CONFIG_6       = 0x06,
        CONFIG_7       = 0x07,
        CONFIG_8       = 0x08,
        CONFIG_9       = 0x09,
    };
    FSConfigManager(int configPin1, int configPin2, int configPin3);
    ~FSConfigManager();

    void addConfigImplementation(FSConfigID id, std::shared_ptr<FSConfig> config);

    void setup(std::shared_ptr<FSControls> fsControls, CmdMessenger & comm);

    FSConfigID getConfig() const;

private:
    arduino::memory::unique_ptr<FSConfigManagerImpl> pimpl_;
};

} // namespace spaduino

