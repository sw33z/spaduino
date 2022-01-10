#include <gtest/gtest.h>
#include "CmdMessengerTestBase.h"

#include <FSAction.h>
#include <TimerMock.h>

#include <input/InputSwitch.h>
#include <Arduino.h>

namespace spaduino
{
namespace test_input
{
    class InputSwitchTest: public spaduino::test::CmdMessengerTestBase
    {
        public:
            InputSwitchTest()
                : spaduino::test::CmdMessengerTestBase()
            {
            }
            ~InputSwitchTest() = default;
    };

    TEST_F (InputSwitchTest, test_input_switch_on_event )
    {
        int pin = 1;
        unsigned long millis_now = 0;
        // input is pullup, so set the pin to HIGH 
        digitalWrite(pin, HIGH);

        // also initialise millis to 0
        TimerMock::getInstance()->setMillis(millis_now);

        std::unique_ptr<spaduino::input::InputSwitch> input(new spaduino::input::InputSwitch(
                                                    pin, 
                                                    spaduino::FSDataID::ID_11,
                                                    std::make_shared<FSActionSendEvent>(*(comm_.get()), F("SIMCONNECT: EVENT ON")),
                                                    nullptr));

        input->sendDataAllowed(true);
        // loop the input, no cmd should be generated.
        input->loopOnce();
        EXPECT_EQ("", Serial.getDataStr());

        // change the pin to LOW (swith is ON now)
        digitalWrite(pin, LOW);
        input->loopOnce();

        // change the millis... add 40 millis
        millis_now += 40;
        TimerMock::getInstance()->setMillis(millis_now);

        // loop the input, a cmd should be generated
        input->loopOnce();
        EXPECT_EQ("4,SIMCONNECT: EVENT ON;", Serial.getDataStr());

        // change the pin to HIGH (swith is OFF now)
        digitalWrite(pin, HIGH);

        // change the millis... add 40 millis
        millis_now += 40;
        TimerMock::getInstance()->setMillis(millis_now);

        // nothing is expected because it is a simple switch (only sends cmds when pushed)
        input->loopOnce();
        EXPECT_EQ("", Serial.getDataStr());

        // change the pin to LOW (swith is ON now)
        digitalWrite(pin, LOW);

        // change the millis... add 40 millis
        millis_now += 40;
        TimerMock::getInstance()->setMillis(millis_now);

        // loop the input, a cmd should be generated
        input->loopOnce();
        EXPECT_EQ("4,SIMCONNECT: EVENT ON;", Serial.getDataStr());

        // request state, it should be on
        input->requestState();
        EXPECT_EQ("4,SIMCONNECT: EVENT ON;", Serial.getDataStr());
    }

    TEST_F (InputSwitchTest, test_input_switch_on_event_send_value )
    {
        int pin = 1;
        unsigned long millis_now = 0;
        // input is pullup, so set the pin to HIGH 
        digitalWrite(pin, HIGH);

        // also initialise millis to 0
        TimerMock::getInstance()->setMillis(millis_now);

        std::unique_ptr<spaduino::input::InputSwitch> input(new spaduino::input::InputSwitch(
                                                    pin, 
                                                    spaduino::FSDataID::ID_11,
                                                    std::make_shared<FSActionSendValue>(*(comm_.get()), spaduino::FSDataID::ID_11, 1),
                                                    nullptr));
        input->sendDataAllowed(true);
        std::stringstream ss;
        ss << static_cast<int>(input->getID());
        std::string command_swith_id= ss.str();

        EXPECT_EQ("", Serial.getDataStr()); // no pending commands

        // loop the input, no cmd should be generated.
        input->loopOnce();
        EXPECT_EQ("", Serial.getDataStr());

        // change the pin to LOW (swith is ON now)
        digitalWrite(pin, LOW);
        input->loopOnce();

        // change the millis... add 40 millis
        millis_now += 40;
        TimerMock::getInstance()->setMillis(millis_now);

        // loop the input, a cmd should be generated
        input->loopOnce();
        EXPECT_EQ(command_swith_id+",1;", Serial.getDataStr());

        // change the pin to HIGH (swith is OFF now)
        digitalWrite(pin, HIGH);

        // change the millis... add 40 millis
        millis_now += 40;
        TimerMock::getInstance()->setMillis(millis_now);

        // nothing is expected because it is a simple switch (only sends cmds when pushed)
        input->loopOnce();
        EXPECT_EQ("", Serial.getDataStr());

        // change the pin to LOW (swith is ON now)
        digitalWrite(pin, LOW);

        // change the millis... add 40 millis
        millis_now += 40;
        TimerMock::getInstance()->setMillis(millis_now);

        // loop the input, a cmd should be generated
        input->loopOnce();
        EXPECT_EQ(command_swith_id+",1;", Serial.getDataStr());

        // request state, it should be on
        input->requestState();
        EXPECT_EQ(command_swith_id+",1;", Serial.getDataStr());
    }

    TEST_F (InputSwitchTest,  test_input_switch_on_off_events )
    {
        int pin = 1;
        unsigned long millis_now = 0;
        // input is pullup, so set the pin to HIGH 
        digitalWrite(pin, HIGH);
        // also initialise millis to 0
        TimerMock::getInstance()->setMillis(millis_now);

        std::unique_ptr<spaduino::input::InputSwitch> input(new spaduino::input::InputSwitch(
                                                    pin, 
                                                    spaduino::FSDataID::ID_11,
                                                    std::make_shared<FSActionSendEvent>( *(comm_.get()), F("SIMCONNECT: EVENT ON")),
                                                    std::make_shared<FSActionSendEvent>( *(comm_.get()), F("SIMCONNECT: EVENT OFF"))));
        input->sendDataAllowed(true);

        // check there is no pending command
        EXPECT_EQ("", Serial.getDataStr());

        // loop the input, no cmd should be generated.
        input->loopOnce();
        EXPECT_EQ("", Serial.getDataStr());

        // change the pin to LOW (swith is ON now)
        digitalWrite(pin, LOW);

        // change the millis... add 40 millis
        millis_now += 40;
        TimerMock::getInstance()->setMillis(millis_now);

        // loop the input, a cmd should be generated
        input->loopOnce();
        EXPECT_EQ("4,SIMCONNECT: EVENT ON;", Serial.getDataStr());

        // change the pin to HIGH (swith is OFF now)
        digitalWrite(pin, HIGH);

        // change the millis... add 40 millis
        millis_now += 40;
        TimerMock::getInstance()->setMillis(millis_now);

        // the off cmd should be triggered now
        input->loopOnce();
        EXPECT_EQ("4,SIMCONNECT: EVENT OFF;", Serial.getDataStr());

        // request state, it should be off
        input->requestState();
        EXPECT_EQ("4,SIMCONNECT: EVENT OFF;", Serial.getDataStr());

        // change the pin to LOW (swith is ON now)
        digitalWrite(pin, LOW);

        // change the millis... add 40 millis
        millis_now += 40;
        TimerMock::getInstance()->setMillis(millis_now);

        // the on cmd should be triggered now
        input->loopOnce();
        EXPECT_EQ("4,SIMCONNECT: EVENT ON;", Serial.getDataStr());

        // request state, it should be on
        input->requestState();
        EXPECT_EQ("4,SIMCONNECT: EVENT ON;", Serial.getDataStr());
    }

    TEST_F (InputSwitchTest,  test_input_switch_on_off_events_send_value )
    {
        int pin = 1;
        unsigned long millis_now = 0;
        // input is pullup, so set the pin to HIGH 
        digitalWrite(pin, HIGH);
        // also initialise millis to 0
        TimerMock::getInstance()->setMillis(millis_now);

        std::unique_ptr<spaduino::input::InputSwitch> input(new spaduino::input::InputSwitch(
                                                    pin, 
                                                    spaduino::FSDataID::ID_11,
                                                    std::make_shared<FSActionSendValue>(*(comm_.get()), spaduino::FSDataID::ID_11, 1),
                                                    std::make_shared<FSActionSendValue>(*(comm_.get()), spaduino::FSDataID::ID_11, 0)
                                                    ));
        input->sendDataAllowed(true);
        std::stringstream ss;
        ss << static_cast<int>(input->getID());
        std::string command_swith_id= ss.str();

        // check there is no pending command
        EXPECT_EQ("", Serial.getDataStr());

        // loop the input, no cmd should be generated.
        input->loopOnce();
        EXPECT_EQ("", Serial.getDataStr());

        // change the pin to LOW (swith is ON now)
        digitalWrite(pin, LOW);

        // change the millis... add 40 millis
        millis_now += 40;
        TimerMock::getInstance()->setMillis(millis_now);

        // loop the input, a cmd should be generated
        input->loopOnce();
        EXPECT_EQ(command_swith_id+",1;", Serial.getDataStr());

        // change the pin to HIGH (swith is OFF now)
        digitalWrite(pin, HIGH);

        // change the millis... add 40 millis
        millis_now += 40;
        TimerMock::getInstance()->setMillis(millis_now);

        // the off cmd should be triggered now
        input->loopOnce();
        EXPECT_EQ(command_swith_id+",0;", Serial.getDataStr());

        // request state, it should be off
        input->requestState();
        EXPECT_EQ(command_swith_id+",0;", Serial.getDataStr());

        // change the pin to LOW (swith is ON now)
        digitalWrite(pin, LOW);

        // change the millis... add 40 millis
        millis_now += 40;
        TimerMock::getInstance()->setMillis(millis_now);

        // the on cmd should be triggered now
        input->loopOnce();
        EXPECT_EQ(command_swith_id+",1;", Serial.getDataStr());

        // request state, it should be on
        input->requestState();
        EXPECT_EQ(command_swith_id+",1;", Serial.getDataStr());
    }
    
} // namespace test_input
} // namespace spaduino