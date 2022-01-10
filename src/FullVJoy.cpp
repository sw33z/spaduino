#include "FullVJoy.h"

#include <Arduino.h>

#include <CmdMessenger.h>

#include <FSDataID.h>
#include <PinsConfig.h>
#include <FSControls.h>
#include <FSAction.h>
#include <FSControls.h>

#include <display/FSValueLedDisplay.h>
#include <display/FSValueTM1637Display.h>

#include <input/InputSwitch.h>
#include <input/InputRotaryEncoderLongPressButton.h>

namespace spaduino
{

constexpr FSDataID AP_LED_1               = FSDataID::ID_10;
constexpr FSDataID AP_LED_2               = FSDataID::ID_11;
constexpr FSDataID AP_LED_3               = FSDataID::ID_12;
constexpr FSDataID AP_LED_4               = FSDataID::ID_13;
constexpr FSDataID AP_LED_5               = FSDataID::ID_14;
constexpr FSDataID AP_LED_6               = FSDataID::ID_15;
constexpr FSDataID AP_LED_7               = FSDataID::ID_16;
constexpr FSDataID AP_LED_8               = FSDataID::ID_17;

constexpr FSDataID AP_7SEGMENT_1          = FSDataID::ID_18;
constexpr FSDataID AP_7SEGMENT_2          = FSDataID::ID_19;
constexpr FSDataID AP_7SEGMENT_3          = FSDataID::ID_20;
constexpr FSDataID AP_7SEGMENT_4          = FSDataID::ID_21;

constexpr FSDataID AP_ENCODER_1           = FSDataID::ID_22;
constexpr FSDataID AP_ENCODER_2           = FSDataID::ID_23;
constexpr FSDataID AP_ENCODER_3           = FSDataID::ID_24;
constexpr FSDataID AP_ENCODER_4           = FSDataID::ID_25;

constexpr FSDataID AP_SWITCH_1            = FSDataID::ID_26;
constexpr FSDataID AP_SWITCH_2            = FSDataID::ID_27;
constexpr FSDataID AP_SWITCH_3            = FSDataID::ID_28;
constexpr FSDataID AP_SWITCH_4            = FSDataID::ID_29;
constexpr FSDataID AP_SWITCH_5            = FSDataID::ID_30;
constexpr FSDataID AP_SWITCH_6            = FSDataID::ID_31;
constexpr FSDataID AP_SWITCH_7            = FSDataID::ID_32;
constexpr FSDataID AP_SWITCH_8            = FSDataID::ID_33;
constexpr FSDataID AP_SWITCH_9            = FSDataID::ID_34;

constexpr FSDataID AP_ON_OFF_SWITCH_1     = FSDataID::ID_35;
constexpr FSDataID AP_ON_OFF_SWITCH_2     = FSDataID::ID_36;
constexpr FSDataID AP_ON_OFF_SWITCH_3     = FSDataID::ID_37;
constexpr FSDataID AP_ON_OFF_SWITCH_4     = FSDataID::ID_38;
constexpr FSDataID AP_ON_OFF_SWITCH_5     = FSDataID::ID_39;
constexpr FSDataID AP_ON_OFF_SWITCH_6     = FSDataID::ID_40;
constexpr FSDataID AP_ON_OFF_SWITCH_7     = FSDataID::ID_41;

void FullVJoy::setup(std::shared_ptr<FSControls> fscontrols, CmdMessenger & comm)
{
#ifndef UNIT_TEST
    fscontrols->subscribe(F("LOCAL:ARDUPILOT LED 1"), AP_LED_1);
    fscontrols->subscribe(F("LOCAL:ARDUPILOT LED 2"), AP_LED_2);
    fscontrols->subscribe(F("LOCAL:ARDUPILOT LED 3"), AP_LED_3);
    fscontrols->subscribe(F("LOCAL:ARDUPILOT LED 4"), AP_LED_4);
    fscontrols->subscribe(F("LOCAL:ARDUPILOT LED 5"), AP_LED_5);
    fscontrols->subscribe(F("LOCAL:ARDUPILOT LED 6"), AP_LED_6);
    fscontrols->subscribe(F("LOCAL:ARDUPILOT LED 7"), AP_LED_7);
    fscontrols->subscribe(F("LOCAL:ARDUPILOT LED 8"), AP_LED_8);

    fscontrols->subscribe(F("LOCAL:ARDUPILOT 7SEGMENT 1"), AP_7SEGMENT_1);
    fscontrols->subscribe(F("LOCAL:ARDUPILOT 7SEGMENT 2"), AP_7SEGMENT_2);
    fscontrols->subscribe(F("LOCAL:ARDUPILOT 7SEGMENT 3"), AP_7SEGMENT_3);
    fscontrols->subscribe(F("LOCAL:ARDUPILOT 7SEGMENT 4"), AP_7SEGMENT_4);

    std::shared_ptr<display::FSValueLedDisplay> led1(new display::FSValueLedDisplay(AP_LED_1, pins::LED_1));
    std::shared_ptr<display::FSValueLedDisplay> led2(new display::FSValueLedDisplay(AP_LED_2, pins::LED_2));
    std::shared_ptr<display::FSValueLedDisplay> led3(new display::FSValueLedDisplay(AP_LED_3, pins::LED_3));
    std::shared_ptr<display::FSValueLedDisplay> led4(new display::FSValueLedDisplay(AP_LED_4, pins::LED_4));
    std::shared_ptr<display::FSValueLedDisplay> led5(new display::FSValueLedDisplay(AP_LED_5, pins::LED_5));
    std::shared_ptr<display::FSValueLedDisplay> led6(new display::FSValueLedDisplay(AP_LED_6, pins::LED_6));
    std::shared_ptr<display::FSValueLedDisplay> led7(new display::FSValueLedDisplay(AP_LED_7, pins::LED_7));
    std::shared_ptr<display::FSValueLedDisplay> led8(new display::FSValueLedDisplay(AP_LED_8,  pins::LED_8));   

    std::shared_ptr<display::FSValueTM1637Display> display_7segment_1(new display::FSValueTM1637Display(AP_7SEGMENT_1,
                                                            display::FSValueDisplay::eDisplayRound::e1,
                                                            pins::TM1637_1_PIN_1, 
                                                            pins::TM1637_1_PIN_2, 
                                                            false));
    std::shared_ptr<display::FSValueTM1637Display> display_7segment_2(new display::FSValueTM1637Display(AP_7SEGMENT_2,
                                                            display::FSValueDisplay::eDisplayRound::e1,
                                                            pins::TM1637_2_PIN_2, 
                                                            pins::TM1637_2_PIN_1, 
                                                            false));
    display_7segment_2->setShowMinus1AsDashes(true);

    std::shared_ptr<display::FSValueTM1637Display> display_7segment_3(new display::FSValueTM1637Display(AP_7SEGMENT_3,
                                                            display::FSValueDisplay::eDisplayRound::e1,
                                                            pins::TM1637_3_PIN_2, 
                                                            pins::TM1637_3_PIN_1, 
                                                            false));
    display_7segment_3->setShowMinus1AsDashes(true);

    std::shared_ptr<display::FSValueTM1637Display> display_7segment_4(new display::FSValueTM1637Display(AP_7SEGMENT_4,
                                                            display::FSValueDisplay::eDisplayRound::e1,
                                                            pins::TM1637_4_PIN_2, 
                                                            pins::TM1637_4_PIN_1, 
                                                            false));
    display_7segment_4->setShowMinus1AsDashes(true);

    std::shared_ptr<input::InputSwitch> switch1(new input::InputSwitch(pins::SWITCH_1,
                                     AP_SWITCH_1,
                                     std::make_shared<FSActionEmulate>(comm, VJOY_SWITCH_1, VJOY_ACTION_PRESS),
                                     nullptr));
    std::shared_ptr<input::InputSwitch> switch2(new input::InputSwitch(pins::SWITCH_2,
                                     AP_SWITCH_2,
                                     std::make_shared<FSActionEmulate>(comm, VJOY_SWITCH_2, VJOY_ACTION_PRESS),
                                     nullptr));
    std::shared_ptr<input::InputSwitch> switch3(new input::InputSwitch(pins::SWITCH_3,
                                     AP_SWITCH_3,
                                     std::make_shared<FSActionEmulate>(comm, VJOY_SWITCH_3, VJOY_ACTION_PRESS),
                                     nullptr));
    std::shared_ptr<input::InputSwitch> switch4(new input::InputSwitch(pins::SWITCH_4,
                                     AP_SWITCH_4,
                                     std::make_shared<FSActionEmulate>(comm, VJOY_SWITCH_4, VJOY_ACTION_PRESS),
                                     nullptr));
    std::shared_ptr<input::InputSwitch> switch5(new input::InputSwitch(pins::SWITCH_5,
                                     AP_SWITCH_5,
                                     std::make_shared<FSActionEmulate>(comm, VJOY_SWITCH_5, VJOY_ACTION_PRESS),
                                     nullptr));
    std::shared_ptr<input::InputSwitch> switch6(new input::InputSwitch(pins::SWITCH_6,
                                     AP_SWITCH_6,
                                     std::make_shared<FSActionEmulate>(comm, VJOY_SWITCH_6, VJOY_ACTION_PRESS),
                                     nullptr));
    std::shared_ptr<input::InputSwitch> switch7(new input::InputSwitch(pins::SWITCH_7,
                                     AP_SWITCH_7,
                                     std::make_shared<FSActionEmulate>(comm, VJOY_SWITCH_7, VJOY_ACTION_PRESS),
                                     nullptr));
    std::shared_ptr<input::InputSwitch> switch8(new input::InputSwitch(pins::SWITCH_8,
                                     AP_SWITCH_8,
                                     std::make_shared<FSActionEmulate>(comm, VJOY_SWITCH_8, VJOY_ACTION_PRESS),
                                     nullptr));
    std::shared_ptr<input::InputSwitch> switch9(new input::InputSwitch(pins::SWITCH_9,
                                     AP_SWITCH_9,
                                     std::make_shared<FSActionEmulate>(comm, VJOY_SWITCH_9, VJOY_ACTION_PRESS),
                                     nullptr));


    std::shared_ptr<input::InputSwitch> onoff1(new input::InputSwitch(pins::ONOFF_SWITCH_1,
                                    AP_ON_OFF_SWITCH_1,
                                    std::make_shared<FSActionEmulate>(comm, VJOY_ON_OFF_SWITCH_1, VJOY_ACTION_PRESS),
                                    std::make_shared<FSActionEmulate>(comm, VJOY_ON_OFF_SWITCH_1, VJOY_ACTION_RELEASE)));
    std::shared_ptr<input::InputSwitch> onoff2(new input::InputSwitch(pins::ONOFF_SWITCH_2,
                                    AP_ON_OFF_SWITCH_2,
                                    std::make_shared<FSActionEmulate>(comm, VJOY_ON_OFF_SWITCH_2, VJOY_ACTION_PRESS),
                                    std::make_shared<FSActionEmulate>(comm, VJOY_ON_OFF_SWITCH_2, VJOY_ACTION_RELEASE)));
    std::shared_ptr<input::InputSwitch> onoff3(new input::InputSwitch(pins::ONOFF_SWITCH_3,
                                    AP_ON_OFF_SWITCH_3,
                                    std::make_shared<FSActionEmulate>(comm, VJOY_ON_OFF_SWITCH_3, VJOY_ACTION_PRESS),
                                    std::make_shared<FSActionEmulate>(comm, VJOY_ON_OFF_SWITCH_3, VJOY_ACTION_RELEASE)));
    std::shared_ptr<input::InputSwitch> onoff4(new input::InputSwitch(pins::ONOFF_SWITCH_4,
                                    AP_ON_OFF_SWITCH_4,
                                    std::make_shared<FSActionEmulate>(comm, VJOY_ON_OFF_SWITCH_4, VJOY_ACTION_PRESS),
                                    std::make_shared<FSActionEmulate>(comm, VJOY_ON_OFF_SWITCH_4, VJOY_ACTION_RELEASE)));
    std::shared_ptr<input::InputSwitch> onoff5(new input::InputSwitch(pins::ONOFF_SWITCH_5,
                                    AP_ON_OFF_SWITCH_5,
                                    std::make_shared<FSActionEmulate>(comm, VJOY_ON_OFF_SWITCH_5, VJOY_ACTION_PRESS),
                                    std::make_shared<FSActionEmulate>(comm, VJOY_ON_OFF_SWITCH_5, VJOY_ACTION_RELEASE)));
    std::shared_ptr<input::InputSwitch> onoff6(new input::InputSwitch(pins::ONOFF_SWITCH_6,
                                    AP_ON_OFF_SWITCH_6,
                                    std::make_shared<FSActionEmulate>(comm, VJOY_ON_OFF_SWITCH_6, VJOY_ACTION_PRESS),
                                    std::make_shared<FSActionEmulate>(comm, VJOY_ON_OFF_SWITCH_6, VJOY_ACTION_RELEASE)));
    std::shared_ptr<input::InputSwitch> onoff7(new input::InputSwitch(pins::ONOFF_SWITCH_7,
                                    AP_ON_OFF_SWITCH_7,
                                    std::make_shared<FSActionEmulate>(comm, VJOY_ON_OFF_SWITCH_7, VJOY_ACTION_PRESS),
                                    std::make_shared<FSActionEmulate>(comm, VJOY_ON_OFF_SWITCH_7, VJOY_ACTION_RELEASE)));


    std::shared_ptr<input::InputRotaryEncoderLongPressButton> encoder1(
                                                        new input::InputRotaryEncoderLongPressButton(pins::ENCODER_1_PIN_A, 
                                                          pins::ENCODER_1_PIN_B, 
                                                          pins::ENCODER_1_PIN_C, 
                                                          AP_ENCODER_1,
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_1_CLOCK_WISE_SINGLE, VJOY_ACTION_PRESS),
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_1_CLOCK_WISE_ACCEL, VJOY_ACTION_PRESS),
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_1_COUNTER_CLOCK_WISE_SINGLE, VJOY_ACTION_PRESS),
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_1_COUNTER_CLOCK_WISE_ACCEL, VJOY_ACTION_PRESS),
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_1_CLICK_SIMPLE, VJOY_ACTION_PRESS),
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_1_CLICK_LONG, VJOY_ACTION_PRESS),
                                                          true));
    std::shared_ptr<input::InputRotaryEncoderLongPressButton> encoder2(
                                                        new input::InputRotaryEncoderLongPressButton(pins::ENCODER_2_PIN_A, 
                                                          pins::ENCODER_2_PIN_B, 
                                                          pins::ENCODER_2_PIN_C, 
                                                          AP_ENCODER_2,
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_2_CLOCK_WISE_SINGLE, VJOY_ACTION_PRESS),
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_2_CLOCK_WISE_ACCEL, VJOY_ACTION_PRESS),
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_2_COUNTER_CLOCK_WISE_SINGLE, VJOY_ACTION_PRESS),
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_2_COUNTER_CLOCK_WISE_ACCEL, VJOY_ACTION_PRESS),
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_2_CLICK_SIMPLE, VJOY_ACTION_PRESS),
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_2_CLICK_LONG, VJOY_ACTION_PRESS),
                                                          true));
    std::shared_ptr<input::InputRotaryEncoderLongPressButton> encoder3(
                                                        new input::InputRotaryEncoderLongPressButton(pins::ENCODER_3_PIN_B, 
                                                          pins::ENCODER_3_PIN_A, 
                                                          pins::ENCODER_3_PIN_C, 
                                                          AP_ENCODER_3,
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_3_CLOCK_WISE_SINGLE, VJOY_ACTION_PRESS),
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_3_CLOCK_WISE_ACCEL, VJOY_ACTION_PRESS),
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_3_COUNTER_CLOCK_WISE_SINGLE, VJOY_ACTION_PRESS),
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_3_COUNTER_CLOCK_WISE_ACCEL, VJOY_ACTION_PRESS),
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_3_CLICK_SIMPLE, VJOY_ACTION_PRESS),
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_3_CLICK_LONG, VJOY_ACTION_PRESS),
                                                          true));
    std::shared_ptr<input::InputRotaryEncoderLongPressButton> encoder4(
                                                        new input::InputRotaryEncoderLongPressButton(pins::ENCODER_4_PIN_B, 
                                                          pins::ENCODER_4_PIN_A, 
                                                          pins::ENCODER_4_PIN_C, 
                                                          AP_ENCODER_4,
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_4_CLOCK_WISE_SINGLE, VJOY_ACTION_PRESS),
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_4_CLOCK_WISE_ACCEL, VJOY_ACTION_PRESS),
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_4_COUNTER_CLOCK_WISE_SINGLE, VJOY_ACTION_PRESS),
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_4_COUNTER_CLOCK_WISE_ACCEL, VJOY_ACTION_PRESS),
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_4_CLICK_SIMPLE, VJOY_ACTION_PRESS),
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_4_CLICK_LONG, VJOY_ACTION_PRESS),
                                                          true));


    fscontrols->addDisplayObject(led1);
    fscontrols->addDisplayObject(led2);
    fscontrols->addDisplayObject(led3);
    fscontrols->addDisplayObject(led4);
    fscontrols->addDisplayObject(led5);
    fscontrols->addDisplayObject(led6);
    fscontrols->addDisplayObject(led7);
    fscontrols->addDisplayObject(led8);

    fscontrols->addDisplayObject(display_7segment_1);
    fscontrols->addDisplayObject(display_7segment_2);
    fscontrols->addDisplayObject(display_7segment_3);
    fscontrols->addDisplayObject(display_7segment_4);

    fscontrols->addInputObject(switch1);
    fscontrols->addInputObject(switch2);
    fscontrols->addInputObject(switch3);
    fscontrols->addInputObject(switch4);
    fscontrols->addInputObject(switch5);
    fscontrols->addInputObject(switch6);
    fscontrols->addInputObject(switch7);
    fscontrols->addInputObject(switch8);
    fscontrols->addInputObject(switch9);

    fscontrols->addInputObject(onoff1);
    fscontrols->addInputObject(onoff2);
    fscontrols->addInputObject(onoff3);
    fscontrols->addInputObject(onoff4);
    fscontrols->addInputObject(onoff5);
    fscontrols->addInputObject(onoff6);
    fscontrols->addInputObject(onoff7);

    fscontrols->addInputObject(encoder1);
    fscontrols->addInputObject(encoder2);
    fscontrols->addInputObject(encoder3);
    fscontrols->addInputObject(encoder4);
#endif
}

} // namespace spaduino
