#pragma once

#include <gtest/gtest.h>

#include <CmdMessenger.h>
#include <FSDataID.h>
#include <FSState.h>

#include <iostream>

namespace spaduino
{
namespace test
{
class CmdMessengerTestBase: public ::testing::Test
{
    public:
        CmdMessengerTestBase() = default;
        ~CmdMessengerTestBase() = default;

        virtual void SetUp() override
        {
            comm_.reset(new CmdMessenger(Serial));
        }

        virtual void TearDown() override
        {
            mocks::SerialMock::resetInstance();
            FSState::getInstance().clear();
        }

    protected:
        void sendCmdToSerialAndProcess( spaduino::FSDataID id, int32_t value)
        {
            if (comm_ != nullptr)
            {
                std::stringstream ss;
                ss << static_cast<int>(id) << "," << value << ";";
                Serial.print(ss.str().c_str());
                comm_->feedinSerialData();
            }
        }
        void updateDataInState(spaduino::FSDataID id, int32_t value)
        {
            FSState::getInstance().setValue(id, value);
        }
        std::shared_ptr<CmdMessenger> comm_;
};

} // namespace test
} // namespace spaduino