#pragma once

#include <FSDataID.h>
#include <CmdSubscriber.h>

#include <Arduino.h>
#include <WString.h>

class CmdMessenger;

namespace spaduino
{

class FSObject
{
public:
    FSObject(FSDataID id);
    virtual ~FSObject();

    FSDataID getID() const;

    virtual void loopOnce() = 0;

    void sendDataAllowed(bool allowed);
    virtual void requestState();
protected:
    FSDataID id_;
    bool sendDataAllowed_ = true;
};

}