#include "FSObject.h"
#include <WString.h>

namespace spaduino
{
FSObject::FSObject(FSDataID id)
    : id_(id)
{
}

FSObject::~FSObject()
{
}

FSDataID FSObject::getID() const
{
    return id_;
}

void FSObject::sendDataAllowed(bool allowed)
{
    sendDataAllowed_ = allowed;
}

void FSObject::requestState()
{

}

} // namespace spaduino