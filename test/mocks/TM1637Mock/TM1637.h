#pragma once

#include <string>

class TM1637
{
public:
    enum class NumDigits
    {
        FOUR = 4,
        SIX = 6
    };

    enum class Brightness
    {
      BRIGHT_LOW  = 0x00,
      BRIGHT_0    = 0x00,
      BRIGHT_1    = 0x01,
      BRIGHT_2    = 0x02,
      BRIGHT_3    = 0x03,
      BRIGHT_4    = 0x04,
      BRIGHT_5    = 0x05,
      BRIGHT_6    = 0x06,
      BRIGHT_7    = 0x07,
      BRIGHT_HIGH = 0x0f
    };
    TM1637(int pin1, int pin2, NumDigits numDigits = NumDigits::SIX);
    ~TM1637() = default;

    void setBrightness(Brightness);
    void clearDisplay();

    static TM1637 * getInstance(NumDigits numDigits = NumDigits::SIX);
    static void resetInstance();

    void showNumber(int number, bool padding);
    void showString(const char s[], uint8_t length = 6);
    std::string getShownValue() const;
    void displayError();

private:
    std::string value_;
    uint8_t numDigits_ = 0;
    static TM1637 *instance_;
};