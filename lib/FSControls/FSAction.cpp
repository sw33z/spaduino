#include "FSAction.h"

#include "SpadNextCmd.h"
#include "FSState.h"

#include <CmdMessenger.h>

namespace spaduino
{

FSAction::FSAction(CmdMessenger & comm)
    : comm_(comm)
{
}

FSAction::~FSAction()
{
}

// --------------------------------------------------

FSActionSendEvent::FSActionSendEvent(CmdMessenger & comm, const __FlashStringHelper * event)
    : FSAction(comm)
    , event_(event)
{
}

FSActionSendEvent::~FSActionSendEvent()
{
}

void FSActionSendEvent::run()
{
    comm_.sendCmd(static_cast<byte>(SpadNextCmd::SPADNEXT_CMD_SIM_EVENT),event_);
}

// --------------------------------------------------

FSActionSendEvents::~FSActionSendEvents()
{
}

void FSActionSendEvents::run()
{
    auto event_index = 0;
    const __FlashStringHelper * event;
    while ((event = events_[event_index]) != nullptr && event_index < MAX_EVENTS)
    {
        comm_.sendCmd(static_cast<byte>(SpadNextCmd::SPADNEXT_CMD_SIM_EVENT),event);
        event_index++;
    }
}

// --------------------------------------------------

FSActionSend2Events::FSActionSend2Events(CmdMessenger & comm, const __FlashStringHelper * event1, const __FlashStringHelper * event2)
    : FSActionSendEvent(comm, event1)
    , event2_(event2)
{
}

FSActionSend2Events::~FSActionSend2Events()
{
}

void FSActionSend2Events::run()
{
    FSActionSendEvent::run();
    comm_.sendCmd(static_cast<byte>(SpadNextCmd::SPADNEXT_CMD_SIM_EVENT),event2_);
}

// --------------------------------------------------

FSActionSend3Events::FSActionSend3Events(CmdMessenger & comm, const __FlashStringHelper * event1, const __FlashStringHelper * event2,  const __FlashStringHelper * event3)
    : FSActionSend2Events(comm, event1, event2)
    , event3_(event3)
{
}

FSActionSend3Events::~FSActionSend3Events()
{
}

void FSActionSend3Events::run()
{
    FSActionSend2Events::run();
    comm_.sendCmd(static_cast<byte>(SpadNextCmd::SPADNEXT_CMD_SIM_EVENT),event3_);
}

// --------------------------------------------------

FSActionSendValue::FSActionSendValue(CmdMessenger & comm, FSDataID id, int32_t value)
    : FSAction(comm)
    , id_(id)
    , value_(value)
{
}

FSActionSendValue::~FSActionSendValue()
{
}

void FSActionSendValue::run()
{
    comm_.sendCmd(static_cast<byte>(id_),value_);
}

// --------------------------------------------------


FSActionSendReferenceValue::FSActionSendReferenceValue(CmdMessenger & comm, FSDataID id)
    : FSAction(comm)
    , id_(id)
{
}

FSActionSendReferenceValue::~FSActionSendReferenceValue()
{
}

void FSActionSendReferenceValue::run()
{
    comm_.sendCmd(static_cast<byte>(id_),FSState::getInstance().getValue(id_));
}

// --------------------------------------------------

FSActionValueMinMax::FSActionValueMinMax(FSActionValueMinMax::Type type, int32_t min, int32_t max)
    : type_(type)
    , min_(min)
    , max_(max)
{
}

FSActionValueMinMax::~FSActionValueMinMax()
{
}

FSActionValueMinMax::Type FSActionValueMinMax::getType() const
{
    return type_;
}

int32_t FSActionValueMinMax::getMin() const
{
    return min_;
}

int32_t FSActionValueMinMax::getMax() const
{
    return max_;
}

// --------------------------------------------------

FSActionSendIncrementValue::FSActionSendIncrementValue(CmdMessenger & comm, FSDataID id, int32_t increment)
    : FSAction(comm)
    , id_(id)
    , increment_(increment)
    , limits_(FSActionValueMinMax(FSActionValueMinMax::Type::NONE))
{
}

FSActionSendIncrementValue::FSActionSendIncrementValue(CmdMessenger & comm, FSDataID id, int32_t increment, FSActionValueMinMax limits)
    : FSAction(comm)
    , id_(id)
    , increment_(increment)
    , limits_(limits)
{
}

FSActionSendIncrementValue::~FSActionSendIncrementValue()
{
}

void FSActionSendIncrementValue::run()
{
    switch (limits_.getType())
    {
        case FSActionValueMinMax::Type::NONE:
            comm_.sendCmd(static_cast<byte>(id_),FSState::getInstance().getValue(id_)+increment_);
        break;
        case FSActionValueMinMax::Type::MIN_DEFINED:
        {
            int32_t calculatedValue = FSState::getInstance().getValue(id_)+increment_;
            if (calculatedValue >= limits_.getMin())
            {
                comm_.sendCmd(static_cast<byte>(id_),calculatedValue);
            }
        } 
        break;
        case FSActionValueMinMax::Type::MAX_DEFINED:
        {
            int32_t calculatedValue = FSState::getInstance().getValue(id_)+increment_;
            if (calculatedValue <= limits_.getMax())
            {
                comm_.sendCmd(static_cast<byte>(id_),calculatedValue);
            }
        } 
        break;
        case FSActionValueMinMax::Type::MIN_MAX_DEFINED:
        {
            int32_t calculatedValue = FSState::getInstance().getValue(id_)+increment_;
            if ( (limits_.getMin() <= calculatedValue) && (calculatedValue <= limits_.getMax()) )
            {
                comm_.sendCmd(static_cast<byte>(id_),calculatedValue);
            }
            else if (limits_.getMin() > calculatedValue)
            {
                calculatedValue = limits_.getMax();
                comm_.sendCmd(static_cast<byte>(id_),calculatedValue);
            }
            else if (calculatedValue > limits_.getMax())
            {
                calculatedValue = limits_.getMin();
                comm_.sendCmd(static_cast<byte>(id_),calculatedValue);
            }
        } 
        break;
    }
}

// --------------------------------------------------

FSActionSetValueAndSendIncrementValue::FSActionSetValueAndSendIncrementValue(CmdMessenger & comm, 
                                                                             FSDataID idOne, 
                                                                             int32_t valueOne, 
                                                                             FSDataID idTwo, 
                                                                             int32_t increment)
    : FSActionSendIncrementValue(comm, idTwo, increment)
    , idOne_(idOne)
    , value_(valueOne)
{
}

FSActionSetValueAndSendIncrementValue::FSActionSetValueAndSendIncrementValue(CmdMessenger & comm, 
                                                                             FSDataID idOne, 
                                                                             int32_t valueOne, 
                                                                             FSDataID idTwo, 
                                                                             int32_t increment, 
                                                                             FSActionValueMinMax limits)
    : FSActionSendIncrementValue(comm, idTwo, increment, limits)
    , idOne_(idOne)
    , value_(valueOne)
{
}

FSActionSetValueAndSendIncrementValue::~FSActionSetValueAndSendIncrementValue()
{

}

void FSActionSetValueAndSendIncrementValue::run()
{
    comm_.sendCmd(static_cast<byte>(idOne_),value_);
    FSActionSendIncrementValue::run();
}

// --------------------------------------------------

FSActionSendEventAndIncrementValue::FSActionSendEventAndIncrementValue(CmdMessenger & comm, 
                                                                       const __FlashStringHelper * event, 
                                                                       FSDataID id, 
                                                                       int32_t increment)
    : FSActionSendIncrementValue(comm, id, increment)
    , event_(event)
{
}

FSActionSendEventAndIncrementValue::FSActionSendEventAndIncrementValue(CmdMessenger & comm, 
                                                                       const __FlashStringHelper * event, 
                                                                       FSDataID id, 
                                                                       int32_t increment, 
                                                                       FSActionValueMinMax limits)
    : FSActionSendIncrementValue(comm, id, increment, limits)
    , event_(event)
{
}

FSActionSendEventAndIncrementValue::~FSActionSendEventAndIncrementValue()
{
}

void FSActionSendEventAndIncrementValue::run()
{
    comm_.sendCmd(static_cast<byte>(SpadNextCmd::SPADNEXT_CMD_SIM_EVENT),event_);
    FSActionSendIncrementValue::run();
}

// --------------------------------------------------

FSActionSendIncrementValueEvent::FSActionSendIncrementValueEvent(CmdMessenger & comm, FSDataID id, int32_t increment, const __FlashStringHelper * event)
    : FSAction(comm)
    , id_(id)
    , increment_(increment)
    , event_(event)
{
}

FSActionSendIncrementValueEvent::~FSActionSendIncrementValueEvent()
{
}

void FSActionSendIncrementValueEvent::run()
{
    int32_t calculatedValue = FSState::getInstance().getValue(id_)+increment_;
    comm_.sendCmdStart(static_cast<byte>(SpadNextCmd::SPADNEXT_CMD_SIM_EVENT));
    comm_.sendCmdArg(event_);
    comm_.sendCmdArg(calculatedValue);
    comm_.sendCmdEnd();
}

// --------------------------------------------------

FSActionSendReverseBoolValue::FSActionSendReverseBoolValue(CmdMessenger & comm, FSDataID id)
    : FSAction(comm)
    , id_(id)
{
}

FSActionSendReverseBoolValue::~FSActionSendReverseBoolValue()
{
}

void FSActionSendReverseBoolValue::run()
{
    int32_t value = FSState::getInstance().getValue(id_);
    if (value == 0)
    {
        comm_.sendCmd(static_cast<byte>(id_),1);
    }
    else
    {
        comm_.sendCmd(static_cast<byte>(id_),0);
    }
}

// --------------------------------------------------

FSActionSendReverseBool2Values::FSActionSendReverseBool2Values(CmdMessenger & comm, FSDataID id1, FSDataID id2)
    : FSActionSendReverseBoolValue(comm, id1)
    , id2_(id2)
{
    
}

FSActionSendReverseBool2Values::~FSActionSendReverseBool2Values()
{
}

void FSActionSendReverseBool2Values::run()
{
    FSActionSendReverseBoolValue::run();
    int32_t value2 = FSState::getInstance().getValue(id2_);
    if (value2 == 0)
    {
        comm_.sendCmd(static_cast<byte>(id2_),1);
    }
    else
    {
        comm_.sendCmd(static_cast<byte>(id2_),0);
    }
}

// --------------------------------------------------

FSActionSendReverseBool3Values::FSActionSendReverseBool3Values(CmdMessenger & comm, FSDataID id1, FSDataID id2, FSDataID id3)
    : FSActionSendReverseBool2Values(comm, id1, id2)
    , id3_(id3)
{
}

FSActionSendReverseBool3Values::~FSActionSendReverseBool3Values()
{

}

void FSActionSendReverseBool3Values::run()
{
    FSActionSendReverseBool2Values::run();
    int32_t value3 = FSState::getInstance().getValue(id3_);
    if (value3 == 0)
    {
        comm_.sendCmd(static_cast<byte>(id3_),1);
    }
    else
    {
        comm_.sendCmd(static_cast<byte>(id3_),0);
    }
}

// --------------------------------------------------

FSActionSendReverseBoolEvents::FSActionSendReverseBoolEvents(CmdMessenger & comm, FSDataID id, const __FlashStringHelper * eventON, const __FlashStringHelper * eventOFF)
    : FSAction(comm)
    , id_(id)
    , eventON_(eventON)
    , eventOFF_(eventOFF)
{
}

FSActionSendReverseBoolEvents::~FSActionSendReverseBoolEvents()
{
}

void FSActionSendReverseBoolEvents::run()
{
    int32_t value = FSState::getInstance().getValue(id_);
    if (value == 1)
    {
        comm_.sendCmd(static_cast<byte>(SpadNextCmd::SPADNEXT_CMD_SIM_EVENT),eventOFF_);
    }
    else
    {
        comm_.sendCmd(static_cast<byte>(SpadNextCmd::SPADNEXT_CMD_SIM_EVENT),eventON_);
    }
}

// --------------------------------------------------

FSActionEmulate::FSActionEmulate(CmdMessenger & comm, const __FlashStringHelper * button, const __FlashStringHelper * action)
    : FSAction(comm)
    , button_(button)
    , action_(action)
{
}

FSActionEmulate::~FSActionEmulate()
{
}

void FSActionEmulate::run()
{
    comm_.sendCmdStart(static_cast<byte>(SpadNextCmd::SPADNEXT_CMD_1));
    comm_.sendCmdArg("EMULATE");
    comm_.sendCmdArg("2_1_9");
    comm_.sendCmdArg(button_);
    comm_.sendCmdArg(action_);
    comm_.sendCmdEnd();
}

} // namespace spaduino
