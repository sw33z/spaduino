#pragma once

#include <string>

class LiquidCrystal_I2C
{
public:
    LiquidCrystal_I2C();
    ~LiquidCrystal_I2C() = default;

    void begin(int cols = 16, int rows = 2);
    void clear();
    void setCursor(int col, int row);
    void print(const char * text);
    std::string getRow1();
    std::string getRow2();

private:
    char row1_[16];
    char row2_[16];
    int row_cursor_ = 0;
    int col_cursor_ = 0;
};