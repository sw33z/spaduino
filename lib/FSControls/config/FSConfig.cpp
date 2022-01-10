#include "FSConfig.h"
#include <Arduino.h>

namespace spaduino
{

constexpr int MAX_CONFIGS = 8;

struct FSConfigManagerImpl
{
    FSConfigManagerImpl(int pinConfig1, int pinConfig2, int pinConfig3)
        : pinConfig1_(pinConfig1)
        , pinConfig2_(pinConfig2)
        , pinConfig3_(pinConfig3) // LCOV_EXCL_LINE
    {
        for (int i = 0; i < MAX_CONFIGS; i++)
        {
            configs_[i] = nullptr;
        }

        pinMode(pinConfig1_, INPUT_PULLUP);
        pinMode(pinConfig2_, INPUT_PULLUP);
        pinMode(pinConfig3_, INPUT_PULLUP);
    }

    ~FSConfigManagerImpl() = default;

    void addConfigImplementation(FSConfigManager::FSConfigID id, std::shared_ptr<FSConfig> config)
    {
        if (static_cast<int>(id) < MAX_CONFIGS)
        {
            configs_[static_cast<int>(id)] = config;
        }
    }

    FSConfigManager::FSConfigID readConfig() const
    {
        int state1 = !digitalRead(pinConfig1_);
        int state2 = !digitalRead(pinConfig2_);
        int state3 = !digitalRead(pinConfig3_);
        int state = (state1 << 2) | (state2 << 1) | state3;
        
        return static_cast<FSConfigManager::FSConfigID>(state);
    }

    void setup(std::shared_ptr<FSControls> fsControls, CmdMessenger & comm)
    {
        int intConfig = static_cast<int>(readConfig());
        if (configs_[intConfig] != nullptr)
        {
            configs_[intConfig]->setup(fsControls, comm);
        }
    }

    int pinConfig1_;
    int pinConfig2_;
    int pinConfig3_;
    std::shared_ptr<FSConfig> configs_[MAX_CONFIGS];
};

FSConfigManager::FSConfigManager(int configPin1, int configPin2, int configPin3)
    : pimpl_(new FSConfigManagerImpl(configPin1, configPin2, configPin3))
{
}

FSConfigManager::~FSConfigManager()
{
}

void FSConfigManager::addConfigImplementation(FSConfigID id, std::shared_ptr<FSConfig> config)
{
    pimpl_->addConfigImplementation(id, config);
}

void FSConfigManager::setup(std::shared_ptr<FSControls> fsControls, CmdMessenger & comm)
{
    pimpl_->setup(fsControls, comm);
}

FSConfigManager::FSConfigID FSConfigManager::getConfig() const
{
    return pimpl_->readConfig();
}

} // namepsace spaduino