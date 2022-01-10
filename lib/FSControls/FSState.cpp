#include "FSState.h"

namespace spaduino
{
    constexpr uint32_t MAX_STATE_VALUES = MAX_SPAD_NEXT_ID - MIN_SPAD_NEXT_ID;

    struct FSStateImpl
    {
        void clear()
        {
            for(uint32_t i = 0; i < MAX_STATE_VALUES; ++i)
            {
                values_[i] = 0;
            }
        }

        FSStateImpl()
        {
            clear();
        }

        ~FSStateImpl() = default;

        uint32_t getInternalID(FSDataID id) const
        {
            return static_cast<uint32_t>(id) - MIN_SPAD_NEXT_ID;
        }

        void setValue(FSDataID id, int32_t value)
        {
            uint32_t internalID = getInternalID(id);
            if (internalID >= MAX_STATE_VALUES)
                return;

            values_[internalID] = value;
        }

        int32_t getValue(FSDataID id) const
        {
            uint32_t internalID = getInternalID(id);
            if (internalID >= MAX_STATE_VALUES)
                return 0;
            
            return values_[internalID];
        }

        int32_t values_[MAX_STATE_VALUES];
    };

// ---------------------------------------------------------------------------

FSState * FSState::instance_ = nullptr;

FSState::FSState()
    : pimpl_(new FSStateImpl())
{
}

FSState::~FSState()    // LCOV_EXCL_LINE
{
}                      // LCOV_EXCL_LINE

FSState & FSState::getInstance()
{
    if (!instance_)
    {
        instance_ = new FSState();
    }
    return *instance_;
}

void FSState::setValue(FSDataID id, int32_t value)
{
    pimpl_->setValue(id, value);
}

int32_t FSState::getValue(FSDataID id) const
{
    return pimpl_->getValue(id);
}

void FSState::clear()
{
    pimpl_->clear();
}

} // namspace spaduino