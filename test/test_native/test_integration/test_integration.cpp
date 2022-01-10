#include <gtest/gtest.h>
#include "CmdMessengerTestBase.h"

#include <display/FSValueLedDisplay.h>
#include <input/InputSwitch.h>

#include <FSControls.h>
#include <FSAction.h>
#include <FSState.h>
#include <TimerMock.h>

#include <iostream>

using namespace spaduino;

namespace spaduino
{
namespace test_integration
{
    class IntegrationTests: public spaduino::test::CmdMessengerTestBase
    {
        public:
            IntegrationTests()
                : spaduino::test::CmdMessengerTestBase()
            {
            }
            ~IntegrationTests() = default;
    };

    TEST_F(IntegrationTests, test_one_led_one_switch)
    {
        FSControls fscontrols(F("FSCONTROLS"), *(comm_.get()));
        fscontrols.subscribe(F("SIMCONNECT:AUTOPILOT ALTITUDE LOCK"), FSDataID::ID_10);
        fscontrols.subscribe(F("SIMCONNECT:TAXI LIGHT"), FSDataID::ID_11);
        std::stringstream ss_lock_altitude_id;
        ss_lock_altitude_id << static_cast<int>(FSDataID::ID_10);
        std::string id_lock_altitude = ss_lock_altitude_id.str();

        int LED_PIN_1 = 1;
        std::shared_ptr<spaduino::display::FSValueLedDisplay> led_altitude_hold( 
                        new spaduino::display::FSValueLedDisplay(spaduino::FSDataID::ID_10, LED_PIN_1));
                
        fscontrols.addDisplayObject(led_altitude_hold);

        int INPUT_PIN = 2;
        std::shared_ptr<input::InputSwitch> input(new input::InputSwitch(
                                            INPUT_PIN, 
                                            spaduino::FSDataID::ID_11,
                                            std::make_shared<FSActionSendValue>(*(comm_.get()), FSDataID::ID_11, 1),
                                            std::make_shared<FSActionSendValue>(*(comm_.get()), FSDataID::ID_11, 0)));
        fscontrols.addInputObject(input);

        Serial.print("0,INIT;");
        comm_->feedinSerialData();
        EXPECT_EQ("0,SPAD,FSCONTROLS,Ardupilot;", Serial.getDataStrTestBuffer());

        Serial.print("0,CONFIG;");
        comm_->feedinSerialData();
        EXPECT_EQ("1,SUBSCRIBE,10,SIMCONNECT:AUTOPILOT ALTITUDE LOCK;1,SUBSCRIBE,11,SIMCONNECT:TAXI LIGHT;0,CONFIG;", Serial.getDataStrTestBuffer());

        Serial.print("2,START;");
        comm_->feedinSerialData();
        EXPECT_EQ("", Serial.getDataStr());

        EXPECT_EQ(LOW, digitalRead(LED_PIN_1));

        // Update value
        Serial.print(std::string(id_lock_altitude + ",1;").c_str());
        comm_->feedinSerialData();
        fscontrols.loopOnce();
        EXPECT_EQ(1, FSState::getInstance().getValue(FSDataID::ID_10));
        EXPECT_EQ(HIGH, digitalRead(LED_PIN_1));

        // Update value, same value
        Serial.print(std::string(id_lock_altitude + ",1;").c_str());
        comm_->feedinSerialData();
        fscontrols.loopOnce();
        EXPECT_EQ(1, FSState::getInstance().getValue(FSDataID::ID_10));
        EXPECT_EQ(HIGH, digitalRead(LED_PIN_1));

        // Update vale
        Serial.print(std::string(id_lock_altitude + ",0;").c_str());
        comm_->feedinSerialData();
        fscontrols.loopOnce();
        EXPECT_EQ(0, FSState::getInstance().getValue(FSDataID::ID_10));
        EXPECT_EQ(LOW, digitalRead(LED_PIN_1));


        // INPUT TEST
        unsigned long millis_now = 0;
        // input is pullup, so set the pin to HIGH 
        digitalWrite(INPUT_PIN, HIGH);

        // also initialise millis to 0
        TimerMock::getInstance()->setMillis(millis_now);

        // change the pin to LOW (swith is ON now)
        digitalWrite(INPUT_PIN, LOW);
        fscontrols.loopOnce();

        // change the millis... add 40 millis
        millis_now += 40;
        TimerMock::getInstance()->setMillis(millis_now);

        // loop the input, a cmd should be generated
        fscontrols.loopOnce();
        EXPECT_EQ("11,1;", Serial.getDataStr());

        digitalWrite(INPUT_PIN, HIGH);
        millis_now += 40;
        TimerMock::getInstance()->setMillis(millis_now);
        fscontrols.loopOnce();
        EXPECT_EQ("11,0;", Serial.getDataStr());

        // send STOP (inputs are not allowed to send commands now)
        Serial.print("2,STOP;");
        comm_->feedinSerialData();
        EXPECT_EQ("", Serial.getDataStr());

        // input is pullup, so set the pin to HIGH 
        digitalWrite(INPUT_PIN, HIGH);

        // also initialise millis to 0
        TimerMock::getInstance()->setMillis(millis_now);

        // change the pin to LOW (swith is ON now)
        digitalWrite(INPUT_PIN, LOW);
        fscontrols.loopOnce();

        // change the pin to LOW (swith is ON now)
        digitalWrite(INPUT_PIN, LOW);
        fscontrols.loopOnce();

        // change the millis... add 40 millis
        millis_now += 40;
        TimerMock::getInstance()->setMillis(millis_now);

        // loop the input, a cmd should not be generated (because we sent STOP)
        fscontrols.loopOnce();
        EXPECT_EQ("", Serial.getDataStr());
    }
} // namespace test_integration

} // namespace spaduino