#pragma once

#include <stdint.h>
#include <string>

#include "Stream.h"

namespace mocks
{

struct SerialMockImpl;

class SerialMock : public Stream
{
public:
    SerialMock();
    ~SerialMock();
    static SerialMock & getInstance();
    static void resetInstance();

    void begin(int);
    int available();
    size_t write(uint8_t data);
    void print(const char *data);
    void print(char data);
    void print(int32_t data);
    int read();
    std::string getDataStr();
    std::string getDataStrTestBuffer();
    void flush();
    int peek();
    void writeToTestBuffer(bool test);
    bool getWriteToTestBuffer() const;
    void clearTestBuffer();

private:
    SerialMockImpl *pimpl_ = nullptr;
    static SerialMock *instance_;
};

} // namespace spaduino

//#define Serial spaduino::mocks::SerialMock::getInstance()