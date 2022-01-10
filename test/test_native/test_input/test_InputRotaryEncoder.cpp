#include <gtest/gtest.h>
#include "CmdMessengerTestBase.h"

#include <input/InputRotaryEncoder.h>
#include <Arduino.h>
#include <TimerMock.h>
#include <FSAction.h>
#include <FSState.h>

using namespace spaduino;

namespace test_input
{
    constexpr int CLICK_DEBOUNCE = 200;

    constexpr int ENCODER_DEBOUNCE = 5;
    constexpr int SHORT_TIME = ENCODER_DEBOUNCE + 20;

    class InputRotaryEncoderTest: public spaduino::test::CmdMessengerTestBase
    {
        public:
            InputRotaryEncoderTest()
                : spaduino::test::CmdMessengerTestBase()
            {
            }
            ~InputRotaryEncoderTest() = default;

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
                                    spaduino::input::InputRotaryEncoder & encoder,
                                    int pinC, unsigned long time_between_click)
            {
                unsigned long millisVal = millis();
                for (int i = 0 ; i < clicks ; ++i)
                {
                    millisVal += (time_between_click + 1);
                    TimerMock::getInstance()->setMillis(millisVal);
                    digitalWrite(pinC, LOW);
                    encoder.loopOnce();
                }
            }

            void button_release(int clicks,  
                                    spaduino::input::InputRotaryEncoder & encoder,
                                    int pinC, unsigned long time_between_click)
            {
                unsigned long millisVal = millis();
                for (int i = 0 ; i < clicks ; ++i)
                {
                    millisVal += (time_between_click + 1);
                    TimerMock::getInstance()->setMillis(millisVal);
                    digitalWrite(pinC, HIGH);
                    encoder.loopOnce();
                }
            }
    };

    TEST_F ( InputRotaryEncoderTest, test_rotary_encoder_increment_slow )
    {
        spaduino::FSState::getInstance().setValue(spaduino::FSDataID::ID_11, 100);
        digitalWrite(1, HIGH);
        digitalWrite(2, HIGH);
        digitalWrite(3, HIGH);
        TimerMock::getInstance()->setMillis(0);
        spaduino::input::InputRotaryEncoder encoder(1, 2, 3, spaduino::FSDataID::ID_11, 
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 500),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -500),
                                                        nullptr,
                                                        true
                                                        );
        encoder.sendDataAllowed(true);

        std::stringstream ss;
        ss << static_cast<int>(spaduino::FSDataID::ID_11);
        std::string strID = ss.str();

        // no cmd should be generated.
        EXPECT_EQ("", Serial.getDataStr());
        full_rotate_increase(1, encoder, 1, 2, SHORT_TIME);

        // 1 rotation... 1 cmd should be found
        EXPECT_EQ(strID+",200;", Serial.getDataStr());

        spaduino::FSState::getInstance().setValue(spaduino::FSDataID::ID_11, 200);

        // 10 rotations
        int NUM_ROTATIONS = 10;
        full_rotate_increase(NUM_ROTATIONS, encoder, 1, 2, SHORT_TIME);
        std::string tenTimes;
        for (int i = 0; i < 10; ++i)
        {
            tenTimes += strID+",300;";
        }
        EXPECT_EQ(tenTimes, Serial.getDataStr());

        EXPECT_EQ("", Serial.getDataStr());
    }

    TEST_F ( InputRotaryEncoderTest, test_rotary_encoder_decrement_slow )
    {
        spaduino::FSState::getInstance().setValue(spaduino::FSDataID::ID_11, 200);
        digitalWrite(1, HIGH);
        digitalWrite(2, HIGH);
        digitalWrite(3, HIGH);
        TimerMock::getInstance()->setMillis(0);
        spaduino::input::InputRotaryEncoder encoder(1, 2, 3, spaduino::FSDataID::ID_11, 
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 500),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -500),
                                                        nullptr,
                                                        true
                                                        );
        encoder.sendDataAllowed(true);

        std::stringstream ss;
        ss << static_cast<int>(spaduino::FSDataID::ID_11);
        std::string strID = ss.str();

        // no cmd should be generated.
        EXPECT_EQ("", Serial.getDataStr());

        full_rotate_decrease(1, encoder, 1, 2, SHORT_TIME);
        // 1 rotation... 1 cmd should be found
        EXPECT_EQ(strID+",100;", Serial.getDataStr());

        // 10 rotations
        int NUM_ROTATIONS = 10;
        std::string tenTimes;
        for (int i = 0; i < NUM_ROTATIONS; ++i)
        {
            tenTimes += strID+",100;";
        }
        full_rotate_decrease(NUM_ROTATIONS, encoder, 1, 2, SHORT_TIME);
        EXPECT_EQ(tenTimes, Serial.getDataStr());

        EXPECT_EQ("", Serial.getDataStr());
    }

    TEST_F ( InputRotaryEncoderTest, test_rotary_encoder_increment_fast )
    {
        digitalWrite(1, HIGH);
        digitalWrite(2, HIGH);
        digitalWrite(3, HIGH);
        TimerMock::getInstance()->setMillis(0);
        spaduino::input::InputRotaryEncoder encoder(1, 2, 3, spaduino::FSDataID::ID_11, 
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 500),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -500),
                                                        nullptr,
                                                        true
                                                        );
        encoder.sendDataAllowed(true);
        std::stringstream ss;
        ss << static_cast<int>(spaduino::FSDataID::ID_11);
        std::string strID = ss.str();

        // no cmd should be generated.
        EXPECT_EQ("", Serial.getDataStr());

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

    TEST_F ( InputRotaryEncoderTest, test_rotary_encoder_decrement_fast )
    {
        digitalWrite(1, HIGH);
        digitalWrite(2, HIGH);
        digitalWrite(3, HIGH);
        TimerMock::getInstance()->setMillis(0);
        spaduino::input::InputRotaryEncoder encoder(1, 2, 3, spaduino::FSDataID::ID_11, 
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 500),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -500),
                                                        nullptr,
                                                        true
                                                        );
        encoder.sendDataAllowed(true);
        std::stringstream ss;
        ss << static_cast<int>(spaduino::FSDataID::ID_11);
        std::string strID = ss.str();

        // no cmd should be generated.
        EXPECT_EQ("", Serial.getDataStr());

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

    TEST_F ( InputRotaryEncoderTest, test_rotary_encoder_increment_fast_but_is_not_accelerated )
    {
        digitalWrite(1, HIGH);
        digitalWrite(2, HIGH);
        digitalWrite(3, HIGH);
        TimerMock::getInstance()->setMillis(0);
        spaduino::input::InputRotaryEncoder encoder(1, 2, 3, spaduino::FSDataID::ID_11, 
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 500),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -500),
                                                        nullptr,
                                                        false
                                                        );
        encoder.sendDataAllowed(true);
        std::stringstream ss;
        ss << static_cast<int>(spaduino::FSDataID::ID_11);
        std::string strID = ss.str();

        // no cmd should be generated.
        EXPECT_EQ("", Serial.getDataStr());

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

    TEST_F ( InputRotaryEncoderTest, test_rotary_encoder_decrement_fast_but_is_not_accelerated )
    {
        digitalWrite(1, HIGH);
        digitalWrite(2, HIGH);
        digitalWrite(3, HIGH);
        TimerMock::getInstance()->setMillis(0);
        spaduino::input::InputRotaryEncoder encoder(1, 2, 3, spaduino::FSDataID::ID_11, 
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 500),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -500),
                                                        nullptr,
                                                        false
                                                        );
        encoder.sendDataAllowed(true);
        std::stringstream ss;
        ss << static_cast<int>(spaduino::FSDataID::ID_11);
        std::string strID = ss.str();

        // no cmd should be generated.
        EXPECT_EQ("", Serial.getDataStr());

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

    TEST_F ( InputRotaryEncoderTest, test_rotary_encoder_click )
    {
        digitalWrite(1, HIGH);
        digitalWrite(2, HIGH);
        digitalWrite(3, HIGH);
        TimerMock::getInstance()->setMillis(0);
        spaduino::input::InputRotaryEncoder encoder(1, 2, 3, spaduino::FSDataID::ID_11, 
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 500),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -500),
                                                        std::make_shared<FSActionSendEvent>( *(comm_.get()), F("CLICK_EVENT")),
                                                        false
                                                        );
        encoder.sendDataAllowed(true);

        // no cmd should be generated.
        EXPECT_EQ("", Serial.getDataStr());

        // click 
        button_click(1, encoder, 3, CLICK_DEBOUNCE);
        EXPECT_EQ("4,CLICK_EVENT;", Serial.getDataStr());

        // release
        button_release(1, encoder, 3, CLICK_DEBOUNCE);
        EXPECT_EQ("", Serial.getDataStr());

        // click 
        button_click(1, encoder, 3, CLICK_DEBOUNCE);
        EXPECT_EQ("4,CLICK_EVENT;", Serial.getDataStr());  

        // release
        button_release(1, encoder, 3, 2);
        EXPECT_EQ("", Serial.getDataStr());

        // click rebounce
        button_click(1, encoder, 3, 2);
        EXPECT_EQ("", Serial.getDataStr());

        // release
        button_release(1, encoder, 3, CLICK_DEBOUNCE);
        EXPECT_EQ("", Serial.getDataStr());
    }

    TEST_F ( InputRotaryEncoderTest, test_rotary_encoder_request_state )
    {
        digitalWrite(1, HIGH);
        digitalWrite(2, HIGH);
        digitalWrite(3, HIGH);
        TimerMock::getInstance()->setMillis(0);
        spaduino::input::InputRotaryEncoder encoder(1, 2, 3, spaduino::FSDataID::ID_11, 
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, 500),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -100),
                                                        std::make_shared<FSActionSendIncrementValue>( *(comm_.get()), spaduino::FSDataID::ID_11, -500),
                                                        std::make_shared<FSActionSendEvent>( *(comm_.get()), F("CLICK_EVENT")),
                                                        false
                                                        );
        encoder.sendDataAllowed(true);

        // no cmd should be generated.
        EXPECT_EQ("", Serial.getDataStr());

        // request state for rotary encoder should do nothing
        encoder.requestState();
        EXPECT_EQ("", Serial.getDataStr());
    }
}
