#include <gtest/gtest.h>
#include "CmdMessengerTestBase.h"

#include <FSAction.h>
#include <FSState.h>

#include <SerialMock.h>
#include <sstream>

using namespace spaduino;

namespace test_spaduino
{
class FSActionTest: public spaduino::test::CmdMessengerTestBase
{
    public:
        FSActionTest()
            : spaduino::test::CmdMessengerTestBase()
        {
        }
        ~FSActionTest() = default;
};

TEST_F(FSActionTest, test_send_event)
{
    FSActionSendEvent action(*(comm_.get()), F("EVENT_NAME"));
    action.run();
    EXPECT_EQ("4,EVENT_NAME;", Serial.getDataStr());
}

TEST_F(FSActionTest, test_send_2_events)
{
    FSActionSend2Events action(*(comm_.get()), F("EVENT_NAME 1"), F("EVENT_NAME 2"));
    action.run();
    EXPECT_EQ("4,EVENT_NAME 1;4,EVENT_NAME 2;", Serial.getDataStr());
}

TEST_F(FSActionTest, test_send_3_events)
{
    FSActionSend3Events action(*(comm_.get()), F("EVENT_NAME 1"), F("EVENT_NAME 2"), F("EVENT_NAME 3"));
    action.run();
    EXPECT_EQ("4,EVENT_NAME 1;4,EVENT_NAME 2;4,EVENT_NAME 3;", Serial.getDataStr());
}

TEST_F(FSActionTest, test_send_3_events_variadic)
{
    FSActionSendEvents action(*(comm_.get()), F("EVENT_NAME 1"), F("EVENT_NAME 2"), F("EVENT_NAME 3"));
    action.run();
    EXPECT_EQ("4,EVENT_NAME 1;4,EVENT_NAME 2;4,EVENT_NAME 3;", Serial.getDataStr());
}

TEST_F(FSActionTest, test_send_4_events_variadic)
{
    FSActionSendEvents action(*(comm_.get()), F("EVENT_NAME 1"), F("EVENT_NAME 2"), F("EVENT_NAME 3"), F("EVENT_NAME 4"));
    action.run();
    EXPECT_EQ("4,EVENT_NAME 1;4,EVENT_NAME 2;4,EVENT_NAME 3;4,EVENT_NAME 4;", Serial.getDataStr());
}

TEST_F(FSActionTest, test_send_5_events_variadic)
{
    FSActionSendEvents action(*(comm_.get()), F("EVENT_NAME 1"), F("EVENT_NAME 2"), F("EVENT_NAME 3"), F("EVENT_NAME 4"), F("EVENT_NAME 5"));
    action.run();
    EXPECT_EQ("4,EVENT_NAME 1;4,EVENT_NAME 2;4,EVENT_NAME 3;4,EVENT_NAME 4;4,EVENT_NAME 5;", Serial.getDataStr());
}

TEST_F(FSActionTest, test_send_6_events_variadic)
{
    FSActionSendEvents action(*(comm_.get()), F("EVENT_NAME 1"), F("EVENT_NAME 2"), F("EVENT_NAME 3"), F("EVENT_NAME 4"), F("EVENT_NAME 5"), F("EVENT_NAME 6"));
    action.run();
    // max events is 5, last one is ignored
    EXPECT_EQ("4,EVENT_NAME 1;4,EVENT_NAME 2;4,EVENT_NAME 3;4,EVENT_NAME 4;4,EVENT_NAME 5;", Serial.getDataStr());
}

TEST_F(FSActionTest, test_send_value)
{
    FSActionSendValue action(*(comm_.get()), FSDataID::ID_10, 123);
    action.run();
    std::stringstream ss;
    ss << static_cast<int>(FSDataID::ID_10);
    EXPECT_EQ(ss.str() + ",123;", Serial.getDataStr());
}


TEST_F(FSActionTest, test_send_reference_value)
{
    FSState::getInstance().setValue(FSDataID::ID_11, 1234);
    FSActionSendReferenceValue action(*(comm_.get()), FSDataID::ID_11);
    action.run();
    std::stringstream ss;
    ss << static_cast<int>(FSDataID::ID_11);
    EXPECT_EQ(ss.str() + ",1234;", Serial.getDataStr());
    FSState::getInstance().clear();
}

TEST_F(FSActionTest, test_send_increment_value)
{
    FSState::getInstance().setValue(FSDataID::ID_11, 12);
    FSActionSendIncrementValue action(*(comm_.get()), FSDataID::ID_11, 10, FSActionValueMinMax(FSActionValueMinMax::Type::NONE));
    action.run();
    std::stringstream ss;
    ss << static_cast<int>(FSDataID::ID_11);
    EXPECT_EQ(ss.str() + ",22;", Serial.getDataStr());
    FSState::getInstance().clear();
}

TEST_F(FSActionTest, test_set_value_and_increment_value)
{
    FSState::getInstance().setValue(FSDataID::ID_11, 12);
    FSActionSetValueAndSendIncrementValue action(*(comm_.get()), FSDataID::ID_12, 123, FSDataID::ID_11, 10, FSActionValueMinMax(FSActionValueMinMax::Type::NONE));
    action.run();
    std::stringstream ss;
    ss << static_cast<int>(FSDataID::ID_12);
    std::stringstream ss2;
    ss2 << static_cast<int>(FSDataID::ID_11);
    EXPECT_EQ(ss.str() + ",123;" + ss2.str() + ",22;", Serial.getDataStr());
    FSState::getInstance().clear();
}

TEST_F(FSActionTest, test_set_value_and_increment_value_no_limits)
{
    FSState::getInstance().setValue(FSDataID::ID_11, 12);
    FSActionSetValueAndSendIncrementValue action(*(comm_.get()), FSDataID::ID_12, 123, FSDataID::ID_11, 10);
    action.run();
    std::stringstream ss;
    ss << static_cast<int>(FSDataID::ID_12);
    std::stringstream ss2;
    ss2 << static_cast<int>(FSDataID::ID_11);
    EXPECT_EQ(ss.str() + ",123;" + ss2.str() + ",22;", Serial.getDataStr());
    FSState::getInstance().clear();
}

TEST_F(FSActionTest, test_send_event_and_increment_value)
{
    FSState::getInstance().setValue(FSDataID::ID_11, 12);
    FSActionSendEventAndIncrementValue action(*(comm_.get()), F("EVENT_TO_SEND,1"), FSDataID::ID_11, 10, FSActionValueMinMax(FSActionValueMinMax::Type::NONE));
    action.run();
    std::stringstream ss;
    ss << static_cast<int>(FSDataID::ID_11);
    EXPECT_EQ("4,EVENT_TO_SEND,1;" + ss.str() + ",22;", Serial.getDataStr());
    FSState::getInstance().clear();
}

TEST_F(FSActionTest, test_send_event_and_increment_value_no_limits)
{
    FSState::getInstance().setValue(FSDataID::ID_11, 12);
    FSActionSendEventAndIncrementValue action(*(comm_.get()), F("EVENT_TO_SEND,1"), FSDataID::ID_11, 10);
    action.run();
    std::stringstream ss;
    ss << static_cast<int>(FSDataID::ID_11);
    EXPECT_EQ("4,EVENT_TO_SEND,1;" + ss.str() + ",22;", Serial.getDataStr());
    FSState::getInstance().clear();
}

TEST_F(FSActionTest, test_send_increment_value_limits_min)
{
    FSState::getInstance().setValue(FSDataID::ID_11, 3);
    FSActionSendIncrementValue action(*(comm_.get()), FSDataID::ID_11, -1, 
                                     FSActionValueMinMax(FSActionValueMinMax::Type::MIN_DEFINED, 1));
    action.run();
    std::stringstream ss;
    ss << static_cast<int>(FSDataID::ID_11);
    EXPECT_EQ(ss.str() + ",2;", Serial.getDataStr());
    FSState::getInstance().setValue(FSDataID::ID_11, 2);

    action.run();
    EXPECT_EQ(ss.str() + ",1;", Serial.getDataStr());
    FSState::getInstance().setValue(FSDataID::ID_11, 1);

    action.run();
    EXPECT_EQ("", Serial.getDataStr());

    FSState::getInstance().clear();
}

TEST_F(FSActionTest, test_send_increment_value_limits_max)
{
    FSState::getInstance().setValue(FSDataID::ID_11, 3);
    FSActionSendIncrementValue action(*(comm_.get()), FSDataID::ID_11, 1, 
                                     FSActionValueMinMax(FSActionValueMinMax::Type::MAX_DEFINED, -1, 5));
    action.run();
    std::stringstream ss;
    ss << static_cast<int>(FSDataID::ID_11);
    EXPECT_EQ(ss.str() + ",4;", Serial.getDataStr());
    FSState::getInstance().setValue(FSDataID::ID_11, 4);

    action.run();
    EXPECT_EQ(ss.str() + ",5;", Serial.getDataStr());
    FSState::getInstance().setValue(FSDataID::ID_11, 5);

    action.run();
    EXPECT_EQ("", Serial.getDataStr());

    FSState::getInstance().clear();
}

TEST_F(FSActionTest, test_send_increment_value_limits_min_max)
{
    FSState::getInstance().setValue(FSDataID::ID_11, 3);
    FSActionSendIncrementValue actionadd(*(comm_.get()), FSDataID::ID_11, 1, 
                                     FSActionValueMinMax(FSActionValueMinMax::Type::MIN_MAX_DEFINED, 1, 5));

    FSActionSendIncrementValue actionremove(*(comm_.get()), FSDataID::ID_11, -1, 
                                     FSActionValueMinMax(FSActionValueMinMax::Type::MIN_MAX_DEFINED, 1, 5));
    actionadd.run();
    std::stringstream ss;
    ss << static_cast<int>(FSDataID::ID_11);
    EXPECT_EQ(ss.str() + ",4;", Serial.getDataStr());
    FSState::getInstance().setValue(FSDataID::ID_11, 4);

    actionadd.run();
    EXPECT_EQ(ss.str() + ",5;", Serial.getDataStr());
    FSState::getInstance().setValue(FSDataID::ID_11, 5);

    actionadd.run();
    EXPECT_EQ(ss.str() + ",1;", Serial.getDataStr());
    FSState::getInstance().setValue(FSDataID::ID_11, 1);

    actionadd.run();
    EXPECT_EQ(ss.str() + ",2;", Serial.getDataStr());
    FSState::getInstance().setValue(FSDataID::ID_11, 2);

    actionremove.run();
    EXPECT_EQ(ss.str() + ",1;", Serial.getDataStr());
    FSState::getInstance().setValue(FSDataID::ID_11, 1);

    actionremove.run();
    EXPECT_EQ(ss.str() + ",5;", Serial.getDataStr());
    FSState::getInstance().setValue(FSDataID::ID_11, 5);

    actionremove.run();
    EXPECT_EQ(ss.str() + ",4;", Serial.getDataStr());
    FSState::getInstance().setValue(FSDataID::ID_11, 4);

    FSState::getInstance().clear();
}

TEST_F(FSActionTest, test_send_reverse_bool_value)
{
    FSState::getInstance().setValue(FSDataID::ID_11, 0);
    FSActionSendReverseBoolValue action(*(comm_.get()), FSDataID::ID_11);
    action.run();
    std::stringstream ss;
    ss << static_cast<int>(FSDataID::ID_11);
    EXPECT_EQ(ss.str() + ",1;", Serial.getDataStr());

    FSState::getInstance().setValue(FSDataID::ID_11, 1);
    action.run();
    EXPECT_EQ(ss.str() + ",0;", Serial.getDataStr());
    FSState::getInstance().clear();
}

TEST_F(FSActionTest, test_send_reverse_2_bool_value)
{
    FSState::getInstance().setValue(FSDataID::ID_11, 0);
    FSState::getInstance().setValue(FSDataID::ID_12, 0);
    FSActionSendReverseBool2Values action(*(comm_.get()), FSDataID::ID_11, FSDataID::ID_12);
    action.run();
    std::stringstream ss;
    ss << static_cast<int>(FSDataID::ID_11);
    std::stringstream ss2;
    ss2 << static_cast<int>(FSDataID::ID_12);
    EXPECT_EQ(ss.str() + ",1;" + ss2.str() + ",1;", Serial.getDataStr());

    FSState::getInstance().setValue(FSDataID::ID_11, 1);
    FSState::getInstance().setValue(FSDataID::ID_12, 1);
    action.run();
    EXPECT_EQ(ss.str() + ",0;" + ss2.str() + ",0;", Serial.getDataStr());
    FSState::getInstance().clear();
}

TEST_F(FSActionTest, test_send_reverse_3_bool_value)
{
    FSState::getInstance().setValue(FSDataID::ID_11, 0);
    FSState::getInstance().setValue(FSDataID::ID_12, 0);
    FSState::getInstance().setValue(FSDataID::ID_10, 0);
    FSActionSendReverseBool3Values action(*(comm_.get()), FSDataID::ID_11, FSDataID::ID_12, FSDataID::ID_10);
    action.run();
    std::stringstream ss;
    ss << static_cast<int>(FSDataID::ID_11);
    std::stringstream ss2;
    ss2 << static_cast<int>(FSDataID::ID_12);
    std::stringstream ss3;
    ss3 << static_cast<int>(FSDataID::ID_10);
    EXPECT_EQ(ss.str() + ",1;" + ss2.str() + ",1;" + ss3.str() + ",1;", Serial.getDataStr());

    FSState::getInstance().setValue(FSDataID::ID_11, 1);
    FSState::getInstance().setValue(FSDataID::ID_12, 1);
    FSState::getInstance().setValue(FSDataID::ID_10, 1);
    action.run();
    EXPECT_EQ(ss.str() + ",0;" + ss2.str() + ",0;" + ss3.str() + ",0;", Serial.getDataStr());
    FSState::getInstance().clear();
}

TEST_F(FSActionTest, test_send_reverse_bool_events)
{
    FSState::getInstance().setValue(FSDataID::ID_11, 0);
    FSActionSendReverseBoolEvents action(*(comm_.get()), FSDataID::ID_11, F("EVENT_NAME ON"), F("EVENT_NAME OFF"));
    action.run();
    std::stringstream ss;
    ss << static_cast<int>(FSDataID::ID_11);
    EXPECT_EQ("4,EVENT_NAME ON;", Serial.getDataStr());

    FSState::getInstance().setValue(FSDataID::ID_11, 1);
    action.run();
    EXPECT_EQ("4,EVENT_NAME OFF;", Serial.getDataStr());
    FSState::getInstance().clear();
}

TEST_F(FSActionTest, test_send_increment_value_events)
{
    FSState::getInstance().setValue(FSDataID::ID_11, 3);
    FSActionSendIncrementValueEvent action(*(comm_.get()), FSDataID::ID_11, 1, F("EVENT_NAME"));
    action.run();
    EXPECT_EQ("4,EVENT_NAME,4;", Serial.getDataStr());
    FSState::getInstance().setValue(FSDataID::ID_11, 4);

    action.run();
    EXPECT_EQ("4,EVENT_NAME,5;", Serial.getDataStr());
    FSState::getInstance().setValue(FSDataID::ID_11, 5);

    action.run();
    EXPECT_EQ("4,EVENT_NAME,6;", Serial.getDataStr());

    FSState::getInstance().clear();
}

TEST_F(FSActionTest, test_emulate)
{
    FSState::getInstance().setValue(FSDataID::ID_11, 3);
    FSActionEmulate action(*(comm_.get()), F("BUTTON_1"), F("PRESS"));
    action.run();
    EXPECT_EQ("1,EMULATE,2_1_9,BUTTON_1,PRESS;", Serial.getDataStr());

    FSState::getInstance().clear();
}


} // namespace test_spaduino
