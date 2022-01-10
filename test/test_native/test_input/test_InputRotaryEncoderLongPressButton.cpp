#include <gtest/gtest.h>
#include "CmdMessengerTestBase.h"

#include <input/InputRotaryEncoderLongPressButton.h>
#include <FSAction.h>

#include <TimerMock.h>
#include <Arduino.h>

using namespace spaduino;

namespace test_input
{
    constexpr int CLICK_DEBOUNCE = 200;
    constexpr int LONG_PRESS = 1000;

    constexpr int ENCODER_DEBOUNCE = 5;
    constexpr int SHORT_TIME = ENCODER_DEBOUNCE + 20;

    class InputRotaryEncoderLongPressButtonTest: public spaduino::test::CmdMessengerTestBase
    {
        public:
            InputRotaryEncoderLongPressButtonTest()
                : spaduino::test::CmdMessengerTestBase()
            {
            }
            ~InputRotaryEncoderLongPressButtonTest() = default;

            void full_rotate_increase(int rotations, 
                                    spaduino::input::InputRotaryEncoder & encoder,
                                    int pinA, int pinB, unsigned long time_between_rotations)
            {
                unsigned long millisVal = millis();
                for (int i = 0 ; i < rotations ; ++i)
                {
                    digitalWrite(pinA, HIGH);
                    digitalWrite(pinB, LOW);
                    encoder.loopOnce();

                    digitalWrite(pinA, LOW);
                    digitalWrite(pinB, LOW);
                    encoder.loopOnce();

                    digitalWrite(pinA, LOW);
                    digitalWrite(pinB, HIGH);
                    encoder.loopOnce();

                    millisVal += (time_between_rotations + 1);
                    TimerMock::getInstance()->setMillis(millisVal);
                    digitalWrite(pinA, HIGH);
                    digitalWrite(pinB, HIGH);
                    encoder.loopOnce();
                }
            }

            void full_rotate_decrease(int rotations, 
                                    spaduino::input::InputRotaryEncoder & encoder,
                                    int pinA, int pinB, unsigned long time_between_rotations)
            {
                unsigned long millisVal = millis();
                for (int i = 0 ; i < rotations ; ++i)
                {
                    digitalWrite(pinA, LOW);
                    digitalWrite(pinB, HIGH);
                    encoder.loopOnce();

                    digitalWrite(pinA, LOW);
                    digitalWrite(pinB, LOW);
                    encoder.loopOnce();

                    digitalWrite(pinA, HIGH);
                    digitalWrite(pinB, LOW);
                    encoder.loopOnce();

                    millisVal += (time_between_rotations + 1);
                    TimerMock::getInstance()->setMillis(millisVal);
                    digitalWrite(pinA, HIGH);
                    digitalWrite(pinB, HIGH);
                    encoder.loopOnce();
                }
            }

            void button_click(int clicks,
                                    spaduino::input::InputRotaryEncoderLongPressButton & encoder,
                                    int pinC, unsigned long time_between_click)
            {
                unsigned long millisValue = millis();
                for (int i = 0 ; i < clicks ; ++i)
                {
                    millisValue += (time_between_click + 1);
                    TimerMock::getInstance()->setMillis(millisValue);
                    digitalWrite(pinC, LOW); // press button
                    encoder.loopOnce();
                    millisValue += (time_between_click + 1);
                    TimerMock::getInstance()->setMillis(millisValue);
                    digitalWrite(pinC, HIGH); // release button
                    encoder.loopOnce();
                }
            }

            void button_long_press(int clicks,
                                    spaduino::input::InputRotaryEncoderLongPressButton & encoder,
                                    int pinC, unsigned long time_between_click, unsigned long time_press, bool release)
            {
                unsigned long millisValue = millis();
                for (int i = 0 ; i < clicks ; ++i)
                {
                    millisValue += (time_between_click + 1);
                    TimerMock::getInstance()->setMillis(millisValue);
                    digitalWrite(pinC, LOW); // press button
                    encoder.loopOnce();
                    millisValue += (time_press + 1);
                    TimerMock::getInstance()->setMillis(millisValue);
                    encoder.loopOnce();

                    millisValue = (millisValue + time_between_click + 1);
                    if (release)
                    {
                        digitalWrite(pinC, HIGH); // release button
                    }
                    TimerMock::getInstance()->setMillis(millisValue);
                    encoder.loopOnce();
                }
            }

            void button_release(int clicks, 
                                    spaduino::input::InputRotaryEncoderLongPressButton & encoder,
                                    int pinC, unsigned long time_between_click)
            {
                unsigned long millisValue = millis();
                for (int i = 0 ; i < clicks ; ++i)
                {
                    millisValue += (time_between_click + 1);
                    TimerMock::getInstance()->setMillis(millisValue);
                    digitalWrite(pinC, HIGH);
                    encoder.loopOnce();
                }
            }

    };

    TEST_F (InputRotaryEncoderLongPressButtonTest, test_rotary_encoder_long_press_increment_slow )
    {
        digitalWrite(1, HIGH);
        digitalWrite(2, HIGH);
        digitalWrite(3, HIGH);
        TimerMock::getInstance()->setMillis(0);
        spaduino::input::InputRotaryEncoderLongPressButton encoder(1, 2, 3, spaduino::FSDataID::ID_11, 
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 500),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -500),
                                                        std::make_shared<FSActionSendEvent>( *(comm_.get()), F("CLICK_EVENT")),
                                                        std::make_shared<FSActionSendEvent>( *(comm_.get()), F("LONG_PRESS_EVENT")),
                                                        true
                                                        );
        encoder.sendDataAllowed(true);

        std::stringstream ss;
        ss << static_cast<int>(spaduino::FSDataID::ID_11);
        std::string strID = ss.str();

        full_rotate_increase(1, encoder, 1, 2, SHORT_TIME);

        // 1 rotation... 1 cmd should be found
        EXPECT_EQ(strID+",100;", Serial.getDataStr());

        // 10 rotations
        int NUM_ROTATIONS = 10;
        std::string tenTimes;
        for (int i = 0; i < NUM_ROTATIONS; ++i)
        {
            tenTimes += strID+",100;";
        }
        full_rotate_increase(NUM_ROTATIONS, encoder, 1, 2, SHORT_TIME);
        EXPECT_EQ(tenTimes, Serial.getDataStr());

        EXPECT_EQ("", Serial.getDataStr());
    }

    TEST_F (InputRotaryEncoderLongPressButtonTest,  test_rotary_encoder_long_press_decrement_slow )
    {
        digitalWrite(1, HIGH);
        digitalWrite(2, HIGH);
        digitalWrite(3, HIGH);
        TimerMock::getInstance()->setMillis(0);
        spaduino::input::InputRotaryEncoderLongPressButton encoder(1, 2, 3, spaduino::FSDataID::ID_11, 
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 500),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -500),
                                                        std::make_shared<FSActionSendEvent>( *(comm_.get()), F("CLICK_EVENT")),
                                                        std::make_shared<FSActionSendEvent>( *(comm_.get()), F("LONG_PRESS_EVENT")),
                                                        true
                                                        );
        encoder.sendDataAllowed(true);

        std::stringstream ss;
        ss << static_cast<int>(spaduino::FSDataID::ID_11);
        std::string strID = ss.str();

        full_rotate_decrease(1, encoder, 1, 2, SHORT_TIME);

        // 1 rotation... 1 cmd should be found
        EXPECT_EQ(strID+",-100;", Serial.getDataStr());

        full_rotate_decrease(1, encoder, 1, 2, SHORT_TIME);

        // 1 rotation... 1 cmd should be found
        EXPECT_EQ(strID+",-100;", Serial.getDataStr());

        // 10 rotations
        int NUM_ROTATIONS = 10;
        std::string tenTimes;
        for (int i = 0; i < NUM_ROTATIONS; ++i)
        {
            tenTimes += strID+",-100;";
        }
        full_rotate_decrease(NUM_ROTATIONS, encoder, 1, 2, SHORT_TIME);
        EXPECT_EQ(tenTimes, Serial.getDataStr());

        EXPECT_EQ("", Serial.getDataStr());
    }

    TEST_F (InputRotaryEncoderLongPressButtonTest,  test_rotary_encoder_long_press_increment_fast )
    {
        digitalWrite(1, HIGH);
        digitalWrite(2, HIGH);
        digitalWrite(3, HIGH);
        TimerMock::getInstance()->setMillis(0);
        spaduino::input::InputRotaryEncoderLongPressButton encoder(1, 2, 3, spaduino::FSDataID::ID_11, 
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 500),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -500),
                                                        std::make_shared<FSActionSendEvent>( *(comm_.get()), F("CLICK_EVENT")),
                                                        std::make_shared<FSActionSendEvent>( *(comm_.get()), F("LONG_PRESS_EVENT")),
                                                        true
                                                        );
        encoder.sendDataAllowed(true);

        std::stringstream ss;
        ss << static_cast<int>(spaduino::FSDataID::ID_11);
        std::string strID = ss.str();

        full_rotate_increase(1, encoder, 1, 2, ENCODER_DEBOUNCE);

        // 1 rotation... 1 cmd should be found
        EXPECT_EQ(strID+",500;", Serial.getDataStr());

        // 10 rotations
        int NUM_ROTATIONS = 10;
        std::string tenTimes;
        for (int i = 0; i < NUM_ROTATIONS; ++i)
        {
            tenTimes += strID+",500;";
        }
        full_rotate_increase(NUM_ROTATIONS, encoder, 1, 2, ENCODER_DEBOUNCE);
        EXPECT_EQ(tenTimes, Serial.getDataStr());

        EXPECT_EQ("", Serial.getDataStr());
    }

    TEST_F (InputRotaryEncoderLongPressButtonTest,  test_rotary_encoder_long_press_decrement_fast )
    {
        digitalWrite(1, HIGH);
        digitalWrite(2, HIGH);
        digitalWrite(3, HIGH);
        TimerMock::getInstance()->setMillis(0);
        spaduino::input::InputRotaryEncoderLongPressButton encoder(1, 2, 3, spaduino::FSDataID::ID_11, 
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 500),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -500),
                                                        std::make_shared<FSActionSendEvent>( *(comm_.get()), F("CLICK_EVENT")),
                                                        std::make_shared<FSActionSendEvent>( *(comm_.get()), F("LONG_PRESS_EVENT")),
                                                        true
                                                        );
        encoder.sendDataAllowed(true);

        std::stringstream ss;
        ss << static_cast<int>(spaduino::FSDataID::ID_11);
        std::string strID = ss.str();

        full_rotate_decrease(1, encoder, 1, 2, ENCODER_DEBOUNCE);

        // 1 rotation... 1 cmd should be found
        EXPECT_EQ(strID+",-500;", Serial.getDataStr());

        // 10 rotations
        int NUM_ROTATIONS = 10;
        std::string tenTimes;
        for (int i = 0; i < NUM_ROTATIONS; ++i)
        {
            tenTimes += strID+",-500;";
        }
        full_rotate_decrease(NUM_ROTATIONS, encoder, 1, 2, ENCODER_DEBOUNCE);
        EXPECT_EQ(tenTimes, Serial.getDataStr());

        EXPECT_EQ("", Serial.getDataStr());
    }

    TEST_F (InputRotaryEncoderLongPressButtonTest,  test_rotary_encoder_long_press_increment_fast_but_is_not_accelerated )
    {
        digitalWrite(1, HIGH);
        digitalWrite(2, HIGH);
        digitalWrite(3, HIGH);
        TimerMock::getInstance()->setMillis(0);
        spaduino::input::InputRotaryEncoderLongPressButton encoder(1, 2, 3, spaduino::FSDataID::ID_11, 
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 500),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -500),
                                                        std::make_shared<FSActionSendEvent>( *(comm_.get()), F("CLICK_EVENT")),
                                                        std::make_shared<FSActionSendEvent>( *(comm_.get()), F("LONG_PRESS_EVENT")),
                                                        false
                                                        );
        encoder.sendDataAllowed(true);

        std::stringstream ss;
        ss << static_cast<int>(spaduino::FSDataID::ID_11);
        std::string strID = ss.str();

        full_rotate_increase(1, encoder, 1, 2, ENCODER_DEBOUNCE);

        // 1 rotation... 1 cmd should be found
        EXPECT_EQ(strID+",100;", Serial.getDataStr());

        // 10 rotations
        int NUM_ROTATIONS = 10;
        std::string tenTimes;
        for (int i = 0; i < NUM_ROTATIONS; ++i)
        {
            tenTimes += strID+",100;";
        }
        full_rotate_increase(NUM_ROTATIONS, encoder, 1, 2, ENCODER_DEBOUNCE);
        EXPECT_EQ(tenTimes, Serial.getDataStr());

        EXPECT_EQ("", Serial.getDataStr());
    }

    TEST_F (InputRotaryEncoderLongPressButtonTest,  test_rotary_encoder_long_press_decrement_fast_but_is_not_accelerated )
    {
        digitalWrite(1, HIGH);
        digitalWrite(2, HIGH);
        digitalWrite(3, HIGH);
        TimerMock::getInstance()->setMillis(0);
        spaduino::input::InputRotaryEncoderLongPressButton encoder(1, 2, 3, spaduino::FSDataID::ID_11, 
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 500),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -500),
                                                        std::make_shared<FSActionSendEvent>( *(comm_.get()), F("CLICK_EVENT")),
                                                        std::make_shared<FSActionSendEvent>( *(comm_.get()), F("LONG_PRESS_EVENT")),
                                                        false
                                                        );
        encoder.sendDataAllowed(true);

        std::stringstream ss;
        ss << static_cast<int>(spaduino::FSDataID::ID_11);
        std::string strID = ss.str();

        full_rotate_decrease(1, encoder, 1, 2, ENCODER_DEBOUNCE);

        // 1 rotation... 1 cmd should be found
        EXPECT_EQ(strID+",-100;", Serial.getDataStr());

        // 10 rotations
        int NUM_ROTATIONS = 10;
        std::string tenTimes;
        for (int i = 0; i < NUM_ROTATIONS; ++i)
        {
            tenTimes += strID+",-100;";
        }
        full_rotate_decrease(NUM_ROTATIONS, encoder, 1, 2, ENCODER_DEBOUNCE);
        EXPECT_EQ(tenTimes, Serial.getDataStr());

        EXPECT_EQ("", Serial.getDataStr());
    }

    TEST_F (InputRotaryEncoderLongPressButtonTest,  test_rotary_encoder_long_press_click )
    {
        digitalWrite(1, HIGH);
        digitalWrite(2, HIGH);
        digitalWrite(3, HIGH);
        TimerMock::getInstance()->setMillis(0);
        spaduino::input::InputRotaryEncoderLongPressButton encoder(1, 2, 3, spaduino::FSDataID::ID_11, 
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 500),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -500),
                                                        std::make_shared<FSActionSendEvent>( *(comm_.get()), F("CLICK_EVENT")),
                                                        std::make_shared<FSActionSendEvent>( *(comm_.get()), F("LONG_PRESS_EVENT")),
                                                        false
                                                        );
        encoder.sendDataAllowed(true);

        std::stringstream ss;
        ss << static_cast<int>(spaduino::FSDataID::ID_11);
        std::string strID = ss.str();

        // click 
        button_click(1, encoder, 3, CLICK_DEBOUNCE);
        EXPECT_EQ("4,CLICK_EVENT;", Serial.getDataStr());

        // click 
        button_click(1, encoder, 3, CLICK_DEBOUNCE);
        EXPECT_EQ("4,CLICK_EVENT;", Serial.getDataStr());

        // click rebounce
        button_click(1, encoder, 3, 2);
        EXPECT_EQ("", Serial.getDataStr());
    }

    TEST_F (InputRotaryEncoderLongPressButtonTest,  test_rotary_encoder_long_press_request_state )
    {
        digitalWrite(1, HIGH);
        digitalWrite(2, HIGH);
        digitalWrite(3, HIGH);
        TimerMock::getInstance()->setMillis(0);
        spaduino::input::InputRotaryEncoderLongPressButton encoder(1, 2, 3, spaduino::FSDataID::ID_11, 
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 500),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -500),
                                                        std::make_shared<FSActionSendEvent>( *(comm_.get()), F("CLICK_EVENT")),
                                                        std::make_shared<FSActionSendEvent>( *(comm_.get()), F("LONG_PRESS_EVENT")),
                                                        false
                                                        );
        encoder.sendDataAllowed(true);

        // request state for rotary encoder should do nothing
        encoder.requestState();
        EXPECT_EQ("", Serial.getDataStr());
    }

    TEST_F (InputRotaryEncoderLongPressButtonTest,  test_rotary_encoder_long_press_button_long_press )
    {
        digitalWrite(1, HIGH);
        digitalWrite(2, HIGH);
        digitalWrite(3, HIGH);
        TimerMock::getInstance()->setMillis(0);
        spaduino::input::InputRotaryEncoderLongPressButton encoder(1, 2, 3, spaduino::FSDataID::ID_11, 
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 500),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -500),
                                                        std::make_shared<FSActionSendEvent>( *(comm_.get()), F("CLICK_EVENT")),
                                                        std::make_shared<FSActionSendEvent>( *(comm_.get()), F("LONG_PRESS_EVENT")),
                                                        false
                                                        );
        encoder.sendDataAllowed(true);

        // long press release
        button_long_press(1, encoder, 3, CLICK_DEBOUNCE, LONG_PRESS, true);
        EXPECT_EQ("4,LONG_PRESS_EVENT;", Serial.getDataStr());

        // long press no release
        button_long_press(1, encoder, 3, CLICK_DEBOUNCE, LONG_PRESS, false);
        EXPECT_EQ("4,LONG_PRESS_EVENT;", Serial.getDataStr()); 

        // release the button now
        long millisVal = millis();
        digitalWrite(3, HIGH); // release button
        TimerMock::getInstance()->setMillis(millisVal+CLICK_DEBOUNCE+1);
        encoder.loopOnce();
        EXPECT_EQ("", Serial.getDataStr());

        // normal press
        button_long_press(1, encoder, 3, CLICK_DEBOUNCE, CLICK_DEBOUNCE, true);
        EXPECT_EQ("4,CLICK_EVENT;", Serial.getDataStr());
    }
}
