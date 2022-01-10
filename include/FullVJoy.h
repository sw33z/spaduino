#pragma once

#include <config/FSConfig.h>

namespace spaduino
{

// Define button names so they're easier to identify
// ENCODER 1
#define VJOY_ENCODER_1_CLOCK_WISE_SINGLE         F("BUTTON_1")
#define VJOY_ENCODER_1_CLOCK_WISE_ACCEL          F("BUTTON_2")
#define VJOY_ENCODER_1_COUNTER_CLOCK_WISE_SINGLE F("BUTTON_3")
#define VJOY_ENCODER_1_COUNTER_CLOCK_WISE_ACCEL  F("BUTTON_4")
#define VJOY_ENCODER_1_CLICK_SIMPLE              F("BUTTON_5")
#define VJOY_ENCODER_1_CLICK_LONG                F("BUTTON_6")

// ENCODER 2
#define VJOY_ENCODER_2_CLOCK_WISE_SINGLE         F("BUTTON_7")
#define VJOY_ENCODER_2_CLOCK_WISE_ACCEL          F("BUTTON_8")
#define VJOY_ENCODER_2_COUNTER_CLOCK_WISE_SINGLE F("BUTTON_9")
#define VJOY_ENCODER_2_COUNTER_CLOCK_WISE_ACCEL  F("BUTTON_10")
#define VJOY_ENCODER_2_CLICK_SIMPLE              F("BUTTON_11")
#define VJOY_ENCODER_2_CLICK_LONG                F("BUTTON_12")

// SWITCHES ROW 1
#define VJOY_SWITCH_1                            F("BUTTON_13")
#define VJOY_SWITCH_2                            F("BUTTON_14")
#define VJOY_SWITCH_3                            F("BUTTON_15")

// ENCODER 3
#define VJOY_ENCODER_3_CLOCK_WISE_SINGLE         F("BUTTON_16")
#define VJOY_ENCODER_3_CLOCK_WISE_ACCEL          F("BUTTON_17")
#define VJOY_ENCODER_3_COUNTER_CLOCK_WISE_SINGLE F("BUTTON_18")
#define VJOY_ENCODER_3_COUNTER_CLOCK_WISE_ACCEL  F("BUTTON_19")
#define VJOY_ENCODER_3_CLICK_SIMPLE              F("BUTTON_20")
#define VJOY_ENCODER_3_CLICK_LONG                F("BUTTON_21")

// ENCODER 4
#define VJOY_ENCODER_4_CLOCK_WISE_SINGLE         F("BUTTON_22")
#define VJOY_ENCODER_4_CLOCK_WISE_ACCEL          F("BUTTON_23")
#define VJOY_ENCODER_4_COUNTER_CLOCK_WISE_SINGLE F("BUTTON_24")
#define VJOY_ENCODER_4_COUNTER_CLOCK_WISE_ACCEL  F("BUTTON_25")
#define VJOY_ENCODER_4_CLICK_SIMPLE              F("BUTTON_26")
#define VJOY_ENCODER_4_CLICK_LONG                F("BUTTON_27")

// SWITCHES ROW 2
#define VJOY_SWITCH_4                            F("BUTTON_28")
#define VJOY_SWITCH_5                            F("BUTTON_29")
#define VJOY_SWITCH_6                            F("BUTTON_30")

// SWITCHES ROW 3
#define VJOY_SWITCH_7                            F("BUTTON_31")
#define VJOY_SWITCH_8                            F("BUTTON_32")
#define VJOY_SWITCH_9                            F("BUTTON_33")

// ON/OFF SWITCHES
#define VJOY_ON_OFF_SWITCH_1                     F("BUTTON_34")
#define VJOY_ON_OFF_SWITCH_2                     F("BUTTON_35")
#define VJOY_ON_OFF_SWITCH_3                     F("BUTTON_36")
#define VJOY_ON_OFF_SWITCH_4                     F("BUTTON_37")
#define VJOY_ON_OFF_SWITCH_5                     F("BUTTON_38")
#define VJOY_ON_OFF_SWITCH_6                     F("BUTTON_39")
#define VJOY_ON_OFF_SWITCH_7                     F("BUTTON_40")


//define ACTIONS
#define VJOY_ACTION_PRESS                        F("PRESS")
#define VJOY_ACTION_RELEASE                      F("RELEASE")

class FullVJoy : public FSConfig
{
public:
    FullVJoy() = default;
    ~FullVJoy() = default;

    void setup(std::shared_ptr<FSControls> fsControls, CmdMessenger & comm) override;
};

} // namespace spaduino
