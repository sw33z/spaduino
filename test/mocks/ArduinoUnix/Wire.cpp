#include <Wire.h>


TwoWire::TwoWire()
{

}

TwoWire::~TwoWire()
{

}

// Wire & Wire::getInstance()
// {
//     if (instance_ == nullptr)
//     {
//         instance_ = new Wire();
//     }
//     return *instance_;
// }

void TwoWire::begin(int /*address*/)
{

}

void TwoWire::onReceive(void (*callback)(int))
{
    callback(dataAvailableChannel1_);
}

void TwoWire::beginTransmission(int /*address*/)
{

}

void TwoWire::endTransmission()
{

}

void TwoWire::send(const uint8_t* /*data*/, size_t /*length*/)
{

}

int TwoWire::available()
{
    return 0;
}

int TwoWire::read()
{
    return 0;
}

TwoWire Wire = TwoWire();