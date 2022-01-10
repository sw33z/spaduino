#pragma once

#include "FSDataID.h"
#include "FSObject.h"

#include "unique_ptr.h"
#include "shared_ptr.h"

class CmdMessenger;
class __FlashStringHelper;

namespace spaduino
{

class FSAction;
struct FSCommand;

namespace input
{

struct InputImpl;

class Input : public FSObject
{
public:
    Input(spaduino::FSDataID id);
    virtual ~Input();
protected:
    void runAction(std::shared_ptr<FSAction> action);

private:
    arduino::memory::unique_ptr<InputImpl> pimpl_;
};

} // namespace input
} // namespace spaduino