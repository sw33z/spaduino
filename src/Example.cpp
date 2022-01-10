#include "Example.h"
#include "ExamplePins.h"

#include <FSAction.h>
#include <FSControls.h>
#include <FSDataID.h>

#include <display/FSValueLedDisplay.h>
#include <display/FSValueTM1637Display.h>

#include <input/InputSwitch.h>
#include <input/InputRotaryEncoder.h>

namespace spaduino
{

constexpr FSDataID AP_MASTER_LED              = FSDataID::ID_10;
constexpr FSDataID AP_ALTITUDE_VAL            = FSDataID::ID_11;
constexpr FSDataID AP_MASTER                  = FSDataID::ID_12;
constexpr FSDataID AP_ALTITUDE_ENCODER        = FSDataID::ID_13;
constexpr FSDataID TAXI_LIGHT                 = FSDataID::ID_14;

void Example::setup(std::shared_ptr<FSControls> fscontrols, CmdMessenger & comm)
{
    // initialize a led, subscribed to the "SIMCONNECT:AUTOPILOT MASTER" var
    // Subscribe the value AP_MASTER_LED (AP_ALTITUDE_VAL) to the var
    fscontrols->subscribe(F("SIMCONNECT:AUTOPILOT MASTER"), AP_MASTER_LED);
    // initialize the led object
    std::shared_ptr<display::FSValueLedDisplay> led( new display::FSValueLedDisplay(AP_MASTER_LED, pins::LED));
    // add the object to FSControls
    fscontrols->addDisplayObject(led);


    // Initialize a TM1637 7 segment display to show the autopilot Altitude
    // Subscribe the value AP_ALTITUDE_VAL (FSDataID::ID_11) to the event
    fscontrols->subscribe(F("SIMCONNECT:AUTOPILOT ALTITUDE LOCK VAR"), AP_ALTITUDE_VAL);

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
                                                std::make_shared<FSActionSendEvent>(comm, F("SIMCONNECT:AP_MASTER")),
                                                nullptr));
    fscontrols->addInputObject(inputSwitchAPMaster);


    std::shared_ptr<input::InputRotaryEncoder> inputEncoderAltitude( new input::InputRotaryEncoder(pins::ENCODER_PIN_1, 
                                                        pins::ENCODER_PIN_2, 
                                                        pins::ENCODER_BUTTON, 
                                                        AP_ALTITUDE_ENCODER,
                                                        std::make_shared<FSActionSendIncrementValue>(comm, AP_ALTITUDE_VAL, 100),
                                                        std::make_shared<FSActionSendIncrementValue>(comm, AP_ALTITUDE_VAL, 500),
                                                        std::make_shared<FSActionSendIncrementValue>(comm, AP_ALTITUDE_VAL, -100),
                                                        std::make_shared<FSActionSendIncrementValue>(comm, AP_ALTITUDE_VAL, -500),
                                                        std::make_shared<FSActionSendEvent>(comm, F("SIMCONNECT:AP_PANEL_ALTITUDE_HOLD")),
                                                        true));
    fscontrols->addInputObject(inputEncoderAltitude);



    fscontrols->subscribe(F("SIMCONNECT:LIGHT TAXI"), TAXI_LIGHT);
    std::shared_ptr<input::InputSwitch> inputSwitchTaxiLights( new input::InputSwitch(pins::ON_OFF_SWITCH,
                                                    TAXI_LIGHT,
                                                    std::make_shared<FSActionSendValue>(comm, TAXI_LIGHT, 1),
                                                    std::make_shared<FSActionSendValue>(comm, TAXI_LIGHT, 0)));
    fscontrols->addInputObject(inputSwitchTaxiLights);
}   

} // namespace spaduino
