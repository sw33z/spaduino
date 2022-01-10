#pragma once

#include <stdint.h>

#include "FSDataID.h"

#include "unique_ptr.h"
namespace spaduino
{

constexpr uint32_t MIN_SPAD_NEXT_ID = static_cast<uint32_t>(FSDataID::FIRST_LESS_NON_VALID_ID) + 1;
constexpr uint32_t MAX_SPAD_NEXT_ID = static_cast<uint32_t>(FSDataID::FIRST_GREATER_NON_VALID_ID);

struct FSStateImpl;
class FSState
{
public:
    FSState();
    ~FSState();

    static FSState & getInstance();

    void setValue(FSDataID id, int32_t value);
    int32_t getValue(FSDataID id) const;
    void clear();

private:
    arduino::memory::unique_ptr<FSStateImpl> pimpl_;
    static FSState *instance_;
};
} // namspace spaduino