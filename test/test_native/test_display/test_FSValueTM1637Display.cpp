#include <gtest/gtest.h>
//#include "utils.h"
#include "CmdMessengerTestBase.h"

#include <display/FSValueTM1637Display.h>

#include <TM1637.h>

namespace spaduino
{
namespace test_display
{
    class FSValueTM1637DisplayTest: public spaduino::test::CmdMessengerTestBase
    {
        public:
            FSValueTM1637DisplayTest()
                : spaduino::test::CmdMessengerTestBase()
            {
            }
            ~FSValueTM1637DisplayTest() = default;
    };

    TEST_F( FSValueTM1637DisplayTest, test_tm1637_display_simple_value)
    {
        TM1637 *mock = TM1637::getInstance();

        std::unique_ptr<spaduino::display::FSValueTM1637Display> display(new spaduino::display::FSValueTM1637Display(spaduino::FSDataID::ID_11,
                                                           spaduino::display::FSValueDisplay::eDisplayRound::e1,
                                                           1,
                                                           2,
                                                           false)); 

        EXPECT_EQ("      ", mock->getShownValue());
        updateDataInState(spaduino::FSDataID::ID_11, 123);
        display->loopOnce();
        EXPECT_EQ("   123", mock->getShownValue());

        updateDataInState(spaduino::FSDataID::ID_11, 1234);
        display->loopOnce();
        EXPECT_EQ("  1234", mock->getShownValue());
    }

    TEST_F( FSValueTM1637DisplayTest, test_tm1637_display_simple_value_4_digits)
    {
        TM1637 *mock = TM1637::getInstance(TM1637::NumDigits::FOUR);

        std::unique_ptr<spaduino::display::FSValueTM1637Display> display(new spaduino::display::FSValueTM1637Display(spaduino::FSDataID::ID_11,
                                                           spaduino::display::FSValueDisplay::eDisplayRound::e1,
                                                           1,
                                                           2,
                                                           false,
                                                           spaduino::display::FSValueTM1637Display::NumDigits::FOUR)); 

        EXPECT_EQ("    ", mock->getShownValue());
        updateDataInState(spaduino::FSDataID::ID_11, 123);
        display->loopOnce();
        EXPECT_EQ(" 123", mock->getShownValue());

        updateDataInState(spaduino::FSDataID::ID_11, 1234);
        display->loopOnce();
        EXPECT_EQ("1234", mock->getShownValue());
    }


    TEST_F( FSValueTM1637DisplayTest, test_tm1637_display_simple_value_padding)
    {
        TM1637 *mock = TM1637::getInstance();
        std::unique_ptr<spaduino::display::FSValueTM1637Display> display(new spaduino::display::FSValueTM1637Display(
                                                           spaduino::FSDataID::ID_11,
                                                           spaduino::display::FSValueDisplay::eDisplayRound::e1,
                                                           1,
                                                           2,
                                                           true)); 
        /*
        spaduino::FSCommand cmd;
        cmd.valueID = spaduino::FSID::AVIONICS_ELECTRICAL_POWER;
        cmd.value = 1;
        display.display(cmd);
        */
        EXPECT_EQ("      ", mock->getShownValue());

        updateDataInState(spaduino::FSDataID::ID_11, 123);
        display->loopOnce();
        EXPECT_EQ("000123", mock->getShownValue());

        updateDataInState(spaduino::FSDataID::ID_11, 1234);
        display->loopOnce();
        EXPECT_EQ("001234", mock->getShownValue());
    }

    TEST_F( FSValueTM1637DisplayTest, test_tm1637_display_simple_value_padding_with_helper)
    {
        TM1637 *mock = TM1637::getInstance();
        std::unique_ptr<spaduino::display::FSValueTM1637Display6Digits> display(new spaduino::display::FSValueTM1637Display6Digits(
                                                           spaduino::FSDataID::ID_11,
                                                           spaduino::display::FSValueDisplay::eDisplayRound::e1,
                                                           1,
                                                           2,
                                                           true)); 
        /*
        spaduino::FSCommand cmd;
        cmd.valueID = spaduino::FSID::AVIONICS_ELECTRICAL_POWER;
        cmd.value = 1;
        display.display(cmd);
        */
        EXPECT_EQ("      ", mock->getShownValue());

        updateDataInState(spaduino::FSDataID::ID_11, 123);
        display->loopOnce();
        EXPECT_EQ("000123", mock->getShownValue());

        updateDataInState(spaduino::FSDataID::ID_11, 1234);
        display->loopOnce();
        EXPECT_EQ("001234", mock->getShownValue());
    }

    TEST_F( FSValueTM1637DisplayTest, test_tm1637_display_simple_value_padding_4_digits)
    {
        TM1637 *mock = TM1637::getInstance(TM1637::NumDigits::FOUR);
        std::unique_ptr<spaduino::display::FSValueTM1637Display> display(new spaduino::display::FSValueTM1637Display(
                                                           spaduino::FSDataID::ID_11,
                                                           spaduino::display::FSValueDisplay::eDisplayRound::e1,
                                                           1,
                                                           2,
                                                           true,
                                                           spaduino::display::FSValueTM1637Display::NumDigits::FOUR)); 
        EXPECT_EQ("    ", mock->getShownValue());

        updateDataInState(spaduino::FSDataID::ID_11, 123);
        display->loopOnce();
        EXPECT_EQ("0123", mock->getShownValue());

        updateDataInState(spaduino::FSDataID::ID_11, 1234);
        display->loopOnce();
        EXPECT_EQ("1234", mock->getShownValue());
    }

    TEST_F( FSValueTM1637DisplayTest, test_tm1637_display_simple_value_padding_4_digits_with_helper)
    {
        TM1637 *mock = TM1637::getInstance(TM1637::NumDigits::FOUR);
        std::unique_ptr<spaduino::display::FSValueTM1637Display4Digits> display(new spaduino::display::FSValueTM1637Display4Digits(
                                                           spaduino::FSDataID::ID_11,
                                                           spaduino::display::FSValueDisplay::eDisplayRound::e1,
                                                           1,
                                                           2,
                                                           true)); 
        EXPECT_EQ("    ", mock->getShownValue());

        updateDataInState(spaduino::FSDataID::ID_11, 123);
        display->loopOnce();
        EXPECT_EQ("0123", mock->getShownValue());

        updateDataInState(spaduino::FSDataID::ID_11, 1234);
        display->loopOnce();
        EXPECT_EQ("1234", mock->getShownValue());
    }

    TEST_F( FSValueTM1637DisplayTest, test_tm1637_display_simple_value_negative)
    {
        TM1637 *mock = TM1637::getInstance();
        std::unique_ptr<spaduino::display::FSValueTM1637Display> display( new spaduino::display::FSValueTM1637Display(
                                                           spaduino::FSDataID::ID_11,
                                                           spaduino::display::FSValueDisplay::eDisplayRound::e1,
                                                           1,
                                                           2,
                                                           false)); 
        /*
        spaduino::FSCommand cmd;
        cmd.valueID = spaduino::FSID::AVIONICS_ELECTRICAL_POWER;
        cmd.value = 1;
        display.display(cmd);
        */
        EXPECT_EQ("      ", mock->getShownValue());

        updateDataInState(spaduino::FSDataID::ID_11, -123);
        display->loopOnce();
        EXPECT_EQ("  -123", mock->getShownValue());

        updateDataInState(spaduino::FSDataID::ID_11, -1234);
        display->loopOnce();
        EXPECT_EQ(" -1234", mock->getShownValue());
    }

    TEST_F( FSValueTM1637DisplayTest, test_tm1637_display_simple_value_padding_negative )
    {
        TM1637 *mock = TM1637::getInstance();
        std::unique_ptr<spaduino::display::FSValueTM1637Display> display( new spaduino::display::FSValueTM1637Display(
                                                           spaduino::FSDataID::ID_11,
                                                           spaduino::display::FSValueDisplay::eDisplayRound::e1,
                                                           1,
                                                           2,
                                                           true));

        EXPECT_EQ("      ", mock->getShownValue());

        updateDataInState(spaduino::FSDataID::ID_11, -123);
        display->loopOnce();
        EXPECT_EQ("-00123", mock->getShownValue());

        updateDataInState(spaduino::FSDataID::ID_11, -1234);
        display->loopOnce();
        EXPECT_EQ("-01234", mock->getShownValue());
    }
    
    /*
    TEST_F( FSValueTM1637DisplayTest, test_tm1637_display_avionics_power )
    {
        TM1637 *mock = TM1637::getInstance();
        std::unique_ptr<spaduino::display::FSValueTM1637Display> display( new spaduino::display::FSValueTM1637Display(
                                                           spaduino::FSDataID::ID_11,
                                                           F("AltitudeDisplay"),
                                                           *(comm_.get()),  
                                                           F("SIMCONNECT:ALTUTUDE VAR"),
                                                           spaduino::display::FSValueDisplay::eDisplayRound::e1,
                                                           1,
                                                           2,
                                                           true,
                                                           5); 

        spaduino::FSCommand cmd;
        cmd.valueID = spaduino::FSID::AVIONICS_ELECTRICAL_POWER;
        cmd.value = 0;
        display.display(cmd);

        cmd.valueID = spaduino::FSID::ALTITUDE;
        cmd.value = -123;
        display.display(cmd);
        EXPECT_EQ("      ", mock->getShownValue());

        cmd.valueID = spaduino::FSID::AVIONICS_ELECTRICAL_POWER;
        cmd.value = 1;
        display.display(cmd);
        EXPECT_EQ("-00123", mock->getShownValue());

        cmd.valueID = spaduino::FSID::AVIONICS_ELECTRICAL_POWER;
        cmd.value = 0;
        display.display(cmd);
        EXPECT_EQ("      ", mock->getShownValue());
    }
*/

    TEST_F( FSValueTM1637DisplayTest, test_tm1637_display_simple_value_padding_round_10)
    {
        TM1637 *mock = TM1637::getInstance();
        std::unique_ptr<spaduino::display::FSValueTM1637Display> display( new spaduino::display::FSValueTM1637Display(
                                                           spaduino::FSDataID::ID_10,
                                                           spaduino::display::FSValueDisplay::eDisplayRound::e10,
                                                           1,
                                                           2,
                                                           true)); 

        EXPECT_EQ("      ", mock->getShownValue());

        updateDataInState(spaduino::FSDataID::ID_10, 123);
        display->loopOnce();
        EXPECT_EQ("000120", mock->getShownValue());

        updateDataInState(spaduino::FSDataID::ID_10, 1234);
        display->loopOnce();
        EXPECT_EQ("001230", mock->getShownValue());
    }

    TEST_F( FSValueTM1637DisplayTest, test_tm1637_display_simple_value_padding_round_100)
    {
        TM1637 *mock = TM1637::getInstance();
        std::unique_ptr<spaduino::display::FSValueTM1637Display> display( new spaduino::display::FSValueTM1637Display(
                                                           spaduino::FSDataID::ID_10,
                                                           spaduino::display::FSValueDisplay::eDisplayRound::e100,
                                                           1,
                                                           2,
                                                           true)); 

        EXPECT_EQ("      ", mock->getShownValue());

        updateDataInState(spaduino::FSDataID::ID_10, 123);
        display->loopOnce();
        EXPECT_EQ("000100", mock->getShownValue());

        updateDataInState(spaduino::FSDataID::ID_10, 1234);
        display->loopOnce();
        EXPECT_EQ("001200", mock->getShownValue());
    }

    TEST_F( FSValueTM1637DisplayTest, test_tm1637_display_simple_value_padding_round_1000)
    {
        TM1637 *mock = TM1637::getInstance();
        std::unique_ptr<spaduino::display::FSValueTM1637Display> display( new spaduino::display::FSValueTM1637Display(
                                                           spaduino::FSDataID::ID_10,
                                                           spaduino::display::FSValueDisplay::eDisplayRound::e1000,
                                                           1,
                                                           2,
                                                           true)); 

        EXPECT_EQ("      ", mock->getShownValue());

        updateDataInState(spaduino::FSDataID::ID_10, 123);
        display->loopOnce();
        EXPECT_EQ("000000", mock->getShownValue());

        updateDataInState(spaduino::FSDataID::ID_10, 1234);
        display->loopOnce();
        EXPECT_EQ("001000", mock->getShownValue());
    }

    TEST_F( FSValueTM1637DisplayTest, test_tm1637_display_minus_1_as_dashes)
    {
        TM1637 *mock = TM1637::getInstance();
        std::unique_ptr<spaduino::display::FSValueTM1637Display> display( new spaduino::display::FSValueTM1637Display(
                                                           spaduino::FSDataID::ID_10,
                                                           spaduino::display::FSValueDisplay::eDisplayRound::e1,
                                                           1,
                                                           2,
                                                           false)); 
        display->setShowMinus1AsDashes(true);

        EXPECT_EQ("      ", mock->getShownValue());

        updateDataInState(spaduino::FSDataID::ID_10, 123);
        display->loopOnce();
        EXPECT_EQ("   123", mock->getShownValue());

        updateDataInState(spaduino::FSDataID::ID_10, -1);
        display->loopOnce();
        EXPECT_EQ("------", mock->getShownValue());
    }

    TEST_F( FSValueTM1637DisplayTest, test_tm1637_display_minus1_value)
    {
        TM1637 *mock = TM1637::getInstance();

        std::unique_ptr<spaduino::display::FSValueTM1637Display> display(new spaduino::display::FSValueTM1637Display(spaduino::FSDataID::ID_11,
                                                           spaduino::display::FSValueDisplay::eDisplayRound::e1,
                                                           1,
                                                           2,
                                                           false)); 
        display->setShowMinus1AsDashes(true);

        EXPECT_EQ("      ", mock->getShownValue());
        updateDataInState(spaduino::FSDataID::ID_11, 123);
        display->loopOnce();
        EXPECT_EQ("   123", mock->getShownValue());

        updateDataInState(spaduino::FSDataID::ID_11, -1);
        display->loopOnce();
        EXPECT_EQ("------", mock->getShownValue());
    }

    TEST_F( FSValueTM1637DisplayTest, test_tm1637_display_minus1_value_4_digits)
    {
        TM1637 *mock = TM1637::getInstance(TM1637::NumDigits::FOUR);

        std::unique_ptr<spaduino::display::FSValueTM1637Display> display(new spaduino::display::FSValueTM1637Display(spaduino::FSDataID::ID_11,
                                                           spaduino::display::FSValueDisplay::eDisplayRound::e1,
                                                           1,
                                                           2,
                                                           false,
                                                           spaduino::display::FSValueTM1637Display::NumDigits::FOUR)); 
        display->setShowMinus1AsDashes(true);

        EXPECT_EQ("    ", mock->getShownValue());
        updateDataInState(spaduino::FSDataID::ID_11, 123);
        display->loopOnce();
        EXPECT_EQ(" 123", mock->getShownValue());

        updateDataInState(spaduino::FSDataID::ID_11, -1);
        display->loopOnce();
        EXPECT_EQ("----", mock->getShownValue());
    }
}
} // namespace spaduino