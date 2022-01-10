#include "ExampleVJoy.h"
#include "ExamplePins.h"

#include <FSAction.h>
#include <FSControls.h>
#include <FSDataID.h>

#include <display/FSValueLedDisplay.h>
#include <display/FSValueTM1637Display.h>

#include <input/InputSwitch.h>
#include <input/InputRotaryEncoderLongPressButton.h>

// Define button names so they're easier to identify
#define VJOY_SWITCH_1                               F("BUTTON_1")

#define VJOY_ENCODER_1_CLOCK_WISE_SINGLE            F("BUTTON_2")
#define VJOY_ENCODER_1_CLOCK_WISE_ACCEL             F("BUTTON_3")
#define VJOY_ENCODER_1_COUNTER_CLOCK_WISE_SINGLE    F("BUTTON_4")
#define VJOY_ENCODER_1_COUNTER_CLOCK_WISE_ACCEL     F("BUTTON_5")
#define VJOY_ENCODER_1_CLICK_SIMPLE                 F("BUTTON_6")
#define VJOY_ENCODER_1_CLICK_LONG                   F("BUTTON_7")

#define VJOY_ON_OFF_SWITCH_1                        F("BUTTON_8")

//define ACTIONS
#define VJOY_ACTION_PRESS                           F("PRESS")
#define VJOY_ACTION_RELEASE                         F("RELEASE")

namespace spaduino
{

constexpr FSDataID AP_MASTER_LED              = FSDataID::ID_10;
constexpr FSDataID AP_ALTITUDE_VAL            = FSDataID::ID_11;
constexpr FSDataID AP_MASTER                  = FSDataID::ID_12;
constexpr FSDataID AP_ALTITUDE_ENCODER        = FSDataID::ID_13;
constexpr FSDataID TAXI_LIGHT                 = FSDataID::ID_14;

void ExampleVJoy::setup(std::shared_ptr<FSControls> fscontrols, CmdMessenger & comm)
{
    // initialize a led, subscribed to the "LOCAL:ARDUPILOT LED 1" var
    // We'll create this LOCAL VAR in Spad.Next and will update its value with a script
    fscontrols->subscribe(F("LOCAL:ARDUPILOT LED 1"), AP_MASTER_LED);
    // initialize the led object
    std::shared_ptr<display::FSValueLedDisplay> led( new display::FSValueLedDisplay(AP_MASTER_LED, pins::LED));
    // add the object to FSControls
    fscontrols->addDisplayObject(led);


    // Initialize a TM1637 7 segment display to show the autopilot Altitude
    // We'll create this LOCAL VAR in Spad.Next and will update its value with a script
    fscontrols->subscribe(F("LOCAL:ARDUPILOT SELECTED ALTITUDE"), AP_ALTITUDE_VAL);

    // instantiate the 7 segment display (4 digits)
    // you can instantiate a 6 digits display with FSValueTM1637Display6Digits
    std::shared_ptr<display::FSValueTM1637Display4Digits> altitudeValue( 
                                                // the Data ID assigned to it is AP_ALTITUDE_VAL (AP_ALTITUDE_VAL)
                                                new display::FSValueTM1637Display4Digits(AP_ALTITUDE_VAL,
                                                // It will display values with 1 unity precission
                                                // ie. when we receive 113 from Spad.Next it will show 113
                                                display::FSValueDisplay::eDisplayRound::e1,
                                                // pins for the 7 segments display
                                                pins::DISPLAY_CLK, 
                                                pins::DISPLAY_DIO, 
                                                // padding (adds zeroes at the beginning of value)
                                                true));
    altitudeValue->setShowMinus1AsDashes(true);
    fscontrols->addDisplayObject(altitudeValue);


    // Initialize a push button that enables AP MASTER
    std::shared_ptr<input::InputSwitch> inputSwitchAPMaster( new input::InputSwitch(pins::PUSH_BUTTON,
                                                AP_MASTER,
                                                std::make_shared<FSActionEmulate>(comm, VJOY_SWITCH_1, VJOY_ACTION_PRESS),
                                                nullptr));
    fscontrols->addInputObject(inputSwitchAPMaster);


    std::shared_ptr<input::InputRotaryEncoderLongPressButton> inputEncoderAltitude(
                                                        new input::InputRotaryEncoderLongPressButton(pins::ENCODER_PIN_1, 
                                                          pins::ENCODER_PIN_2, 
                                                          pins::ENCODER_BUTTON, 
                                                          AP_ALTITUDE_ENCODER,
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_1_CLOCK_WISE_SINGLE, VJOY_ACTION_PRESS),
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_1_CLOCK_WISE_ACCEL, VJOY_ACTION_PRESS),
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_1_COUNTER_CLOCK_WISE_SINGLE, VJOY_ACTION_PRESS),
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_1_COUNTER_CLOCK_WISE_ACCEL, VJOY_ACTION_PRESS),
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_1_CLICK_SIMPLE, VJOY_ACTION_PRESS),
                                                          std::make_shared<FSActionEmulate>(comm, VJOY_ENCODER_1_CLICK_LONG, VJOY_ACTION_PRESS),
                                                          true));
    fscontrols->addInputObject(inputEncoderAltitude);


    std::shared_ptr<input::InputSwitch> inputSwitchTaxiLights(new input::InputSwitch(pins::ON_OFF_SWITCH,
                                    TAXI_LIGHT,
                                    std::make_shared<FSActionEmulate>(comm, VJOY_ON_OFF_SWITCH_1, VJOY_ACTION_PRESS),
                                    std::make_shared<FSActionEmulate>(comm, VJOY_ON_OFF_SWITCH_1, VJOY_ACTION_RELEASE)));
    fscontrols->addInputObject(inputSwitchTaxiLights);
}   

} // namespace spaduino
