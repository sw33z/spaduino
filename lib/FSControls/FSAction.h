#pragma once

#include "FSDataID.h"

#include <stdint.h>

class CmdMessenger;
class __FlashStringHelper;

namespace spaduino
{
class FSAction
{
public:
    FSAction(CmdMessenger & comm);
    virtual ~FSAction();

    virtual void run() = 0;

protected:
    CmdMessenger & comm_;
};

class FSActionSendEvents : public FSAction
{
public:
    template <typename ... Events>
    FSActionSendEvents(CmdMessenger & comm, const Events*... events)
        : FSAction(comm)
    {
        for (auto i = 0; i < MAX_EVENTS; ++i) 
        {
            events_[i] = nullptr;
        }
        addEvents(events...);
    };

    virtual ~FSActionSendEvents();

    void run() override;

private:
    template <typename SINGLE_EVENT, typename ... Events>
    void addEvents(SINGLE_EVENT event, Events ... events)
    {
        if (nb_events_ < MAX_EVENTS)
        {
            events_[nb_events_] = event;
            nb_events_++;
            addEvents(events...);
        }
    }

    void addEvents()
    {
    }

    static const int MAX_EVENTS = 5;
    int nb_events_ = 0;
    const __FlashStringHelper *events_[MAX_EVENTS];
};

class FSActionSendEvent : public FSAction
{
public:
    FSActionSendEvent(CmdMessenger & comm, const __FlashStringHelper * event);
    virtual ~FSActionSendEvent();

    virtual void run() override;

protected:
    const __FlashStringHelper * event_;
};

class FSActionSend2Events : public FSActionSendEvent
{
public:
    FSActionSend2Events(CmdMessenger & comm, const __FlashStringHelper * event1, const __FlashStringHelper * event2);
    virtual ~FSActionSend2Events();

    virtual void run() override;

private:
    const __FlashStringHelper * event2_;
};

class FSActionSend3Events : public FSActionSend2Events
{
public:
    FSActionSend3Events(CmdMessenger & comm, const __FlashStringHelper * event1, const __FlashStringHelper * event2,  const __FlashStringHelper * event3);
    virtual ~FSActionSend3Events();

    virtual void run() override;

private:
    const __FlashStringHelper * event3_;
};


class FSActionSendValue : public FSAction
{
public:
    FSActionSendValue(CmdMessenger & comm, FSDataID id, int32_t value);
    virtual ~FSActionSendValue();

    virtual void run() override;
protected: 
    FSDataID id_;
    int32_t value_;
};


class FSActionSendReferenceValue : public FSAction
{
public:
    FSActionSendReferenceValue(CmdMessenger & comm, FSDataID id);
    virtual ~FSActionSendReferenceValue();

    virtual void run() override;
protected: 
    FSDataID id_;
};

class FSActionValueMinMax
{
public:
    enum class Type
    {
        MIN_DEFINED,
        MAX_DEFINED,
        MIN_MAX_DEFINED,
        NONE
    };
    FSActionValueMinMax(Type type, int32_t min = 0, int32_t max = 0);
    ~FSActionValueMinMax();

    Type getType() const;
    int32_t getMin() const;
    int32_t getMax() const;

    Type type_ = Type::NONE;
    int32_t min_ = 0;
    int32_t max_ = 0;
};

class FSActionSendIncrementValue : public FSAction
{
public:
    FSActionSendIncrementValue(CmdMessenger & comm, FSDataID id, int32_t increment);
    FSActionSendIncrementValue(CmdMessenger & comm, FSDataID id, int32_t increment, FSActionValueMinMax limits);
    virtual ~FSActionSendIncrementValue();

    virtual void run() override;
protected: 
    FSDataID id_;
    int32_t increment_;
    FSActionValueMinMax limits_;
};

class FSActionSetValueAndSendIncrementValue : public FSActionSendIncrementValue
{
public:
    FSActionSetValueAndSendIncrementValue(CmdMessenger & comm, FSDataID idOne, int32_t valueOne, FSDataID idTwo, int32_t increment);
    FSActionSetValueAndSendIncrementValue(CmdMessenger & comm, FSDataID idOne, int32_t valueOne, FSDataID idTwo, int32_t increment, FSActionValueMinMax limits);
    virtual ~FSActionSetValueAndSendIncrementValue();

    virtual void run() override;
protected: 
    FSDataID idOne_;
    int32_t value_;
};

class FSActionSendEventAndIncrementValue : public FSActionSendIncrementValue
{
public:
    FSActionSendEventAndIncrementValue(CmdMessenger & comm, const __FlashStringHelper * event, FSDataID id, int32_t increment);
    FSActionSendEventAndIncrementValue(CmdMessenger & comm, const __FlashStringHelper * event, FSDataID id, int32_t increment, FSActionValueMinMax limits);
    virtual ~FSActionSendEventAndIncrementValue();

    virtual void run() override;
protected:
    const __FlashStringHelper * event_;
};

class FSActionSendIncrementValueEvent : public FSAction
{
public:
    FSActionSendIncrementValueEvent(CmdMessenger & comm, FSDataID id, int32_t increment, const __FlashStringHelper * event);
    virtual ~FSActionSendIncrementValueEvent();

    virtual void run() override;
protected: 
    FSDataID id_;
    int32_t increment_;
    const __FlashStringHelper * event_;
};

class FSActionSendReverseBoolValue : public FSAction
{
public:
    FSActionSendReverseBoolValue(CmdMessenger & comm, FSDataID id);
    virtual ~FSActionSendReverseBoolValue();

    virtual void run() override;
protected: 
    FSDataID id_;
};

class FSActionSendReverseBool2Values : public FSActionSendReverseBoolValue
{
public:
    FSActionSendReverseBool2Values(CmdMessenger & comm, FSDataID id1, FSDataID id2);
    virtual ~FSActionSendReverseBool2Values();

    virtual void run() override;
protected: 
    FSDataID id2_;
};

class FSActionSendReverseBool3Values : public FSActionSendReverseBool2Values
{
public:
    FSActionSendReverseBool3Values(CmdMessenger & comm, FSDataID id1, FSDataID id2, FSDataID id3);
    virtual ~FSActionSendReverseBool3Values();

    virtual void run() override;
protected: 
    FSDataID id3_;
};

class FSActionSendReverseBoolEvents : public FSAction
{
public:
    FSActionSendReverseBoolEvents(CmdMessenger & comm, FSDataID id, const __FlashStringHelper * eventON, const __FlashStringHelper * eventOFF);
    virtual ~FSActionSendReverseBoolEvents();

    virtual void run() override;
protected: 
    FSDataID id_;
    const __FlashStringHelper * eventON_;
    const __FlashStringHelper * eventOFF_;
};

class FSActionEmulate : public FSAction
{
public:
    FSActionEmulate(CmdMessenger & comm, const __FlashStringHelper * button, const __FlashStringHelper * action);
    virtual ~FSActionEmulate();

    virtual void run() override;
protected: 
    const __FlashStringHelper * button_;
    const __FlashStringHelper * action_;
};
} // namespace spaduino