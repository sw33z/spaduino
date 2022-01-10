#include "Input.h"
#include "FSAction.h"

namespace spaduino
{
namespace input
{

struct InputImpl
{
    InputImpl(spaduino::FSDataID id);
    void sendCommand( const FSCommand & cmd );
    spaduino::FSDataID id_;
};

InputImpl::InputImpl(FSDataID id)
    : id_(id)
{
}

// ----------------------------------------------------------------------------

Input::Input(FSDataID id)
    : FSObject(id)
    , pimpl_(new InputImpl(id))
{
}

Input::~Input()
{
}

void Input::runAction(std::shared_ptr<FSAction> action)
{
    if (action && sendDataAllowed_)
    {
        action->run();
    }
}


} // namespace input
} // namespace spaduino