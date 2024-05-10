/**********************************************************************************************
 *    SBK_PROTONPACK_CORE is a code for lights and sound effects of a Proton Pack
 *    replica or other props.
 *    Copyright (c) 2023-2024 Samuel Barabé
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
 *    SBK_PROTONPACK_CORE
 *    <https://github.com/sbarabe/SBK_PROTONPACK_CORE>
 *    Version 2.1
 *
 *    This code was first intended to be use with the SBK Kid Proton Pack
 *    <https://github.com/sbarabe/SBK-KidSizeProtonPackArduino> but the code has been extended to
 *    be a configurable code for any pack.
 *
 *    Lot of time and resources have been invests providing this open-source code, please support
 *    the author if you use or like this code by visiting the GitHub page and donating,
 *    it may help keeping this project alive and up to date. Requests for correcting/adding features
 *    can be made by creating a New Issue on the GitHub page :
 *    <https://github.com/sbarabe/SBK_PROTONPACK_CORE>
 *
 *    Now, only one type of player module and two bar graph drivers are supported, but others could
 *    be added (if requested) without modification to the main code.
 *
 *    Other animations could also be added upon request, but main sketch should also be updated to
 *    use these new animations.
 *
 *    Features included:
 *    - DFPlayer mini sound board.
 *    - Wand BAR GRAPH : up to 28 segments (configurable) controlled by MAX72xx or HT16K33 drivers
 *    - Vents, indicators, powercell and cylcotron are WS2812 led type: there is one defined chain
 *      for the wand (wandLeds), and one chain for the pack (packLeds).
 *    - WS2812 LEDs positions can be set in their header section.
 *    - LEDs animations are defined in object functions. Class objects and functions are in separated
 *      files to keep the main sketch as short as possible.
 *    - There is an option for volume potentiometer that uses software volume control with the audio player :
 *      you can ignore it : the volume will be the one declare in the AUDIO PLAYER configuration section.
 *    - There is the following regular switches and buttons :
 *         Startup switch :     Power on/shutting down the pack by calling starting up / shutting down sequence.
 *         Charging switch :    Safety ON means non shooting, Safety OFF means shooting possible
 *         Theme switch :       for playing themes song instead of the regular pack function.
 *         Fire button :        It does what it says... It also skips to next themes in themes playing mode.
 *         Fire2 button :       Same as fire button but play previous themes in themes playing mode.
 *    - Other options : smoke module and rumble module
 *
 * *************************************************************************************************
 *    HOW TO USE THE CODE :
 *
 *    This code is based on objects programming, objects are defined in their *.h and *.cpp files. Objects
 *    instances are created and used in the core file SBK_PROTONPACK_CORE.ino. There is also a config file,
 *    SBK_CONFIG.h, where all basic parameters can be changed according to hardware used. This fully
 *    compartmented code helps keeping code cleaner and easier to update/maintain. This also prevent having
 *    an endless code hard to follow.
 *
 *    The sketch folder should be named 'SBK_PROTONPACK_CORE', same as the SBK_PROTONPACK_CORE.ino file. That
 *    file is the main file containing the setup and main code loops.
 *
 *    In your sketch folder, you should place SBK_PROTONPACK_CORE.ino file and all *.h files, and *.ccp files :
 *    SBK_CONFIG.h, PlayerEngine.h, PlayerEngine.cpp, BarGraphEngine.h, BarGraphEngine.cpp, etc.
 *
 *    The SBK_CONFIG.h file contains all the definitions and options : pins, player module, LEDs index,
 *    audio tracks, etc. This is the file you want to customize to adjust your pins setting, LEDs chain and index,
 *    audio tracks info and other options. If you like the animations and the pack workflow, you should not have
 *    to change anything else then the config file.
 *
 *    Be careful to adjust animations times/speed regarding your audio tracks/pact states durations. They are all
 *    gathered in the getLEDsSchemeForThisState() function after the Main Loop in the core file. CAUTION, playing
 *    with times and speeds can really mess around the animations flow, it is highly suggested taking notes of the
 *    original values and changing a few of them at the time and see the effects.
 *
 *    Other libraries are required to be installed in your IDE too, according to your devices
 *    and modules :
 *
 *    - For bar graph driver :
 *               MAX71xx >>>   LedControl.h (https://github.com/wayoda/LedControl)
 *               HT16K33 >>>   HT16K33.h (https://github.com/MikeS11/ProtonPack/tree/master/Source/Libraries/ht16k33-arduino-master)
 *    - For WS2812 LEDs :       Adafruit_NeoPixel.h (https://github.com/adafruit/Adafruit_NeoPixel)
 *    - For DFPlayer Pro (not fully tested yet) : DFRobot_DF1201S.h (https://github.com/DFRobot/DFRobot_DF1201S)
 *    - For DFPlayer Mini :     DFRobotDFPlayerMini.h (https://github.com/DFRobot/DFRobotDFPlayerMini)
 *                     or      DFPlayerMine_Fast.h (https://github.com/PowerBroker2/DFPlayerMini_Fast)
 *
 *    *** ADVANCED USERS ONLY ***
 *    If you want to change animations styles and colors, you need to go in the engines files and modified the
 *    associated functions or create new ones. Then you will have to implement them in the getLEDsSchemeForThisState()
 *    function in the core file.
 *
 *    If you want to change the pack state switch/cases workflow, you will have to modify pack states list and audio
 *    tracks list/length/looping in the CONFIG file, and the Main Loop pact states switch/cases contents.
 *
 *    Sketch mechanic works with different pack states and transitions in the Pack state switches/cases defined
 *    in Main Loop. Each pack state has is initialization stage (stageFlag 0) and looping stage (stageFlag 1). The
 *    looping stage includes exit(s) to other stages : switches actions, buttons actions and audio track ending.
 *
 ***********************************************************************************************/

#include <Arduino.h>
#include "SBK_CONFIG.h"
#include <Wire.h> // Include the I2C library (required)

/*********************************************/
/*                                           */
/*      GENERAL definition and helper        */
/*                                           */
/*********************************************/
uint8_t packState = 0;                                         // Initial pack state in the main loop
uint8_t prevPackState = 0;                                     // State tracking for some sequences
uint8_t stageFlag = 0;                                         // stage flag to implement different state stages in main loop
uint8_t prevStageFlag = 0;                                     // stage flag tracking
unsigned long stateStartTime = 0;                              // general time tracker for functions timers and delays
void clearAllLights();                                         // SHUTOFF all leds for wand and pack and resets some trackers
bool checkIfTrackDoneExit(uint8_t track, uint8_t next_state);  // check if a state track is done playing and go to next stage
bool checkIfSwitchExit(bool switch_state, uint8_t next_state); // check if a switch action and go to next stage
bool checkIfTimerExit(uint16_t time, uint8_t next_state);      // check if a timer is done and go to next stage
void getLEDsSchemeForThisState(uint8_t state);                 // to help manage the animations
void playThisStateTrack(uint8_t track, bool looping);          // play state track if themes switch is OFF
void checkPlayModeForThisState(bool looping);                  // check if play mode is correct for this state (looping / not looping)
const bool NOLOOP = false;                                     // helper for audio track looping
const bool LOOP = true;                                        // helper for audio track looping

/*********************************************/
/*           BAR GRAPH & DRIVER(s)           */
/*********************************************/
//  BAR GRAPH with shift registers
//  LedControl.h is used to work with the shift register MAX7219/7221, it should be installed in your IDE.
//  https://github.com/wayoda/LedControl?utm_source=platformio&utm_medium=piohome
//  HT16K33.h library is used for HT16k33 driver. It should be install manually from a zip file in your IDE.
//  https://github.com/MikeS11/ProtonPack/tree/master/Source/Libraries/ht16k33-arduino-master
//  DRIVER should be defined in SBK_CONFIG.h file
//  Bargraph helper variables :
#include "BarGraphEngine.h"
#ifdef BG_MAX72xx
#include <LedControl.h>
MAX72xxDriver bargraph(BARGRAPH_TOTAL_LEDS, BG_DIRECTION, BG_DIN, BG_CLK, BG_LOAD);
#elif defined(BG_HT16K33)
#include <HT16K33.h>
HT16K33Driver bargraph(BARGRAPH_TOTAL_LEDS, BG_DIRECTION, BG_DIN, BG_CLK, BG_ADDRESS);
#endif

/***********************************************/
/*                WS2812 LEDs                  */
/***********************************************/
//  LEDs chains and index should be defined in SBK_CONFIG.h file
#include <Adafruit_NeoPixel.h>

/*********************************************/
/*          PACK WS2812 leds chain           */
/*********************************************/
Adafruit_NeoPixel packLeds = Adafruit_NeoPixel(PACK_TOTAL_LEDS_NUMBER, PK_LEDS, NEO_GRB + NEO_KHZ800);
/***********************************************/
/*                 VENT LEDs                   */
/***********************************************/
#include "VentEngine.h"
Vent packVent(packLeds, VENT_1ST_LED, VENT_LAST_LED);
/***********************************************/
/*               POWERCELL LEDs                */
/***********************************************/
#include "PowercellEngine.h"
Powercell powercell(packLeds, POWERCELL_DIRECTION, POWERCELL_1ST_LED, POWERCELL_LAST_LED);
/***********************************************/
/*               CYCLOTRON LEDs                */
/***********************************************/
#include "CyclotronEngine.h"
/*****************************/
/* >>>>> GB1/GB2 style <<<<< */
/*****************************/
#ifdef GB12
Cyclotron_GB1_GB2 cyclotron(packLeds, CYCLOTRON_DIRECTION,
                            CYC_POS1_1ST_LED, CYC_POS1_LAST_LED,
                            CYC_POS2_1ST_LED, CYC_POS2_LAST_LED,
                            CYC_POS3_1ST_LED, CYC_POS3_LAST_LED,
                            CYC_POS4_1ST_LED, CYC_POS4_LAST_LED);
#endif
/***************************/
/* >>>>> AF/FE style <<<<< */
/***************************/
#ifdef AFFE
Cyclotron_AF_FE cyclotron(packLeds, CYCLOTRON_DIRECTION, CYC_RING_1ST_LED, CYC_RING_LAST_LED);
#endif

/*********************************************/
/*          WAND WS2812 leds chain           */
/*********************************************/
Adafruit_NeoPixel wandLeds = Adafruit_NeoPixel(WAND_TOTAL_LEDS_NUMBER, WD_LEDS, NEO_GRB + NEO_KHZ800);
/***********************************************/
/*             WAND LEDS INDEX                 */
/***********************************************/
#include "IndicatorEngine.h"
#include "RodEngine.h"
// Wand leds objects definition
FiringRod firingRod(wandLeds, WAND_LED_TIP_1ST, WAND_LED_TIP_LAST);
Vent wandVent(wandLeds, WAND_LED_1ST_VENT, WAND_LED_LAST_VENT);
Indicator slowBlowIndicator(wandLeds, WAND_LED_SLOWBLOW);
Indicator topYellowIndicator(wandLeds, WAND_LED_TOP_YELLOW);
Indicator topWhiteIndicator(wandLeds, WAND_LED_TOP_WHITE);
Indicator frontOrangeIndicator(wandLeds, WAND_LED_FRONT_ORANGE);
Indicator firingRodIndicator(wandLeds, WAND_LED_FIRINGROD_YEL);

/*********************************************/
/*    AUDIO PLAYER definition and helpers    */
/*********************************************/
#include "PlayerEngine.h"
bool playing = false;           // variable for playin status
bool cycling = false;           // cylcing single track mode tracker
bool checkPlayerCommandDelay(); // function to check if audio command delay is paste since last play command
unsigned long lastCommand = 0;  // tracker for the last command sent to audio player
/****************************/
/*    PLAYER definitions    */
/****************************/
#ifdef VOL_POT_PIN
const bool VOL_POT = true; // (0=false, 1 = true) activate the volume potentiometer to drive audio board volume by software
#endif
#ifndef VOL_POT_PIN
const bool VOL_POT = false;
#define VOL_POT_PIN 0
#endif
#ifdef DFP_MINI
#include <DFRobotDFPlayerMini.h>
Player_DFPlayerMini player(VOLUME_MAX, VOLUME_START, HW_RX, HW_TX, VOL_POT_PIN, VOL_POT, PLAYER_COMMAND_DELAY); // define player with (min, max ,volume, MCU RX pin, MCU TX pin)
const uint16_t PLAYER_BAUDRATE = 9600;                                                                          // Native baudrate is 9600 for this player.
#elif defined(DFP_MINI_FAST)
#include <DFPlayerMini_Fast.h>
Player_DFPlayerMini_Fast player(VOLUME_MAX, VOLUME_START, HW_RX, HW_TX, VOL_POT_PIN, VOL_POT, PLAYER_COMMAND_DELAY); // define player with (min, max ,volume, MCU RX pin, MCU TX pin)
const uint16_t PLAYER_BAUDRATE = 9600;                                                                               // Native baudrate is 9600 for this player.
#endif
/************************************/
/* Audio board SERIAL COMMUNICATION */
/************************************/
#ifdef ARDUINO_AVR_NANO_EVERY
#define PLAYER_SERIAL1
#endif
#ifdef ARDUINO_AVR_NANO
#include <SoftwareSerial.h>
SoftwareSerial SoftSerial(SW_RX, SW_TX);
#define PLAYER_SOFTSERIAL
#endif
/*****************/
/* THEMES TRACKS */
/*****************/
void checkPlayThemesMode(); // Function for playing themes

/*********************************************/
/*                                           */
/* BUTTONS & SWITCHES definition and helpers */
/*                                           */
/*********************************************/
// For switches and buttons managing
#include "SwitchEngine.h"
Switch SWthemes(THEME_SWITCH_PIN, false);
Switch SWboot(BOOT_SWITCH_PIN, false);
Switch SWcharge(CHARGE_SWITCH_PIN, false);
Switch PBfire(FIRE_BUTTON_PIN, false);
Switch PBrod(ROD_BUTTON_PIN, false);

/*********************************************/
/*            AVAILABLE OPTIONS              */
/*********************************************/
/* SMOKER OPTION */
#include "SmokeEngine.h"
#ifdef SMOKE_RELAY_PIN
Smoker smoker(SMOKE_RELAY_PIN, &SMOKER_MAX_ON_TIME, &SMOKER_MIN_OFF_TIME, true);
#endif
#ifndef SMOKE_RELAY_PIN
Smoker smoker(0, &SMOKER_MAX_ON_TIME, &SMOKER_MIN_OFF_TIME, false);
#endif
/* RUMBLER OPTION */
#include "RumbleEngine.h"
#ifdef RUMBLE_RELAY_PIN
Rumbler rumbler(RUMBLE_RELAY_PIN, &RUMBLER_MAX_ON_TIME, &RUMBLER_MIN_OFF_TIME, true);
#endif
#ifndef RUMBLE_RELAY_PIN
Rumbler rumbler(0, &RUMBLER_MAX_ON_TIME, &RUMBLER_MIN_OFF_TIME, false);
#endif

//////////////////////////////////////////////////////////////////////////
//////////////////////  ***  SETUP LOOP  ***  ////////////////////////////
//////////////////////////////////////////////////////////////////////////
void setup(void)
{
  // Setup Serial.com for troubleshotting OR audio board communication
  if (DEBUG)
  {
    Serial.begin(DEBUG_BAUDRATE);
  }

// Audio player setup
// For Arduino Nano Every, uses Serial1 on D0/D1
// For others, uses Software Serial, pins should be define according to your board
// Baudrate should be set according to your audio player native baudrate.
// Or you could change the player native baudrate to fit your serial communication (see player's doc).
#ifdef PLAYER_SERIAL1
  Serial1.begin(PLAYER_BAUDRATE);
  if (!player.begin(Serial1))
  {
    if (DEBUG)
    {
      Serial.println("Init failed, please check the wire connection!");
    }
  }
#elif defined(PLAYER_SOFTSERIAL)
  SoftSerial.begin(PLAYER_BAUDRATE);
  if (!player.begin(SoftSerial))
  {
    if (DEBUG)
    {
      Serial.println("Init failed, please check the wire connection!");
    }
  }
#endif
  // Enable/disable software voume control with potentiometer
  player.defineVolumePot(VOL_POT_PIN, VOL_POT);
  lastCommand = millis();

  // setup pack's LEDs chain
  packLeds.begin();
  packLeds.setBrightness(255);
  packLeds.clear();
  packLeds.show();
  cyclotron.begin();
  powercell.begin();
  packVent.begin();

  // setup wand's LEDs chain
  wandLeds.begin();
  wandLeds.setBrightness(255);
  wandLeds.clear();
  wandLeds.show();
  wandVent.begin();
  slowBlowIndicator.begin();
  topWhiteIndicator.begin();
  topYellowIndicator.begin();
  frontOrangeIndicator.begin();
  firingRodIndicator.begin();

  // setup bar graph
  bargraph.begin(BG_SEG_MAP, 28, 2);
  bargraph.clear();
  bargraph.update();

  // setup for the switches/buttons
  SWthemes.begin();
  SWboot.begin();
  SWcharge.begin();
  PBfire.begin();
  PBrod.begin();

  // Smoker setup
  smoker.begin();

  // Sumbler setup
  rumbler.begin();
}
/******************** END SETUP LOOP ********************/

//////////////////////////////////////////////////////////////////////////
//////////////////////  ***  MAIN LOOP  ***  /////////////////////////////
//////////////////////////////////////////////////////////////////////////
void loop()
{
  // Troubleshooting info on pack states and stages
  if (DEBUG)
  {
    if (packState != prevPackState || stageFlag != prevStageFlag)
    {
      Serial.print("Pack State = "), Serial.print(packState);
      Serial.print("  Stage = "), Serial.println(stageFlag);
      if (packState != prevPackState)
      {
        prevPackState = packState;
      }
      if (stageFlag != prevStageFlag)
      {
        prevStageFlag = stageFlag;
      }
    }
  }

  // Update LEDs color setting to last color schemes.
  bargraph.update();
  cyclotron.update();
  powercell.update();
  wandVent.update();
  packVent.update();
  firingRod.update();
  // Update LEDs chains with last color schemes.
  wandLeds.show();
  packLeds.show();

  // Check buttons and switches readings and states
  SWthemes.getState();
  SWboot.getState();
  SWcharge.getState();
  PBfire.getState();
  PBrod.getState();

  // Update smoker and rumbler
  smoker.update();
  rumbler.update();

  // Set audio volume with potentiometer
  player.setVolWithPot();

  ///////////////////////////////////////////////////////////////
  // Actions for different packs states
  switch (packState)
  {

  //////////////////////////////////////////////
  case STATE_PWD_DOWN:

    switch (stageFlag)
    {

    // Initiate this pack State :
    case 0:
      if (DEBUG)
      {
        Serial.println("STATE_PWD_DOWN");
      }
      playThisStateTrack(packState, TRACK_LOOPING[packState]); //  Stop sound effects :
      getLEDsSchemeForThisState(packState);                    // Inititate state LEDs animations
      rumbler.rumbleOFF();
      smoker.smokeOFF();
      stateStartTime = millis(); // Register state start time
      stageFlag = 1;             // End state initialization
      break;

    // This pack state loop :
    case 1:
      checkPlayModeForThisState(TRACK_LOOPING[packState]); // Set playmode for this stage : this pack state is transient, so no looping...
      getLEDsSchemeForThisState(packState);                // Pack state LEDs animations
      checkPlayThemesMode();                               // Cut off sound effects if themes switch is ON
      checkIfSwitchExit(SWboot.isON(), STATE_BOOTING);     // Pack state exit : check if the pack is booting
      break;
    }

    break;

  //////////////////////////////////////////////
  case STATE_BOOTING:

    switch (stageFlag)
    {

    // Initiate this pack State :
    case 0:
      if (DEBUG)
      {
        Serial.println("STATE_BOOTING");
      }
      playThisStateTrack(packState, TRACK_LOOPING[packState]); // Play sound FX track only if themes switch is OFF
      getLEDsSchemeForThisState(packState);                    // Inititate state LEDs animations
      rumbler.rumbleOFF();
      smoker.smokeOFF();
      stateStartTime = millis(); // Register state start time
      stageFlag = 1;             // End state initialization
      break;

    // This pack state loop :
    case 1:
      checkPlayModeForThisState(TRACK_LOOPING[packState]);         // Set playmode for this stage : this pack state is transient, so no cyling...
      getLEDsSchemeForThisState(packState);                        // Pack state LEDs animations
      checkPlayThemesMode();                                       // Cut off sound effects if themes switch is ON
      if (!checkIfSwitchExit(!SWboot.isON(), STATE_SHUTTING_DOWN)) // Pack state exits : check if the pack is shutting down
      {
        // Then check is pact state track is done playing and exit to next pack state
        checkIfTrackDoneExit(packState, STATE_IDLING_UNLOADED);
      }
      break;
    }

    break;

  //////////////////////////////////////////////
  case STATE_IDLING_UNLOADED:

    switch (stageFlag)
    {

    // Initiate this pack State :
    case 0:
      if (DEBUG)
      {
        Serial.println("STATE_IDLING_UNLOADED");
      }
      playThisStateTrack(packState, TRACK_LOOPING[packState]); // Play sound FX track only if themes switch is OFF
      getLEDsSchemeForThisState(packState);                    // Inititate state LEDs animations
      rumbler.rumbleOFF();
      smoker.smokeOFF();
      stateStartTime = millis(); // Register state start time
      stageFlag = 1;             // End state initialization
      break;

    // This pack state loop :
    case 1:
      checkPlayModeForThisState(TRACK_LOOPING[packState]);         // Set Playmode for this state : this state needs a looping track
      getLEDsSchemeForThisState(packState);                        // Pack state LEDs animations
      checkPlayThemesMode();                                       // Cut off sound effects if themes switch is ON
      if (!checkIfSwitchExit(!SWboot.isON(), STATE_SHUTTING_DOWN)) // Pack state exits : check if the pack is shutting down
      {
        checkIfSwitchExit(SWcharge.isON(), STATE_CHARGING); // Then check if safety switch is turned OFF : calling charging up sequence
      }
      break;
    }

    break;

  //////////////////////////////////////////////
  case STATE_IDLING_CHARGED:

    switch (stageFlag)
    {

    // Initiate this pack State :
    case 0:
      if (DEBUG)
      {
        Serial.println("STATE_IDLING_CHARGED");
      }
      playThisStateTrack(packState, TRACK_LOOPING[packState]); // Play sound FX track only if themes switch is OFF
      getLEDsSchemeForThisState(packState);                    // Inititate state LEDs animations
      rumbler.rumbleOFF();
      smoker.smokeOFF();
      stateStartTime = millis(); // Register state start time
      stageFlag = 1;             // End state initialization
      break;

    // This pack state loop :
    case 1:
      checkPlayModeForThisState(TRACK_LOOPING[packState]);         // Set Playmode for this state : this state is not transient, so cylcing is ON
      getLEDsSchemeForThisState(packState);                        // Pack state LEDs animations
      checkPlayThemesMode();                                       // Cut off sound effects if themes switch is ON
      if (!checkIfSwitchExit(!SWboot.isON(), STATE_SHUTTING_DOWN)) // Pack state exits : check if the pack is shutting down
      {
        if (!checkIfSwitchExit(!SWcharge.isON(), STATE_UNLOADING)) // then check if charging switch is OFF, go to unloarding state
        {
          checkIfSwitchExit((PBfire.isON() || PBrod.isON()), STATE_FIRING_RAMP); // then check if fire buttons are ON, go to firing state
        }
        break;
      }
    }

    break;

  //////////////////////////////////////////////
  case STATE_CHARGING:

    switch (stageFlag)
    {

    // Initiate this pack State :
    case 0:
      if (DEBUG)
      {
        Serial.println("STATE_CHARGING");
      }
      playThisStateTrack(packState, TRACK_LOOPING[packState]); // Play sound FX track only if themes switch is OFF
      getLEDsSchemeForThisState(packState);                    // Inititate state LEDs animations
      rumbler.rumbleOFF();
      smoker.smokeOFF();
      stateStartTime = millis(); // Register state start time
      stageFlag = 1;             // End state initialization
      break;

    // This pack state loop :
    case 1:
      checkPlayModeForThisState(TRACK_LOOPING[packState]);         // Set Playmode for this state : this is a transient stage, turn off cycling mode
      getLEDsSchemeForThisState(packState);                        // Pack state LEDs animations
      checkPlayThemesMode();                                       // Cut off sound effects if themes switch is ON
      if (!checkIfSwitchExit(!SWboot.isON(), STATE_SHUTTING_DOWN)) // Pack state exits : check if the pack is shutting down
      {
        if (!checkIfSwitchExit(!SWcharge.isON(), STATE_UNLOADING)) // Then check if safety backed ON
        {
          checkIfTrackDoneExit(packState, STATE_IDLING_CHARGED); // Then check is pact state track is done playing and exit to next pack state
        }
      }
      break;
    }

    break;

  //////////////////////////////////////////////
  case STATE_UNLOADING:

    switch (stageFlag)
    {

    // Initiate this pack State :
    case 0:
      if (DEBUG)
      {
        Serial.println("STATE_UNLOADING");
      }
      playThisStateTrack(packState, TRACK_LOOPING[packState]); // Play sound FX track only if themes switch is OFF
      getLEDsSchemeForThisState(packState);                    // Inititate state LEDs animations
      rumbler.rumbleOFF();
      smoker.smokeOFF();
      stateStartTime = millis(); // Register state start time
      stageFlag = 1;             // End state initialization
      break;

    // This pack state loop :
    case 1:
      checkPlayModeForThisState(TRACK_LOOPING[packState]);         // Set Playmode for this state : this is a transient stage, turn off cycling mode
      getLEDsSchemeForThisState(packState);                        // Pack state LEDs animations
      checkPlayThemesMode();                                       // Cut off sound effects if themes switch is ON
      if (!checkIfSwitchExit(!SWboot.isON(), STATE_SHUTTING_DOWN)) // Pack state exits : check if the pack is shutting down
      {
        if (!checkIfSwitchExit(SWcharge.isON(), STATE_CHARGING)) // Then check if safety backed OFF
        {
          checkIfTrackDoneExit(packState, STATE_IDLING_UNLOADED); // Then check is pact state track is done playing and exit to next pack state
        }
      }
      break;
    }

    break;

  //////////////////////////////////////////////
  case STATE_FIRING_RAMP:

    switch (stageFlag)
    {

    // Initiate this pack State :
    case 0:
      if (DEBUG)
      {
        Serial.println("STATE_FIRING_RAMP");
      }
      playThisStateTrack(packState, TRACK_LOOPING[packState]); // Play sound FX track only if themes switch is OFF
      getLEDsSchemeForThisState(packState);                    // Inititate state LEDs animations
      rumbler.rumbleON();                                      // Start rumbler motor if not already start AND minimum off time delay respected
      smoker.smokeOFF();
      stateStartTime = millis(); // Register state start time
      stageFlag = 1;             // End state initialization
      break;

    // This pack state loop :
    case 1:
      checkPlayModeForThisState(TRACK_LOOPING[packState]);         // Set Playmode for this state : this is a transient stage, turn off cycling mode
      getLEDsSchemeForThisState(packState);                        // Pack state LEDs animations
      rumbler.rumbleON();                                          // Start rumbler motor if not already start AND minimum off time delay respected
      checkPlayThemesMode();                                       // Cut off sound effects if themes switch is ON
      if (!checkIfSwitchExit(!SWboot.isON(), STATE_SHUTTING_DOWN)) // Pack state exits : check if the pack is shutting down
      {
        if (!checkIfSwitchExit((!PBfire.isON() && !PBrod.isON()), STATE_TAIL)) // Then check if firing buttons are released, pack goes in tail state
        {
          checkIfTrackDoneExit(packState, STATE_FIRING_MAX); // Then check is pact state track is done playing and exit to next pack state
        }
      }
      break;
    }

    break;

    //////////////////////////////////////////////
  case STATE_FIRING_MAX:

    switch (stageFlag)
    {

    // Initiate this pack State :
    case 0:
      if (DEBUG)
      {
        Serial.println("STATE_FIRING_MAX");
      }
      playThisStateTrack(packState, TRACK_LOOPING[packState]); // Play sound FX track only if themes switch is OFF
      getLEDsSchemeForThisState(packState);                    // Inititate state LEDs animations
      rumbler.rumbleON();                                      // Start rumbler motor if not already start AND minimum off time delay respected
      smoker.smokeOFF();
      stateStartTime = millis(); // Register state start time
      stageFlag = 1;             // End state initialization
      break;

    // This pack state loop :
    case 1:
      checkPlayModeForThisState(TRACK_LOOPING[packState]);         // Set Playmode for this state : this is a transient stage, turn off cycling mode
      getLEDsSchemeForThisState(packState);                        // Pack state LEDs animations
      rumbler.rumbleON();                                          // Start rumbler motor if not already start AND minimum off time delay respected
      checkPlayThemesMode();                                       // Cut off sound effects if themes switch is ON
      if (!checkIfSwitchExit(!SWboot.isON(), STATE_SHUTTING_DOWN)) // Pack state exits : check if the pack is shutting down
      {
        if (!checkIfSwitchExit((!PBfire.isON() && !PBrod.isON()), STATE_TAIL)) // Then check if firing buttons are released, pack goes in tail state
        {
          if (!checkIfSwitchExit((PBfire.isON() && PBrod.isON()), STATE_FIRING_OVERHEAT)) // Then check if both firing buttons are pressed, pack goes in overheat firing
          {
            checkIfTimerExit(FIRING_DURATION, STATE_FIRING_OVERHEAT); // Then check if firint timer is done and goes in overheat firing
          }
        }
      }
      break;
    }

    break;

  //////////////////////////////////////////////
  case STATE_FIRING_OVERHEAT:

    switch (stageFlag)
    {

    // Initiate this pack State :
    case 0:
      if (DEBUG)
      {
        Serial.println("STATE_FIRING_OVERHEAT");
      }
      playThisStateTrack(packState, TRACK_LOOPING[packState]); // Play sound FX track only if themes switch is OFF
      getLEDsSchemeForThisState(packState);                    // Inititate state LEDs animations
      rumbler.rumbleON();                                      // Start rumbler motor if not already start AND minimum off time delay respected
      smoker.smokeON();                                        // Start smoke module if not already start AND minimum off time delay respected
      stateStartTime = millis();                               // Register state start time
      stageFlag = 1;                                           // End state initialization
      break;

    // This pack state loop :
    case 1:
      checkPlayModeForThisState(TRACK_LOOPING[packState]);         // Set Playmode for this state : this is a transient stage, turn off cycling mode
      getLEDsSchemeForThisState(packState);                        // Pack state LEDs animations
      rumbler.rumbleON();                                          // Start rumbler motor if not already start AND minimum off time delay respected
      smoker.smokeON();                                            // Start smoke module if not already start AND minimum off time delay respected
      checkPlayThemesMode();                                       // Cut off sound effects if themes switch is ON
      if (!checkIfSwitchExit(!SWboot.isON(), STATE_SHUTTING_DOWN)) // Pack state exits : check if the pack is shutting down
      {
        if (!checkIfSwitchExit((!PBfire.isON() && !PBrod.isON()), STATE_OVERHEATED)) // Then check if firing buttons are released, pack goes in in overheated state
        {
          checkIfTrackDoneExit(packState, STATE_OVERHEATED); // Then check is pact state track is done playing and exit to next pack state
        }
      }
      break;
    }

    break;

  //////////////////////////////////////////////
  case STATE_TAIL:

    switch (stageFlag)
    {

    // Initiate this pack State :
    case 0:
      if (DEBUG)
      {
        Serial.println("STATE_TAIL");
      }
      playThisStateTrack(packState, TRACK_LOOPING[packState]); // Play sound FX track only if themes switch is OFF
      getLEDsSchemeForThisState(packState);                    // Inititate state LEDs animations
      rumbler.rumbleOFF();
      smoker.smokeOFF();
      stateStartTime = millis(); // Register state start time
      stageFlag = 1;             // End state initialization
      break;

    // This pack state loop :
    case 1:
      checkPlayModeForThisState(TRACK_LOOPING[packState]);         // Set Playmode for this state : this is a transient stage, turn off cycling mode
      getLEDsSchemeForThisState(packState);                        // Pack state LEDs animations
      checkPlayThemesMode();                                       // Cut off sound effects if themes switch is ON
      if (!checkIfSwitchExit(!SWboot.isON(), STATE_SHUTTING_DOWN)) // Pack state exits : check if the pack is shutting down
      {
        checkIfTrackDoneExit(packState, STATE_IDLING_CHARGED); // Then check is pact state track is done playing and exit to next pack state
      }
      break;
    }

    break;

  //////////////////////////////////////////////
  case STATE_OVERHEATED:

    switch (stageFlag)
    {

    // Initiate this pack State :
    case 0:
      if (DEBUG)
      {
        Serial.println("STATE_OVERHEATED");
      }
      playThisStateTrack(packState, TRACK_LOOPING[packState]); // Play sound FX track only if themes switch is OFF
      getLEDsSchemeForThisState(packState);                    // Inititate state LEDs animations
      smoker.smokeON();                                        // Start smoke module if not already start AND minimum off time delay respected
      stateStartTime = millis();                               // Register state start time
      stageFlag = 1;                                           // End state initialization
      break;

    // This pack state loop :
    case 1:
      checkPlayModeForThisState(TRACK_LOOPING[packState]);         // Set Playmode for this state : this is a transient stage, turn off cycling mode
      getLEDsSchemeForThisState(packState);                        // Pack state LEDs animations
      smoker.smokeON();                                            // Start smoke module if not already start AND minimum off time delay respected
      checkPlayThemesMode();                                       // Cut off sound effects if themes switch is ON
      if (!checkIfSwitchExit(!SWboot.isON(), STATE_SHUTTING_DOWN)) // Pack state exits : check if the pack is shutting down
      {
        checkIfTrackDoneExit(packState, STATE_IDLING_CHARGED); // Then check is pact state track is done playing and exit to next pack state
      }
      break;
    }

    break;

  //////////////////////////////////////////////
  case STATE_SHUTTING_DOWN:

    switch (stageFlag)
    {

    // Initiate this pack State :
    case 0:
      if (DEBUG)
      {
        Serial.println("STATE_SHUTTING_DOWN");
      }
      playThisStateTrack(packState, TRACK_LOOPING[packState]); // Play sound FX track only if themes switch is OFF
      getLEDsSchemeForThisState(packState);                    // Inititate state LEDs animations
      rumbler.rumbleOFF();
      smoker.smokeOFF();
      stateStartTime = millis(); // Register state start time
      stageFlag = 1;             // End state initialization
      break;

    // This pack state loop :
    case 1:
      checkPlayModeForThisState(TRACK_LOOPING[packState]);  // Set Playmode for this state : this is a transient stage, turn off cycling mode
      getLEDsSchemeForThisState(packState);                 // Pack state LEDs animations
      checkPlayThemesMode();                                // Cut off sound effects if themes switch is ON
      if (!checkIfSwitchExit(SWboot.isON(), STATE_BOOTING)) // Pack state exits : check if the pack is booting up
      {
        checkIfTrackDoneExit(packState, STATE_PWD_DOWN); // Then check is pact state track is done playing and exit to next pack state
      }
      break;
    }

    break;
  }
  // END Actions for different packs states
  ///////////////////////////////////////////////////////////////
}
/********************** END MAIN LOOP *******************/

//////////////////////////////////////////////////////////////////////////
///////////////////// *** HELPER FUNCTIONS *** ///////////////////////////
//////////////////////////////////////////////////////////////////////////

/*******************************************/
/*   ANIMATIONS functions in pack states   */
/*******************************************/
void getLEDsSchemeForThisState(uint8_t state)
{
  // Some sequences need to be initialized with pack state, stageFlag is used to know if pack state is in initialization
  bool init;
  if (stageFlag == 0) // Pack state in initialization
    init = true;
  else // Pack state already initialized
    init = false;

  switch (state)
  {
  case STATE_PWD_DOWN:
    if (init)
    {
      clearAllLights(); // clear cyclotron, powercell, vent and wands LEDS
    }
    // To show that power is still on on the pack, show some minimum ligths
    topWhiteIndicator.green(INDICATOR_SLOW_FLASH); // set WAND_LED_TOP_WHITE led green flashing
    powercell.poweredDown();                       // first led blinking slowly
    break;

  case STATE_BOOTING:
    if (init)
    {
      frontOrangeIndicator.clear();
      topWhiteIndicator.clear();
      firingRodIndicator.clear();
      topYellowIndicator.clear();
      wandVent.clear();
    }
    slowBlowIndicator.red(INDICATOR_FAST_FLASH);
    cyclotron.rampToIdleOne(3000, init);
    powercell.boot(3000, init);
    bargraph.boot(50, 50, init);
    packVent.boot(3000, false); // to finish shutdown sequence if boot switch was turned ON when venting are not done
    break;

  case STATE_IDLING_UNLOADED:
    if (init)
    {
      frontOrangeIndicator.clear();
      topWhiteIndicator.clear();
      firingRodIndicator.clear();
      topYellowIndicator.clear();
      wandVent.clear();
      packVent.clear();
    }
    slowBlowIndicator.red(0);
    cyclotron.rampToIdleOne(0, false); // just idling, no ramping
    powercell.rampToIdleOne(0, false); // just idling, no ramping
    bargraph.idleOne(50);
    break;

  case STATE_IDLING_CHARGED:
    if (init)
    {
      firingRodIndicator.clear();
      topYellowIndicator.clear();
      packVent.clear();
    }
    frontOrangeIndicator.orange(0);
    topWhiteIndicator.white(0);
    slowBlowIndicator.red(0);
    cyclotron.rampToIdleTwo(0, false); // just idling, no ramping
    powercell.rampToIdleTwo(0, false); // just idling, no ramping
    bargraph.idleTwo(70);
    wandVent.rampToCoolBlue(0, false); // no ramping, just set to cool blue
    break;

  case STATE_CHARGING:
    if (init)
    {
      firingRodIndicator.clear();
      topYellowIndicator.clear();
      frontOrangeIndicator.clear();
      packVent.clear();
    }
    topWhiteIndicator.white(INDICATOR_FAST_FLASH);
    slowBlowIndicator.red(0);
    powercell.rampToIdleTwo(2500, init);
    cyclotron.rampToIdleTwo(2500, init);
    bargraph.idleOne(50);
    wandVent.rampToCoolBlue(2500, init);
    break;

  case STATE_UNLOADING:
    if (init)
    {
      firingRodIndicator.clear();
      topYellowIndicator.clear();
      frontOrangeIndicator.clear();
      packVent.clear();
    }
    topWhiteIndicator.white(INDICATOR_FAST_FLASH);
    slowBlowIndicator.red(0);
    powercell.rampToIdleOne(2500, init);
    cyclotron.rampToIdleOne(2500, init);
    bargraph.idleTwo(70);
    wandVent.fadeOut(2500, init);
    break;

  case STATE_FIRING_RAMP:
    if (init)
    {
      topYellowIndicator.clear();
    }
    topWhiteIndicator.white(0);
    firingRodIndicator.yellow(0);
    frontOrangeIndicator.orange(0);
    slowBlowIndicator.red(0);
    cyclotron.rampToFiring(5000, init);
    powercell.rampToFiring(5000, init);
    bargraph.firing(50);
    packVent.warming(20000, init);
    firingRod.fireStrobe(20);
    break;

  case STATE_FIRING_MAX:
    if ((millis() - stateStartTime) >= FIRING_WARNING_DELAY)
    {
      topYellowIndicator.yellow(INDICATOR_MEDIUM_FLASH);
    }
    topWhiteIndicator.white(0);
    firingRodIndicator.yellow(0);
    frontOrangeIndicator.orange(0);
    slowBlowIndicator.red(0);
    cyclotron.rampToFiring(5000, false); // already initialized in STATE_FIRING_RAMP
    powercell.rampToFiring(5000, false); // already initialized in STATE_FIRING_RAMP
    bargraph.firing(50);
    packVent.warming(20000, false); // already initialized in STATE_FIRING_RAMP
    firingRod.fireStrobe(20);
    break;

  case STATE_FIRING_OVERHEAT:
    topYellowIndicator.red(INDICATOR_FAST_FLASH); // keep flashing from same pace as previous stage
    topWhiteIndicator.white(0);
    firingRodIndicator.yellow(0);
    frontOrangeIndicator.orange(0);
    slowBlowIndicator.red(0);
    cyclotron.rampToFiring(5000, false); // Sequence initialized in STATE_FIRING_RAMP
    powercell.rampToFiring(5000, false); // Sequence initialized in STATE_FIRING_RAMP
    bargraph.firing(50);
    packVent.warming(15000, false); // Sequence initialized in STATE_FIRING_RAMP
    firingRod.fireStrobe(20);
    break;

  case STATE_TAIL:
    if (init)
    {
      topYellowIndicator.clear();
      firingRodIndicator.clear();
    }
    topWhiteIndicator.white(0);
    frontOrangeIndicator.orange(INDICATOR_MEDIUM_FLASH);
    slowBlowIndicator.red(0);
    firingRod.tail(1500);
    powercell.rampToIdleTwo(2000, init);
    cyclotron.rampToIdleTwo(2000, init);
    bargraph.idleTwo(70);
    packVent.cooling(2000, init);
    break;

  case STATE_OVERHEATED:
    if (init)
    {
      firingRodIndicator.clear();
    }
    frontOrangeIndicator.orange(INDICATOR_MEDIUM_FLASH);
    topYellowIndicator.red(INDICATOR_FAST_FLASH);
    slowBlowIndicator.red(0);
    firingRod.tail(1500);
    powercell.rampToIdleTwo(5000, init);
    cyclotron.rampToIdleTwo(5000, init);
    bargraph.idleTwo(70);
    packVent.cooling(5000, init);
    break;

  case STATE_SHUTTING_DOWN:
    if (init)
    {
      topWhiteIndicator.clear();
      topYellowIndicator.clear();
      firingRodIndicator.clear();
      frontOrangeIndicator.clear();
      wandVent.clear();
    }
    slowBlowIndicator.red(INDICATOR_FAST_FLASH);
    powercell.shuttingDown(3000, init);
    cyclotron.rampToPoweredDown(3000, init);
    bargraph.shuttingDown(50, init);
    packVent.shutdown(3000, init);
    break;
  }
}

/*void templateLightsScheme(uint8_t state, bool init)
{
  switch (state)
  {
  case STATE_PWD_DOWN:
if (init)
    {
    }
    break;
  case STATE_BOOTING:
if (init)
    {
    }
    break;
  case STATE_IDLING_UNLOADED:
if (init)
    {
    }
    break;
  case STATE_IDLING_CHARGED:
    if (init)
    {
    }
    break;
  case STATE_CHARGING:
if (init)
    {
    }
    break;
  case STATE_UNLOADING:
if (init)
    {
    }
    break;
  case STATE_FIRING_RAMP:
if (init)
    {
    }
    break;
  case STATE_FIRING_MAX:
if (init)
    {
    }
    break;

  case STATE_FIRING_OVERHEAT:
    if (init)
    {
    }
    break;
  case STATE_TAIL:
    if (init)
    {
    }
    break;
  case STATE_OVERHEATED:
    if (init)
    {
    }

    break;
  case STATE_SHUTTING_DOWN:
    if (init)
    {
    }
  }
}
 */

//  END of animations functions in pack states
////////////////////////////////////////////////

void clearAllLights()
{
  // Clear leds
  cyclotron.clear();
  powercell.clear();
  bargraph.clear();
  packVent.clear();
  wandVent.clear();
  slowBlowIndicator.clear();
  topWhiteIndicator.clear();
  topYellowIndicator.clear();
  frontOrangeIndicator.clear();
  firingRodIndicator.clear();
  firingRod.clear();

  // Reset trackers
}

void checkPlayThemesMode()
{
  static bool themes = false;
  // initiate themes playing
  if (!themes && SWthemes.isON() && checkPlayerCommandDelay())
  {
    lastCommand = millis();
    player.setThemesPlaymode();
    themes = true;
  }

  if (themes)
  {
    // Check Fire button to play next theme, needs a press and release
    static unsigned long pbfirePrev = 0;
    if (PBfire.toggleON())
    {
      pbfirePrev = millis();
    }
    if ((PBfire.toggleOFF()) && (millis() - pbfirePrev < 1000))
    {
      lastCommand = millis();
      player.next();
    }

    // Check rod button to play previous theme, needs a press and release
    static unsigned long pbrodPrev = 0;
    if (PBrod.toggleON())
    {
      pbrodPrev = millis();
    }
    if ((PBrod.toggleOFF()) && (millis() - pbrodPrev < 1000))
    {
      lastCommand = millis();
      player.previous();
    }

    // Exit of themes playing, restart playing Pack State TRack
    if (!SWthemes.isON())
    {

      playThisStateTrack(packState, TRACK_LOOPING[packState]);
      themes = false;
    }
  }
}

bool checkIfTrackDoneExit(uint8_t track, uint8_t next_state)
{
  if (!player.isPlaying() || ((millis() - stateStartTime) >= max(0, TRACK_LENGTH[track] - AUDIO_ADVANCE)))
  {
    packState = next_state;
    stageFlag = 0;
    return true;
  }
  else
  {
    return false;
  }
}

bool checkIfSwitchExit(bool switch_state, uint8_t next_state)
{
  if (switch_state && checkPlayerCommandDelay())
  {
    packState = next_state;
    stageFlag = 0;
    return true;
  }
  else
  {
    return false;
  }
}

bool checkIfTimerExit(uint16_t time, uint8_t next_state)
{
  if (millis() - stateStartTime >= time)
  {
    packState = next_state;
    stageFlag = 0;
    return true;
  }
  else
  {
    return false;
  }
}

bool checkPlayerCommandDelay()
{
  if (millis() - lastCommand > PLAYER_COMMAND_DELAY)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void playThisStateTrack(uint8_t track, bool looping)
{

  if (!SWthemes.isON())
  {
    lastCommand = millis();
    if (track == STATE_PWD_DOWN) // Pack is in powered down state
      player.stop();             // no sound effect
    else
    {
      if (looping)
      {
        player.loopFileNum(track);
      }
      else
      {
        player.playFileNum(track, TRACK_LENGTH[track]);
      }
    }
  }
}

void checkPlayModeForThisState(bool looping)
{
  if (looping)
  {
    if (!cycling && checkPlayerCommandDelay())
    { // Enable looping
      lastCommand = millis();
      player.setCyclingTrackPlaymode();
      cycling = true;
    }
  }
  else
  {
    if (cycling && checkPlayerCommandDelay())
    { // Disable looping
      lastCommand = millis();
      player.setSinglePlaymode();
      cycling = false;
    }
  }
}
