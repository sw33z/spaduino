#include "SerialMock.h"

#include <list>

namespace mocks
{
SerialMock * SerialMock::instance_ = nullptr;

struct SerialMockImpl
{
    SerialMockImpl() = default;
    ~SerialMockImpl() = default;
    void begin(int) {};
    int available()
    {
        return static_cast<int>(buffer_.size());
    }

    void write(uint8_t data)
    {
        if (writeToTestBuffer_)
        {
            testBuffer_.push_back(data);
        }
        else
        {
            buffer_.push_back(data);
        }
    }

    void print(const char *data)
    {
        char * data_now = const_cast<char *>(data);
        while (*data_now != '\0')
        {
            if (writeToTestBuffer_)
            {
                testBuffer_.push_back(static_cast<uint8_t>(*data_now));
            }
            else
            {
                buffer_.push_back(static_cast<uint8_t>(*data_now));
            }
            data_now++;
        }
    }

    void print(char data)
    {
        if (writeToTestBuffer_)
        {
            testBuffer_.push_back(static_cast<uint8_t>(data));
        }
        else
        {
            buffer_.push_back(static_cast<uint8_t>(data));
        }
    }

    void print(int32_t data)
    {
        std::string s = std::to_string(data);
        for (size_t i = 0; i < s.length(); ++i)
        {
            if (writeToTestBuffer_)
            {
                testBuffer_.push_back(static_cast<uint8_t>(s[i]));
            }
            else
            {
                buffer_.push_back(static_cast<uint8_t>(s[i]));
            }
        }
    }

    uint8_t read()
    {
        if (buffer_.empty())
            return 0;
        uint8_t data = buffer_.front();
        buffer_.erase(buffer_.begin());
        return data;
    }

    uint8_t peek()
    {
        if (buffer_.empty())
            return 0;
        return buffer_.front();
    }

    bool writeToTestBuffer_ = false;
    std::list<uint8_t> buffer_;
    std::list<uint8_t> testBuffer_;
};

SerialMock & SerialMock::getInstance()
{
    if (instance_ == nullptr)
    {
        instance_ = new SerialMock();
    }
    return *instance_;
}

void SerialMock::resetInstance()
{
    instance_ = nullptr;
}

SerialMock::SerialMock()
    : pimpl_(new SerialMockImpl())
{}

SerialMock::~SerialMock()
{
    delete pimpl_;
    pimpl_ = nullptr;
}
void SerialMock::begin(int param)
{
    pimpl_->begin(param);
}

int SerialMock::available()
{
    return pimpl_->available();
}

size_t SerialMock::write(uint8_t data)
{
    pimpl_->write(data);
    return 1;
}

void SerialMock::print(const char *data)
{
    pimpl_->print(data);
}

void SerialMock::print(char data)
{
    pimpl_->print(data);
}

void SerialMock::print(int32_t data)
{
    pimpl_->print(data);
}

int SerialMock::read()
{
    return static_cast<int>(pimpl_->read());
}

std::string SerialMock::getDataStr()
{
    std::string ret;
    while(available())
    {
        ret += static_cast<char>(read());
    }
    return ret;
}

void SerialMock::flush()
{

}

int SerialMock::peek()
{
    return static_cast<int>(pimpl_->peek());
}

void SerialMock::writeToTestBuffer(bool test)
{
    pimpl_->writeToTestBuffer_ = test;
}

bool SerialMock::getWriteToTestBuffer() const
{
    return pimpl_->writeToTestBuffer_;
}

void SerialMock::clearTestBuffer()
{
    pimpl_->testBuffer_.clear();
}

std::string SerialMock::getDataStrTestBuffer()
{
    std::string ret;
    while(pimpl_->testBuffer_.size())
    {
        ret += static_cast<char>(pimpl_->testBuffer_.front());
        pimpl_->testBuffer_.erase(pimpl_->testBuffer_.begin());
    }
    return ret;
}


} // namespace mocks