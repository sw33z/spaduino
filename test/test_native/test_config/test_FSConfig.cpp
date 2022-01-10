#include <gtest/gtest.h>
#include "CmdMessengerTestBase.h"

#include <config/FSConfig.h>
#include <input/InputSwitch.h>
#include <FSAction.h>
#include <FSControls.h>
#include <TimerMock.h>

#include <Arduino.h>

#include <shared_ptr.h>

using namespace spaduino;

namespace test_config
{
    constexpr int PIN_SWITCH = 1;
    constexpr int CONFIG_PIN_1 = 2;
    constexpr int CONFIG_PIN_2 = 3;
    constexpr int CONFIG_PIN_3 = 4;

    class FSConfig1 : public FSConfig
    {
        public:
            FSConfig1() = default;
            ~FSConfig1() = default;
            void setup(std::shared_ptr<FSControls> controls, CmdMessenger & comm) override
            {

                std::shared_ptr<input::InputSwitch> testSwitch(new input::InputSwitch(
                                            PIN_SWITCH, 
                                            FSDataID::ID_13,
                                            std::make_shared<FSActionSendEvent>(comm, F("SIMCONNECT: EVENT ALTITUDE ON")),
                                            nullptr));
                testSwitch->sendDataAllowed(true);
                controls->addInputObject(testSwitch);
            }
    };

    class FSConfig2 : public FSConfig
    {
        public:
            FSConfig2() = default;
            ~FSConfig2() = default;
            void setup(std::shared_ptr<FSControls> controls, CmdMessenger & comm) override
            {
                std::shared_ptr<input::InputSwitch> testSwitch(new input::InputSwitch(
                                            PIN_SWITCH, 
                                            FSDataID::ID_14,
                                            std::make_shared<FSActionSendEvent>(comm, F("SIMCONNECT: EVENT VS ON")),
                                            nullptr));
                testSwitch->sendDataAllowed(true);
                controls->addInputObject(testSwitch);
            }
    };
    class FSConfig3 : public FSConfig
    {
        public:
            FSConfig3() = default;
            ~FSConfig3() = default;
            void setup(std::shared_ptr<FSControls> controls, CmdMessenger & comm)
            {
                std::shared_ptr<input::InputSwitch> testSwitch(new input::InputSwitch(
                                            PIN_SWITCH, 
                                            FSDataID::ID_15,
                                            std::make_shared<FSActionSendEvent>(comm, F("SIMCONNECT: EVENT HEADING ON")),
                                            nullptr));
                testSwitch->sendDataAllowed(true);
                controls->addInputObject(testSwitch);
            }
    };
    class FSConfig4 : public FSConfig
    {
        public:
            FSConfig4() = default;
            ~FSConfig4() = default;
            void setup(std::shared_ptr<FSControls> controls, CmdMessenger & comm)
            {
                std::shared_ptr<input::InputSwitch> testSwitch(new input::InputSwitch(
                                            PIN_SWITCH, 
                                            FSDataID::ID_16,
                                            std::make_shared<FSActionSendEvent>(comm, F("SIMCONNECT: EVENT APPROACH ON")),
                                            nullptr));
                testSwitch->sendDataAllowed(true);
                controls->addInputObject(testSwitch);
            }
    };

    class FSConfigTest: public test::CmdMessengerTestBase
    {
        public:
            FSConfigTest()
                : test::CmdMessengerTestBase()
            {
            }
            ~FSConfigTest() = default;
    };

    TEST_F(FSConfigTest, test_config_1)
    {
        std::shared_ptr<FSControls>fscontrols(new FSControls(F("FSCONTROLS"), *(comm_.get())));

        FSConfigManager configManager(CONFIG_PIN_1, CONFIG_PIN_2, CONFIG_PIN_3);
        std::shared_ptr<FSConfig1>config1(new FSConfig1());
        std::shared_ptr<FSConfig2>config2(new FSConfig2());
        
        configManager.addConfigImplementation(FSConfigManager::FSConfigID::CONFIG_0, config1);
        configManager.addConfigImplementation(FSConfigManager::FSConfigID::CONFIG_1, config2);

        unsigned long millis_now = 0;
        digitalWrite(CONFIG_PIN_1, HIGH);
        digitalWrite(CONFIG_PIN_2, HIGH);
        digitalWrite(CONFIG_PIN_3, HIGH);
        digitalWrite(PIN_SWITCH, HIGH);
        TimerMock::getInstance()->setMillis(millis_now);

        configManager.setup(fscontrols, *(comm_.get()));

        fscontrols->loopOnce();
        EXPECT_EQ("", Serial.getDataStr());

        EXPECT_EQ(FSConfigManager::FSConfigID::CONFIG_0, configManager.getConfig());

        millis_now += 1000;
        digitalWrite(PIN_SWITCH, LOW);
        TimerMock::getInstance()->setMillis(millis_now);

        fscontrols->loopOnce();
        EXPECT_EQ("4,SIMCONNECT: EVENT ALTITUDE ON;", Serial.getDataStr());
    }

    TEST_F(FSConfigTest, test_config_3)
    {
        std::shared_ptr<FSControls>fscontrols(new FSControls(F("FSCONTROLS"), *(comm_.get())));
        FSConfigManager configManager(CONFIG_PIN_1, CONFIG_PIN_2, CONFIG_PIN_3);
        std::shared_ptr<FSConfig1>config1(new FSConfig1());
        std::shared_ptr<FSConfig2>config2(new FSConfig2());
        std::shared_ptr<FSConfig3>config3(new FSConfig3());
        
        configManager.addConfigImplementation(FSConfigManager::FSConfigID::CONFIG_0, config1);
        configManager.addConfigImplementation(FSConfigManager::FSConfigID::CONFIG_1, config2);
        configManager.addConfigImplementation(FSConfigManager::FSConfigID::CONFIG_2, config3);

        unsigned long millis_now = 0;
        digitalWrite(CONFIG_PIN_1, HIGH);
        digitalWrite(CONFIG_PIN_2, LOW);
        digitalWrite(CONFIG_PIN_3, HIGH);
        digitalWrite(PIN_SWITCH, HIGH);
        TimerMock::getInstance()->setMillis(millis_now);

        configManager.setup(fscontrols, *(comm_.get()));
        EXPECT_EQ(FSConfigManager::FSConfigID::CONFIG_2, configManager.getConfig());

        fscontrols->loopOnce();
        EXPECT_EQ("", Serial.getDataStr());

        millis_now += 1000;
        digitalWrite(PIN_SWITCH, LOW);
        TimerMock::getInstance()->setMillis(millis_now);

        fscontrols->loopOnce();
        EXPECT_EQ("4,SIMCONNECT: EVENT HEADING ON;", Serial.getDataStr());
    }

    TEST_F(FSConfigTest, test_config_2)
    {
        std::shared_ptr<FSControls>fscontrols(new FSControls(F("FSCONTROLS"), *(comm_.get())));
        FSConfigManager configManager(CONFIG_PIN_1, CONFIG_PIN_2, CONFIG_PIN_3);
        std::shared_ptr<FSConfig1>config1(new FSConfig1());
        std::shared_ptr<FSConfig2>config2(new FSConfig2());
        
        configManager.addConfigImplementation(FSConfigManager::FSConfigID::CONFIG_0, config1);
        configManager.addConfigImplementation(FSConfigManager::FSConfigID::CONFIG_1, config2);

        unsigned long millis_now = 0;
        digitalWrite(CONFIG_PIN_1, HIGH);
        digitalWrite(CONFIG_PIN_2, HIGH);
        digitalWrite(CONFIG_PIN_3, LOW);
        digitalWrite(PIN_SWITCH, HIGH);
        TimerMock::getInstance()->setMillis(millis_now);

        configManager.setup(fscontrols, *(comm_.get()));

        fscontrols->loopOnce();
        EXPECT_EQ("", Serial.getDataStr());

        millis_now += 1000;
        digitalWrite(PIN_SWITCH, LOW);
        TimerMock::getInstance()->setMillis(millis_now);

        fscontrols->loopOnce();
        EXPECT_EQ("4,SIMCONNECT: EVENT VS ON;", Serial.getDataStr());
    }

    TEST_F(FSConfigTest, test_config_4)
    {
        std::shared_ptr<FSControls>fscontrols(new FSControls(F("FSCONTROLS"), *(comm_.get())));
        FSConfigManager configManager(CONFIG_PIN_1, CONFIG_PIN_2, CONFIG_PIN_3);
        std::shared_ptr<FSConfig1>config1(new FSConfig1());
        std::shared_ptr<FSConfig2>config2(new FSConfig2());
        std::shared_ptr<FSConfig3>config3(new FSConfig3());
        std::shared_ptr<FSConfig4>config4(new FSConfig4());
        
        configManager.addConfigImplementation(FSConfigManager::FSConfigID::CONFIG_0, config1);
        configManager.addConfigImplementation(FSConfigManager::FSConfigID::CONFIG_1, config2);
        configManager.addConfigImplementation(FSConfigManager::FSConfigID::CONFIG_2, config3);
        configManager.addConfigImplementation(FSConfigManager::FSConfigID::CONFIG_3, config4);

        unsigned long millis_now = 0;
        digitalWrite(CONFIG_PIN_1, HIGH);
        digitalWrite(CONFIG_PIN_2, LOW);
        digitalWrite(CONFIG_PIN_3, LOW);
        digitalWrite(PIN_SWITCH, HIGH);
        TimerMock::getInstance()->setMillis(millis_now);

        configManager.setup(fscontrols, *(comm_.get()));

        fscontrols->loopOnce();
        EXPECT_EQ("", Serial.getDataStr());

        millis_now += 1000;
        digitalWrite(PIN_SWITCH, LOW);
        TimerMock::getInstance()->setMillis(millis_now);

        fscontrols->loopOnce();
        EXPECT_EQ("4,SIMCONNECT: EVENT APPROACH ON;", Serial.getDataStr());
    }
}
