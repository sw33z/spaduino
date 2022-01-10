#pragma once

constexpr unsigned int DEFAULT_BIT_DELAY = 100;
constexpr unsigned int DEFAULT_SCROLL_DELAY = 100;

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

    TM1637(uint8_t pinClk, uint8_t pinDIO, NumDigits numDigits, unsigned int bitDelay = DEFAULT_BIT_DELAY, unsigned int scrollDelay = DEFAULT_SCROLL_DELAY);

    void setBrightness(Brightness brightness, bool on = true);
    void setScrolldelay(unsigned int scrollDelay = 200);
    void setSegments(const uint8_t segments[], uint8_t length = 0, uint8_t pos = 0);
    void clear();
    void showNumber(int num, bool leading_zero = false, uint8_t length = 0, uint8_t pos = 0);
    void showNumber(long num, bool leading_zero = false, uint8_t length = 0, uint8_t pos = 0);
    void showNumber(double num, uint8_t decimal_length = 0, uint8_t length = 0, uint8_t pos = 0);
    void showNumberDec(long num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 0, uint8_t pos = 0);
    void showNumberHex(uint16_t num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 0, uint8_t pos = 0);
    uint8_t encodeDigit(uint8_t digit);
    void showString(const char s[], uint8_t length = 0, uint8_t pos = 0, uint8_t dots = 0);
    void showString_P(const char s[], uint8_t length = 0, uint8_t pos = 0, uint8_t dots = 0);
    void showLevel(unsigned int level = 100, bool horizontal = true);
    void showAnimation(const uint8_t **data, unsigned int frames = 0, unsigned int ms = 10);
    void showAnimation_P(const uint8_t **data, unsigned int frames = 0, unsigned int ms = 10);
    uint8_t encodeASCII(uint8_t chr);

protected:
    void bitDelay();
    void start();
    void stop();
    bool writeByte(uint8_t b);
    void showDots(uint8_t dots, uint8_t* digits);
    void showNumberBaseEx(int8_t base, uint32_t num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);

private:
    void checkLength(uint8_t & length);
    NumDigits m_numDigits;
    uint8_t m_pinClk;
    uint8_t m_pinDIO;
    unsigned int m_bitDelay;
    unsigned int m_scrollDelay;
    uint8_t m_brightness;
};
