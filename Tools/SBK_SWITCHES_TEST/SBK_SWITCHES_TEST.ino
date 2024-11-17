/**********************************************************************************************
 *    SBK_SWITCHES_TEST is a code to test switches and push buttons connections used in many
 *    Ghostbusters props replica.
 *    Copyright (c) 2024 Samuel Barabé
 *
 *    This program is free software: you can redistribute it and/or modify it under the terms
 *    of the GNU General Public License as published by the Free Software Foundation, either
 *    version 3 of the License, or any later version.
 *
 *    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *    See the GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License along with this program.
 *    If not, see <https://www.gnu.org/licenses/>.
 ***********************************************************************************************/

/**********************************************************************************************
 *    GENERAL INFO :
 *
 *    SBK_SWITCHES_TEST
 *    <https://github.com/sbarabe/SBK_PROTONPACK_CORE/tree/main/Tools/SBK_SWITCHES_TEST>
 *    Version 0
 *
 *    You can use this code to check your switches connections :
 *    1) Line 48 >>> Define the Serial Communication baud rate to use : 9600 is default and should be ok for this test.
 *    2) Lines 56-65 >>> DEFINE as many SWITCHE pins you need to test according to your schmatics. 
 *       Leave commented "//"" the uneeded pins.
 *       Take note of your associations.
 *    3) Line 67-72 >>> DEFINE as many PUSH BUTTON pins you need to test according to your schmatics.
 *       Leave commented "//"" the uneeded pins.
 *       Take note of your associations. 
 *    4) Line 75-108 >>> You can reverse the SWITCH output logic setting to true the 2nd argument
 *       (bool resverse) of the switch instance : Switch switch_name(unsigned int SW_PIN, bool reverse); 
 *    5) Line 110-130 >>> You can reverse PUSH BUTTON output logic by setting to true the 2nd argument
 *       (bool resverse) of the switch instance : Switch push_button_name(unsigned int PB_PIN, bool reverse);
 *    6) Load the code to your arduino module, open your Serial Monitor on the Arduino IDE, and set the right
 *       baud rate in the Serial Monitor windows.
 *  
 *    If your pins deifinitions, wiring and Serial Monitor setup are good, you should see messages in the
 *    Serial Monitor windows when a switch/button is turned on/off.
 *
 ***********************************************************************************************/

#include <Arduino.h>
#include <Wire.h>  // Include the I2C library (required)

#define BAUD_RATE 9600

/*********************************************/
/*                                           */
/* BUTTONS & SWITCHES definition and helpers */
/*                                           */
/*********************************************/
/* DEFINE the Arduino pins for SWITCHES to test, comment "//" switches you do not need */
#define SW1_PIN   5    
#define SW2_PIN   4   
#define SW3_PIN   6 
#define SW4_PIN   7   
//#define SW5_PIN   8  
//#define SW6_PIN   11
//#define SW7_PIN   A0
//#define SW8_PIN   A1
//#define SW9_PIN   12
//#define SW10_PIN  13

/* DEFINE the Arduino pins for PUSH BUTTON to test, comment "//" button you do not need */
#define PB1_PIN   9
#define PB2_PIN   10
//#define PB3_PIN   A2
//#define PB4_PIN   A3
//#define PB5_PIN   A7


/* These section creates SWITCHE instances if a pin is defined.       */
/* Switch switch_name(unsigned int SW_PIN, bool reverse);             */
/* ON/OFF are reversed if the second argument (bool reverse) is true  */ 
#include "SwitchEngine.h"
#ifdef SW1_PIN 
Switch SW1(SW1_PIN, false);
#endif
#ifdef SW2_PIN
Switch SW2(SW2_PIN, false);
#endif
#ifdef SW3_PIN
Switch SW3(SW3_PIN, false);
#endif
#ifdef SW4_PIN
Switch SW4(SW4_PIN, false);
#endif
#ifdef SW5_PIN
Switch SW5(SW5_PIN, false);
#endif
#ifdef SW6_PIN
Switch SW6(SW6_PIN, false);
#endif
#ifdef SW7_PIN
Switch SW7(SW7_PIN, false);
#endif
#ifdef SW8_PIN
Switch SW8(SW8_PIN, false);
#endif
#ifdef SW9_PIN
Switch SW9(SW9_PIN, false);
#endif
#ifdef SW10_PIN
Switch SW10(SW10_PIN, false);
#endif

/* These section creates PUSH BUTTON instances if a pin is defined.   */
/* Switch pushbutton_name(unsigned int PB_PIN, bool reverse);         */
/* ON/OFF are reversed if the second argument (bool reverse) is true  */
#ifdef PB1_PIN 
Switch PB_1(PB1_PIN, false);
#endif
#ifdef PB2_PIN 
Switch PB_2(PB2_PIN, false);
#endif
#ifdef PB3_PIN 
Switch PB_3(PB3_PIN, false);
#endif
#ifdef PB4_PIN 
Switch PB_4(PB4_PIN, false);
#endif
#ifdef PB4_PIN 
Switch PB_4(PB4_PIN, false);
#endif
#ifdef PB5_PIN 
Switch PB_5(PB5_PIN, false);
#endif


void setup(void) {
// Setup Serial communication to output swithes/buttons states to serial monitor
Serial.begin(BAUD_RATE);

// setup for the switches
#ifdef SW1_PIN 
SW1.begin();
#endif
#ifdef SW2_PIN
SW2.begin();
#endif
#ifdef SW3_PIN
SW3.begin();
#endif
#ifdef SW4_PIN
SW4.begin();
#endif
#ifdef SW5_PIN
SW5.begin();
#endif
#ifdef SW6_PIN
SW6.begin();
#endif
#ifdef SW7_PIN
SW7.begin();
#endif
#ifdef SW8_PIN
SW8.begin();
#endif
#ifdef SW9_PIN
 SW9.begin();
#endif
#ifdef SW10_PIN
SW10.begin();
#endif
// setup for the buttons
#ifdef PB1_PIN 
PB_1.begin();
#endif
#ifdef PB2_PIN
PB_2.begin();
#endif
#ifdef PB3_PIN
PB_3.begin();
#endif
#ifdef PB4_PIN
PB_4.begin();
#endif
#ifdef PB5_PIN
PB_5.begin();
#endif
}

void loop() {
// Switches outputs
#ifdef SW1_PIN 
SW1.getState();
if(SW1.toggleON()) Serial.println("Switch #1 :  ON");
if(SW1.toggleOFF()) Serial.println("Switch #1 : OFF");
#endif
#ifdef SW2_PIN
SW2.getState();
if(SW2.toggleON()) Serial.println("Switch #2 :  ON");
if(SW2.toggleOFF()) Serial.println("Switch #2 : OFF");
#endif
#ifdef SW3_PIN
SW3.getState();
if(SW3.toggleON()) Serial.println("Switch #3 :  ON");
if(SW3.toggleOFF()) Serial.println("Switch #3 : OFF");
#endif
#ifdef SW4_PIN
SW4.getState();
if(SW4.toggleON()) Serial.println("Switch #4 :  ON");
if(SW4.toggleOFF()) Serial.println("Switch #4 : OFF");
#endif
#ifdef SW5_PIN
SW5.getState();
if(SW5.toggleON()) Serial.println("Switch #5 :  ON");
if(SW5.toggleOFF()) Serial.println("Switch #5 : OFF");
#endif
#ifdef SW6_PIN
SW6.getState();
if(SW6.toggleON()) Serial.println("Switch #6 :  ON");
if(SW6.toggleOFF()) Serial.println("Switch #6 : OFF");
#endif
#ifdef SW7_PIN
SW7.getState();
if(SW7.toggleON()) Serial.println("Switch #7 :  ON");
if(SW7.toggleOFF()) Serial.println("Switch #7 : OFF");
#endif
#ifdef SW8_PIN
SW8.getState();
if(SW8.toggleON()) Serial.println("Switch #8 :  ON");
if(SW8.toggleOFF()) Serial.println("Switch #8 : OFF");
#endif
#ifdef SW9_PIN
SW9.getState();
if(SW9.toggleON()) Serial.println("Switch #9 :  ON");
if(SW9.toggleOFF()) Serial.println("Switch #9 : OFF");
#endif
#ifdef SW10_PIN
SW10.getState();
if(SW10.toggleON()) Serial.println("Switch #10 :  ON");
if(SW10.toggleOFF()) Serial.println("Switch #10 : OFF");
#endif
// setup for the buttons
#ifdef PB1_PIN 
PB_1.getState();
if(PB_1.toggleON()) Serial.println("Push Button #1 :  PRESSED");
if(PB_1.toggleOFF()) Serial.println("Push Button #1 : RELEASED");
#endif
#ifdef PB2_PIN
PB_2.getState();
if(PB_2.toggleON()) Serial.println("Push Button #2 :  PRESSED");
if(PB_2.toggleOFF()) Serial.println("Push Button #2 : RELEASED");
#endif
#ifdef PB3_PIN
PB_3.getState();
if(PB_3.toggleON()) Serial.println("Push Button #3 :  PRESSED");
if(PB_3.toggleOFF()) Serial.println("Push Button #3 : RELEASED");
#endif
#ifdef PB4_PIN
PB_4.getState();
if(PB_4.toggleON()) Serial.println("Push Button #4 :  PRESSED");
if(PB_4.toggleOFF()) Serial.println("Push Button #4 : RELEASED");
#endif
#ifdef PB5_PIN
PB_5.getState();
if(PB_5.toggleON()) Serial.println("Push Button #5 :  PRESSED");
if(PB_5.toggleOFF()) Serial.println("Push Button #5 : RELEASED");
#endif  

}