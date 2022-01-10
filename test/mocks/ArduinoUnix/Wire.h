#pragma once

#include <stdint.h>
#include <stdlib.h>

class TwoWire
{
public:
    TwoWire();
    ~TwoWire();

    //static Wire & getInstance();

    void begin(int address);
    void onReceive(void (*)(int));
    void beginTransmission(int address);
    void endTransmission();
    void send(const uint8_t* data, size_t length);
    int available();
    int read();

private:
    uint8_t channel1_[1024];
    uint8_t channel2_[1024];
    int dataAvailableChannel1_ = 0;
    int dataAvailableChannel2_ = 0;
    //static Wire *instance_;
};

extern TwoWire Wire;