#pragma once

#include <Arduino.h>

namespace spaduino
{

    namespace pins
    {
        constexpr int CONFIG_PIN_1 = A13;
        constexpr int CONFIG_PIN_2 = A14;
        constexpr int CONFIG_PIN_3 = A15;

        constexpr int LED = 33;

        constexpr int DISPLAY_CLK = 7;
        constexpr int DISPLAY_DIO = 8;

        constexpr int PUSH_BUTTON = 45;

        constexpr int ENCODER_PIN_1 = 24;
        constexpr int ENCODER_PIN_2 = 26;
        constexpr int ENCODER_BUTTON = 28;

        constexpr int ON_OFF_SWITCH = 37;
    } // namespace pins

} // namespace spaduino