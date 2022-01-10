#include "FSControls.h"

#include "SpadNextCmd.h"
#include "FSObject.h"
#include "FSState.h"

#include <CmdMessenger.h>

namespace spaduino
{

static const int MAX_OBJECTS = MAX_SPAD_NEXT_ID - MIN_SPAD_NEXT_ID;
static const int MAX_OBJECTS_SUBSCRIBED_TO_DATA = 5;

struct FSSubscribeItem
{
    FSSubscribeItem(const __FlashStringHelper * variable, FSDataID id)
        : variable_(variable)
        , id_(id)
    {}

    void subscribeInSpadNext(CmdMessenger & comm)
    {
        String path = String(F("subscriber/")) + String(static_cast<int>(id_));
/*
        // REMOVED BECASUE SPAD.NEXT COMPLAINS IN NEW VERSION.
        // LOOKS LIKE WHEN ADDING IT'S ALREADY SUBSCRIBING
        comm.sendCmdStart(static_cast<byte>(SpadNextCmd::SPADNEXT_CMD_1));
        comm.sendCmdArg(F("ADD"));
        comm.sendCmdArg(static_cast<int>(id_));
        comm.sendCmdArg(path);
        comm.sendCmdArg(F("S32"));
        comm.sendCmdArg(F("RW"));
        comm.sendCmdArg(variable_);
        comm.sendCmdEnd();
*/
        if (variable_ != nullptr)
        {
            comm.sendCmdStart(static_cast<byte>(SpadNextCmd::SPADNEXT_CMD_1));
            comm.sendCmdArg(F("SUBSCRIBE"));
            comm.sendCmdArg(static_cast<int>(id_));
            comm.sendCmdArg(variable_);
            comm.sendCmdEnd();
        } 
    }

    const __FlashStringHelper * variable_;
    FSDataID id_;
};

struct FSControlsImpl : public CmdSubscriber
{
    FSControlsImpl(const __FlashStringHelper * uuid, CmdMessenger & comm);
    ~FSControlsImpl();

    //virtual void registerInSpadNext() override;
    virtual void onData(uint8_t cmdID) override;

    void addDisplayObject(std::shared_ptr<spaduino::FSObject> obj);
    void addInputObject(std::shared_ptr<spaduino::FSObject> obj);

    void sendConfig();
    void sendPong();
    void sendSpad();

    void sendDataAllowed(bool allowed);
    void requestInputsInitialStates();

    void loopOnce();

    void subscribe(const __FlashStringHelper * variable, FSDataID id);

    const __FlashStringHelper * name_;
    CmdMessenger & comm_;
    std::shared_ptr<spaduino::FSObject> displays_[MAX_OBJECTS];
    std::shared_ptr<spaduino::FSObject> inputs_[MAX_OBJECTS];
    std::shared_ptr<FSSubscribeItem> subscreItems_[MAX_OBJECTS];

};

FSControlsImpl::FSControlsImpl(const __FlashStringHelper * uuid, CmdMessenger & comm)
    : CmdSubscriber()
    , name_(uuid)
    , comm_(comm)
{
    for (int i = 0; i < MAX_OBJECTS; ++i)
    {
        displays_[i] = nullptr;
        inputs_[i] = nullptr;
        subscreItems_[i] = nullptr;
    }
    comm_.attach(static_cast<byte>(SpadNextCmd::SPADNEXT_CMD_0), this);
    comm_.attach(static_cast<byte>(SpadNextCmd::SPADNEXT_CMD_2), this);
}

FSControlsImpl::~FSControlsImpl()
{
    /*
    for (int i = 0; i < MAX_OBJECTS; ++i)
    {
        if (displays_[i] != nullptr)
        {
            delete displays_[i];
            displays_[i] = nullptr;
        }
        if (inputs_[i] != nullptr)
        {
            delete inputs_[i];
            inputs_[i] = nullptr;
        }
    }*/
}

void FSControlsImpl::addDisplayObject(std::shared_ptr<spaduino::FSObject> obj)
{
    displays_[static_cast<int>(obj->getID())-MIN_SPAD_NEXT_ID] = obj;
}

void FSControlsImpl::addInputObject(std::shared_ptr<spaduino::FSObject> obj)
{
    inputs_[static_cast<int>(obj->getID())-MIN_SPAD_NEXT_ID] = obj;
}

void FSControlsImpl::onData(uint8_t cmdID)
{
    if (cmdID >= MIN_SPAD_NEXT_ID && cmdID <= MAX_SPAD_NEXT_ID)
    {
        // handle State
        int32_t value = comm_.readInt32Arg();
        FSState::getInstance().setValue(static_cast<FSDataID>(cmdID), value);
        return;
    }
    
    // handling of cmds 0 and 2
    char *request = comm_.readStringArg();
    if (strcmp(request, "INIT") == 0)
    {
        // CMD 0,INIT
        sendSpad();
    }
    else if (strcmp(request, "CONFIG") == 0)
    {
        // CMD 0,CONFIG
        sendConfig();
    }
    else if (strcmp(request, "PING") == 0)
    {
        // CMD 0,PING
        sendPong();
    }
    else if (strcmp(request, "START") == 0)
    {
        // CMD 2,START
        sendDataAllowed(true);
        requestInputsInitialStates();
    }
    else if (strcmp(request, "STOP") == 0)
    {
        // CMD 2,STOP
        sendDataAllowed(false);
    }
}

void FSControlsImpl::sendConfig()
{
#ifdef UNIT_TEST
    Serial.writeToTestBuffer(true);
#endif
    for (int i = 0; i < MAX_OBJECTS; ++i)
    {
        if (subscreItems_[i] != nullptr)
        {
            subscreItems_[i]->subscribeInSpadNext(comm_);
            comm_.attach(static_cast<byte>(subscreItems_[i]->id_), this);
        }
    }

    comm_.sendCmdStart(static_cast<byte>(SpadNextCmd::SPADNEXT_CMD_0));
    comm_.sendCmdArg(F("CONFIG"));
    comm_.sendCmdEnd();
#ifdef UNIT_TEST
    Serial.writeToTestBuffer(false);
#endif
}

void FSControlsImpl::sendPong()
{
#ifdef UNIT_TEST
    Serial.writeToTestBuffer(true);
#endif
    comm_.sendCmdStart(static_cast<byte>(SpadNextCmd::SPADNEXT_CMD_0));
    comm_.sendCmdArg(F("PONG"));
    comm_.sendCmdArg(comm_.readInt32Arg());
    comm_.sendCmdEnd();
#ifdef UNIT_TEST
    Serial.writeToTestBuffer(false);
#endif
}

void FSControlsImpl::sendSpad()
{
#ifdef UNIT_TEST
    Serial.writeToTestBuffer(true);
#endif
    comm_.sendCmdStart(static_cast<byte>(SpadNextCmd::SPADNEXT_CMD_0));
    comm_.sendCmdArg(F("SPAD"));
    comm_.sendCmdArg(name_); 
    comm_.sendCmdArg(F("Ardupilot"));
    comm_.sendCmdEnd();
#ifdef UNIT_TEST
    Serial.writeToTestBuffer(false);
#endif
}

void FSControlsImpl::sendDataAllowed(bool allowed)
{
    for (int i = 0; i < MAX_OBJECTS; ++i)
    {
        if (displays_[i] != nullptr)
        {
            displays_[i]->sendDataAllowed(allowed);
        }
        if (inputs_[i] != nullptr)
        {
            inputs_[i]->sendDataAllowed(allowed);
        }
    }
}

void FSControlsImpl::requestInputsInitialStates()
{
    for (int i = 0; i < MAX_OBJECTS; ++i)
    {
        if (inputs_[i] != nullptr)
        {
            inputs_[i]->requestState();
        }
    }
}

void FSControlsImpl::loopOnce()
{
    for (int i = 0; i < MAX_OBJECTS; ++i)
    {
        if (displays_[i] != nullptr)
        {
            displays_[i]->loopOnce();
        }
    }

    for (int i = 0; i < MAX_OBJECTS; ++i)
    {
        if (inputs_[i] != nullptr)
        {
            inputs_[i]->loopOnce();
        }
    }
}

void FSControlsImpl::subscribe(const __FlashStringHelper * variable, FSDataID id)
{
    std::shared_ptr<FSSubscribeItem> item(new FSSubscribeItem(variable, id));
    uint32_t internalID = static_cast<uint32_t>(id) - MIN_SPAD_NEXT_ID;
    subscreItems_[internalID] = item;
}

// ------------------------------------------------------------------------------

FSControls::FSControls(const __FlashStringHelper * name, CmdMessenger & comm)
    : pimpl_(new FSControlsImpl(name, comm))
{
}

FSControls::~FSControls()
{
}

void FSControls::addDisplayObject(std::shared_ptr<FSObject> obj)
{
    pimpl_->addDisplayObject(obj);
}

void FSControls::addInputObject(std::shared_ptr<FSObject> obj)
{
    pimpl_->addInputObject(obj);
}

void FSControls::loopOnce()
{
    pimpl_->loopOnce();
}

void FSControls::subscribe(const __FlashStringHelper * variable, FSDataID id)
{
    pimpl_->subscribe(variable, id);
}

} // namespace spaduino