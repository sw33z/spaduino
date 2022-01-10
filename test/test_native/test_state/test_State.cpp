#include <gtest/gtest.h>

#include <FSState.h>

using namespace spaduino;

namespace test_state
{

TEST (FSState, test_init)
{
    FSState & state = FSState::getInstance();
    for (uint32_t i = MIN_SPAD_NEXT_ID; i < MAX_SPAD_NEXT_ID; ++i)
    {
        EXPECT_EQ(0, state.getValue(static_cast<FSDataID>(i)));
    }
    FSState::getInstance().clear();
}

TEST (FSState, test_set_values)
{
    FSState & state = FSState::getInstance();
    state.setValue(FSDataID::ID_11, 123);
    EXPECT_EQ(123, state.getValue(FSDataID::ID_11));

    EXPECT_EQ(0, state.getValue(FSDataID::ID_10));
    state.setValue(FSDataID::ID_10, 12345);
    EXPECT_EQ(12345, state.getValue(FSDataID::ID_10));
    FSState::getInstance().clear();
}

TEST (FSState, test_set_get_non_valid_ids)
{
    FSState & state = FSState::getInstance();
    state.setValue(FSDataID::FIRST_GREATER_NON_VALID_ID, 123);
    EXPECT_EQ(0, state.getValue(FSDataID::FIRST_GREATER_NON_VALID_ID));

    state.setValue(FSDataID::FIRST_LESS_NON_VALID_ID, 123);
    EXPECT_EQ(0, state.getValue(FSDataID::FIRST_LESS_NON_VALID_ID));
    FSState::getInstance().clear();
}

} // namespace test_state