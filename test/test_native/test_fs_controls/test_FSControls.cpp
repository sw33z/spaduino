#include <gtest/gtest.h>
#include "CmdMessengerTestBase.h"

#include <FSControls.h>

#include <SerialMock.h>

using namespace spaduino;

namespace test_spaduino
{
class FSControlsTest: public spaduino::test::CmdMessengerTestBase
{
    public:
        FSControlsTest()
            : spaduino::test::CmdMessengerTestBase()
        {
        }
        ~FSControlsTest() = default;
};

TEST_F(FSControlsTest, test_subscribe)
{
    FSControls fscontrols(F("FSCONTROLS"), *(comm_.get()));
    fscontrols.subscribe(F("AP ALTITUDE VAR"), FSDataID::ID_13);
    fscontrols.subscribe(F("AP COURSE VAR"), FSDataID::ID_16);
    Serial.print("0,CONFIG;");
    comm_->feedinSerialData();
    EXPECT_EQ("1,SUBSCRIBE,13,AP ALTITUDE VAR;1,SUBSCRIBE,16,AP COURSE VAR;0,CONFIG;", Serial.getDataStrTestBuffer());
}

TEST_F(FSControlsTest, test_init_protocol)
{
    FSControls fscontrols(F("FSCONTROLS"), *(comm_.get()));
    Serial.print("0,INIT;");
    comm_->feedinSerialData();
    EXPECT_EQ("0,SPAD,FSCONTROLS,Ardupilot;", Serial.getDataStrTestBuffer());
}

TEST_F(FSControlsTest, test_ping)
{
    FSControls fscontrols(F("FSCONTROLS"), *(comm_.get()));
    Serial.print("0,PING,12345;");
    comm_->feedinSerialData();
    EXPECT_EQ("0,PONG,12345;", Serial.getDataStrTestBuffer());
}

} // namespace test_spaduino