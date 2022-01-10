#include "TM1637.h"

#include <iostream>

TM1637 * TM1637::instance_ = nullptr;

TM1637::TM1637(int /*pin1*/, int /*pin2*/, NumDigits numDigits)
 : numDigits_(static_cast<uint8_t>(numDigits))
{
    while(value_.length() < numDigits_)
    {
        value_ = std::string(" ") + value_;
    }
}

TM1637 * TM1637::getInstance(NumDigits numDigits)
{
    if (instance_ == nullptr)
    {
        instance_ = new TM1637(1,2,numDigits);
    }
    return instance_;
}

void TM1637::resetInstance()
{
    instance_ = nullptr;
}

void TM1637::showNumber(int number, bool padding)
{
    if (number > 999999 || number < -99999)
        return;

    std::cout << "DISPLAY: " << number << " ****************************** " << std::endl;
    char format[16];
    if (padding)
    {
        format[0] = '%';
        format[1] = '0';
        format[2] = (char)(numDigits_ + 48);
        format[3] = 'd';
        format[4] = '\0';
    }
    else
    {
        format[0] = '%';
        format[1] = 'd';
        format[2] = '\0';
    }
    char strValue_[16];
    sprintf(strValue_, format, number);
    std::cout << "CALCULATED: " << strValue_ << std::endl;
    value_ = strValue_;
    while(value_.length() < numDigits_)
    {
        value_ = std::string(" ") + value_;
    }
}

void TM1637::setBrightness(Brightness)
{
}

std::string TM1637::getShownValue() const
{
    return value_;
}

void TM1637::showString(const char s[], uint8_t length)
{
    value_ = std::string(s, length);
}