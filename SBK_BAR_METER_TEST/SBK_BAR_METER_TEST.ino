/**********************************************************************************************
 *    SBK_BAR_METER_TEST is a code to test the bar meter connections and mapping used in many Ghostbusters props replica.
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
 *    SBK_BAR_METER_TEST
 *    <https://github.com/sbarabe/SBK_PROTONPACK_CORE/tree/main/SBK_BAR_METER_TEST>
 *    Version 0
 *
 *    You can use this code to check your bar meter connections and mapping :
 *    1) Line 60 >>> Set the animation direction : FORWARD/REVERSE
 *    2) Line 64 >>> Set animation speed 
 *    3) Line 69 >>> Set the number of bar meter segments : #define SEG10 or SEG28
 *    4) Lines 71 to 163 >>> Check/edit bar meter leds mapping according to your connections and bar graph type :
 *        - For SEG10, you can edit the mapping according to your connections at line 77.
 *        - For SEG28, you can choose(and edit) between two preset mapping at line 94 according to your bar meter type and connections
 *    5) Line 170 >>> Set the type of bar meter driver  : #define BG_HT16K33 or BG_MAX72xx
 *         - HT16K33 is an I2C driver working with SDA/SCL pins, good for short distance of no more then few inches usually.
 *         - MAX7219/MAX7221 drivers are serial drivers using 3 pins (DATA, CLOCK and LOAD), it is slower then I2C but better for long distance between Arduino and driver.
 *    
 *    If your pins deifinition, wiring and leds mapping matrix are good, bar meter segements will turn ON from bottom to top until all ON, then reverse to OFF.

 *    If there is no segment ON, yours connection between driver and bar metter, AND/OR communication between Arduino and driver, AND/OR this code bar meter pins definition might be wrong.
 *    If you have some segments ON but acting weirdly, you probably have coomunication wires crossed between driver and Arduino AND/OR this code bar meter pins definition might be wrong.
 *    If you have an consitant animation but thing are not in the right order, either your led mapping (in this code) or your wiring between driver and bar meter are wrong.
 *    If you have the animation but in reverse, try switching the BG_DIRECTION.
 *
 *    When you have the right animation, then transcript to your code :
 *    >>> animation direction
 *    >>> Bar meter segments number and leds mapping matrix
 *    >>> bar meter pins definitions
 *
 ***********************************************************************************************/

#include <Arduino.h>
#include <Wire.h>  // Include the I2C library (required)

#include "BarGraphEngine.h"  // library for bar meter animations

/*********************************************/
/*     BAR METER ANIMATION DIRECTION         */
/*********************************************/
#define FORWARD 0
#define REVERSE 1
/*  DEFINE animation direction : */
const bool BG_DIRECTION = FORWARD;  // animation direction (FORWARD/REVERSE)
// If you feel that you're bar meter animation is on the wrong side,
// try to switch between FORWARD/REVERSE to see if it fix the problem.
/*  DEFINE the animation speed here :  */
const unsigned int SPEED = 250;

/*********************************************/
/*     BAR METER DEFINITION AND MAPPING      */
/*********************************************/
/*  SELECT (uncomment only one) BAR METER SEGMENTS number :     */
//#define SEG10 // for 10 segements bar meter
#define SEG28  // for 28 segments bar meter

// Mapping for 10 segments bar meter
#ifdef SEG10
const uint8_t BARGRAPH_TOTAL_LEDS = 10;
/*  DEFINE segments mapping on bar graph driver {ROW,COL}.    */
const uint8_t BG_SEG_MAP[BARGRAPH_TOTAL_LEDS][2] = {
  { 1, 1 },  // SEG #1
  { 1, 0 },  // SEG #2
  { 0, 7 },  // SEG #3
  { 0, 6 },  // SEG #4
  { 0, 5 },  // SEG #5
  { 0, 4 },  // SEG #6
  { 0, 3 },  // SEG #7
  { 0, 2 },  // SEG #8
  { 0, 1 },  // SEG #9
  { 0, 0 },  // SEG #10
};
#endif

// Mapping for 28 segments bar meter
#ifdef SEG28
const uint8_t BARGRAPH_TOTAL_LEDS = 28;
/* SELECT (uncomment) only one mapping type below  */
//#define MAPPING1 // Common cathode bar meter type SA
#define MAPPING2  // Common anode bar meter type SK

/*  MAPPING 1 matrix definition, more associated with common cathode SA bar meter  */
#ifdef MAPPING1
/*  DEFINE segments mapping on bar graph driver {ROW,COL}.    */
const uint8_t BG_SEG_MAP[BARGRAPH_TOTAL_LEDS][2] = {
  { 0, 0 },  // SEG #1
  { 0, 1 },  // SEG #2
  { 0, 2 },  // SEG #3
  { 0, 3 },  // SEG #4
  { 1, 0 },  // SEG #5
  { 1, 1 },  // SEG #6
  { 1, 2 },  // SEG #7
  { 1, 3 },  // SEG #8
  { 2, 0 },  // SEG #9
  { 2, 1 },  // SEG #10
  { 2, 2 },  // SEG #11
  { 2, 3 },  // SEG #13
  { 3, 0 },  // SEG #12
  { 3, 1 },  // SEG #14
  { 3, 2 },  // SEG #15
  { 3, 3 },  // SEG #16
  { 4, 0 },  // SEG #17
  { 4, 1 },  // SEG #18
  { 4, 2 },  // SEG #19
  { 4, 3 },  // SEG #20
  { 5, 0 },  // SEG #21
  { 5, 1 },  // SEG #22
  { 5, 2 },  // SEG #23
  { 5, 3 },  // SEG #24
  { 6, 0 },  // SEG #25
  { 6, 1 },  // SEG #26
  { 6, 2 },  // SEG #27
  { 6, 3 },  // SEG #28
};

/*  MAPPING 2 matrix definition, more associated with common cathode SK bar meter  */
#elif defined(MAPPING2)
const uint8_t BG_SEG_MAP[BARGRAPH_TOTAL_LEDS][2] = {
  { 0, 0 },  // SEG #1
  { 1, 0 },  // SEG #2
  { 2, 0 },  // SEG #3
  { 3, 0 },  // SEG #4
  { 0, 1 },  // SEG #5
  { 1, 1 },  // SEG #6
  { 2, 1 },  // SEG #7
  { 3, 1 },  // SEG #8
  { 0, 2 },  // SEG #9
  { 1, 2 },  // SEG #10
  { 2, 2 },  // SEG #11
  { 3, 2 },  // SEG #13
  { 0, 3 },  // SEG #12
  { 1, 3 },  // SEG #14
  { 2, 3 },  // SEG #15
  { 3, 3 },  // SEG #16
  { 0, 4 },  // SEG #17
  { 1, 4 },  // SEG #18
  { 2, 4 },  // SEG #19
  { 3, 4 },  // SEG #20
  { 0, 5 },  // SEG #21
  { 1, 5 },  // SEG #22
  { 2, 5 },  // SEG #23
  { 3, 5 },  // SEG #24
  { 0, 6 },  // SEG #25
  { 1, 6 },  // SEG #26
  { 2, 6 },  // SEG #27
  { 3, 6 },  // SEG #28
};
#endif
#endif

/*********************************************/
/*          BAR METER DRIVER DEFINITION      */
/*********************************************/
/*  SELECT (uncomment) ONE DRIVER TYPE :     */
//#define BG_HT16K33 /* I2C LEDs driver like Adafruit backpack, use 2 pins : SDA, SDC */
#define BG_MAX72xx /* MAX7219/7221 use 3 pins serial communication : data, clock, load. */

// BAR METER HT16K33 DRIVER DEFINITION
#ifdef BG_HT16K33
// HT16K33 driver needs I2C address and communication pins defined
// I2C is for fast and short communication distance, no more then a few inches
#define BG_ADDRESS 0x70  // default address for I2C drivers type like HT16K33
#define SDA_PIN A4       // A4 is the default SDA pin for Arduino Nano
#define SCL_PIN A5       // A5 is the default SCL pin for Arduino Nano
// Include the required library for the HT16K33 driver
#include "SBK_HT16K33.h"
HT16K33Driver bargraph(BARGRAPH_TOTAL_LEDS, BG_DIRECTION, SDA_PIN, SCL_PIN, BG_ADDRESS);
#endif

// BAR METER MAX7219/MAX7221 DRIVER DEFINITION
#ifdef BG_MAX72xx
// MAX72xx drivers need serial communication defined with 3 pins, could be any pins.
// Serial communication is more robust then I2C, better if the communication if more then a few inches away.
#define BG_DIN 11   // connected to bar graph driver DataIn pin
#define BG_CLK 12   // connected to bar graph driver Clock pin
#define BG_LOAD 13  // connected to bar graph driver Load pin
// Include the required library for the MAX72xx drivers
#include <LedControl.h>
MAX72xxDriver bargraph(BARGRAPH_TOTAL_LEDS, BG_DIRECTION, BG_DIN, BG_CLK, BG_LOAD);
#endif



//////////////////////////////////////////////////////////////////////////
//////////////////////  ***  SETUP LOOP  ***  ////////////////////////////
//////////////////////////////////////////////////////////////////////////
void setup(void) {

  // setup bar graph
  bargraph.begin(BG_SEG_MAP, BARGRAPH_TOTAL_LEDS, 2);
  bargraph.clear();
  bargraph.update();
}
/******************** END SETUP LOOP ********************/

//////////////////////////////////////////////////////////////////////////
//////////////////////  ***  MAIN LOOP  ***  /////////////////////////////
//////////////////////////////////////////////////////////////////////////
void loop() {

  // Bar meter animation
  // segements will turn on from bottom to top until all ON, then reverse to OFF
  bargraph.testAnimation(SPEED);

  // Update LEDs color setting to last color schemes.
  bargraph.update();
}
