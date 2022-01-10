#pragma once

#include "FSDataID.h"

#include <WString.h>
#include <CmdMessenger.h>

#include "unique_ptr.h"
#include "shared_ptr.h"

class CmdMessenger;

namespace spaduino
{

class FSObject;
struct FSControlsImpl;

class FSControls 
{
public:
    FSControls(const __FlashStringHelper * name,  CmdMessenger & comm);
    virtual ~FSControls();

    void addDisplayObject(std::shared_ptr<FSObject> obj);
    void addInputObject(std::shared_ptr<FSObject> obj);
    void loopOnce();

    void subscribe(const __FlashStringHelper * variable, FSDataID id);

private:
    arduino::memory::unique_ptr<FSControlsImpl> pimpl_;
};

} // namespace spaduino