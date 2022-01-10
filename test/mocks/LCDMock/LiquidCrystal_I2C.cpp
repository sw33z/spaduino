#include "LiquidCrystal_I2C.h"
#include <stdint.h>

std::string convertToString(char* a, int size);

LiquidCrystal_I2C::LiquidCrystal_I2C()
{
    clear();
}

void LiquidCrystal_I2C::begin(int, int)
{
    // does nothing
}

void LiquidCrystal_I2C::clear()
{
    for (int i = 0; i < 16; ++i)
    {
        row1_[i] = ' ';
        row2_[i] = ' ';
    }
}

void LiquidCrystal_I2C::setCursor(int col, int row)
{
    col_cursor_ = col;
    row_cursor_ = row;
}

void LiquidCrystal_I2C::print(const char * text)
{
    char *row = row1_;

    if (row_cursor_ == 1)
    {
        row = row2_;
    }

    std::string aux(text);
    for (unsigned long i = 0; i < aux.length(); ++i)
    {
        if (col_cursor_ < 16)
        {
            row[col_cursor_++] = aux[i];
        }
    }
}

std::string convertToString(char* a, int size) 
{ 
    int i; 
    std::string s = ""; 
    for (i = 0; i < size; i++) { 
        s = s + a[i]; 
    } 
    return s; 
}

std::string LiquidCrystal_I2C::getRow1()
{
    return convertToString(row1_, 16);
}

std::string LiquidCrystal_I2C::getRow2()
{
    return convertToString(row2_, 16);
}