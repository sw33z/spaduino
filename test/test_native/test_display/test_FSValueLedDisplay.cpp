#include <gtest/gtest.h>
#include "CmdMessengerTestBase.h"

#include "display/FSValueLedDisplay.h"

#include <iostream>

namespace spaduino
{
namespace test_display
{
    class FSValueLedDisplayTest: public spaduino::test::CmdMessengerTestBase
    {
        public:
            FSValueLedDisplayTest()
                : spaduino::test::CmdMessengerTestBase()
            {
            }
            ~FSValueLedDisplayTest() = default;
    };

    TEST_F( FSValueLedDisplayTest, test_display_led )
    {
        int LED_PIN_1 = 1;
        int LED_PIN_2 = 2;
        digitalWrite(LED_PIN_1, HIGH);

        // by default led is initialised to low
        std::unique_ptr<spaduino::display::FSValueLedDisplay> led1( 
            new spaduino::display::FSValueLedDisplay(spaduino::FSDataID::ID_10, LED_PIN_1));

        // led2 is initialised to high
        std::unique_ptr<spaduino::display::FSValueLedDisplay> led2(
            new spaduino::display::FSValueLedDisplay(spaduino::FSDataID::ID_11, LED_PIN_2, true));

        EXPECT_EQ(LOW, digitalRead(LED_PIN_1));
        EXPECT_EQ(HIGH, digitalRead(LED_PIN_2));

        // Turn led1 on
        updateDataInState(spaduino::FSDataID::ID_10, 1);
        led1->loopOnce();
        EXPECT_EQ(HIGH, digitalRead(LED_PIN_1));

/*
        cmd.valueID = spaduino::FSID::AVIONICS_ELECTRICAL_POWER;
        cmd.value = 0;
        led1->display(cmd);
        EXPECT_EQ(HIGH, mock->digitalRead(LED_PIN_1));
*/
        // turn led2 off
        updateDataInState(spaduino::FSDataID::ID_11, 0);
        led2->loopOnce();
        EXPECT_EQ(LOW, digitalRead(LED_PIN_2));

        // led1 is still on 
        updateDataInState(spaduino::FSDataID::ID_10, 1);
        led1->loopOnce();
        EXPECT_EQ(HIGH, digitalRead(LED_PIN_1));

        // turn led1 off
        updateDataInState(spaduino::FSDataID::ID_10, 0);
        led1->loopOnce();
        EXPECT_EQ(LOW, digitalRead(LED_PIN_1));

        // request state, should receive nothing
        led1->requestState();
        EXPECT_EQ("", Serial.getDataStr());
    }
}
} // namespace spaduino