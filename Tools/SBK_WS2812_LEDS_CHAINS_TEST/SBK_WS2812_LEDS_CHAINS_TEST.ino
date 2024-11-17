/**********************************************************************************************
 *    SBK_WS2812_LEDS_CHAINS_TEST is a code to test WS2812 LEDs chains connections and wiring used in many
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
 *    SBK_WS2812_LEDS_CHAINS_TEST
 *    <https://github.com/sbarabe/SBK_PROTONPACK_CORE/tree/main/Tools/SBK_WS2812_LEDS_CHAINS_TEST">
 *    Version 0
 *
 *    1) Line 48 >>> Define your WS2812 PACK LEDs chain Arduino Data PIN
 *    2) Line 49 >>> Define the total LEDs number in the WS2812 PACK LEDs chain
 *    3) Line 56 >>> Define your WS2812 WAND LEDs chain Arduino Data PIN
 *    4) Line 57 >>> Define the total LEDs number in the WS2812 WAND LEDs chain
 *    5) Upload sketch to your Arduino Module and connect your LEDs Data pin
 *    6) Power your circuit and Arduino module
 *
 *    If your wiring, connections and power supply to Arduino and LEDs chains are ok, you will see all the LEDs going from RED to GREEN to BLUE, etc.
 *    Wand animation is twice faster as pacl animation
 *
 ***********************************************************************************************/

#include <Arduino.h>

/***********************************************/
/*                WS2812 LEDs                  */
/***********************************************/
#include <Adafruit_NeoPixel.h>
#include "WS2812TestEngine.h"  // Animations local library

/*********************************************/
/*          PACK WS2812 leds chain           */
/*********************************************/
#define PK_LEDS 2                           // Arduino Data Pin for pack LEDs chain
const uint8_t PACK_TOTAL_LEDS_NUMBER = 45;  // Total number of PIXELS in your LEDs chain
Adafruit_NeoPixel packLeds = Adafruit_NeoPixel(PACK_TOTAL_LEDS_NUMBER, PK_LEDS, NEO_GRB + NEO_KHZ800);
WS2812Test packLedsAnimation(packLeds, 0, (PACK_TOTAL_LEDS_NUMBER - 1));

/*********************************************/
/*          WAND WS2812 leds chain           */
/*********************************************/
#define WD_LEDS 3                           // for wand LEDs chain
const uint8_t WAND_TOTAL_LEDS_NUMBER = 13;  // Wand vent + indicators + firing jewel total WS21812 pixels
Adafruit_NeoPixel wandLeds = Adafruit_NeoPixel(WAND_TOTAL_LEDS_NUMBER, WD_LEDS, NEO_GRB + NEO_KHZ800);
WS2812Test wandLedsAnimation(wandLeds, 0, (WAND_TOTAL_LEDS_NUMBER - 1));

void setup(void) {

  // setup pack's LEDs chain
  packLeds.begin();
  packLeds.setBrightness(255);
  packLeds.clear();
  packLeds.show();
  // setup animations
  packLedsAnimation.begin();

  // setup wand's LEDs chain
  wandLeds.begin();
  wandLeds.setBrightness(255);
  wandLeds.clear();
  wandLeds.show();
  wandLeds.begin();
  // setup animations
  wandLedsAnimation.begin();
}

void loop() {

  // Compute the selected animation progress
  wandLedsAnimation.test(1500);
  // Push WAND animation to WAND LEDs Chain instance
  wandLedsAnimation.update();
  // Show the animation
  wandLeds.show();

  // Compute the selected animation progress
  packLedsAnimation.test(3000);
  // Push PACK animation to PACK LEDs Chain instance
  packLedsAnimation.update();
  // Show the animation
  packLeds.show();
}
