/*
 *  SBK_CONFIG.h is a part of SBK_PROTONPACK_CORE (VERSION 2.1) code for sound effects and animations of a Proton Pack replica
 *  Copyright (c) 2023-2024 Samuel Barabé
 *
 *  See this page for reference <https://github.com/sbarabe/SBK_PROTONPACK_CORE>.
 *
 *  SBK_PROTONPACK_CORE is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Foundation, either version
 *  3 of the License, or (at your option) any later version.
 *
 *  SBK_PROTONPACK_CORE is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
 *  the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with Foobar. If not,
 *  see <https://www.gnu.org/licenses/>
 */

#ifndef SBK_CONFIG_H
#define SBK_CONFIG_H

#include <Arduino.h>

/*********************************************/
/*                                           */
/*            GENERAL definitions            */
/*                                           */
/*********************************************/
#define FORWARD 0 // animation direction
#define REVERSE 1 // animation direction
/* DEBUG information about pack states and stages, set to 'true' to send info to Serial monitor, */
/* set to false to save memory and optimise MCU speed */
bool const DEBUG = false;
#define DEBUG_BAUDRATE 115200 // Or the usual 9600

/*********************************************/
/*                                           */
/*          MCU PINS CONFIGURATION           */
/*                                           */
/*********************************************/
// Update the pins according to your MCU and connections
// ARDUINO (NANO EVERY) PINS DEFINITION
// Hardware Serial for serial communication with audio board
#define HW_RX 0              // to audio board Tx pin, for Nano Every Serial1
#define HW_TX 1              // to audio board Rx pin, for Nano Every Serial1
#define SW_RX 11              // If Software Serial is used, to audio board Tx pin
#define SW_TX 10              // If Software Serial is used,to audio board Rx pin
#define PK_LEDS 2           // for pack LEDs chain
#define BG_DIN A4             // connected to bar graph driver DataIn pin / SDA in case of I2C driver
#define BG_CLK A5            // connected to bar graph driver Clock pin / SCL in cas of I2C driver
#define BG_LOAD A6           // connected to bar graph driver Load pin, not used for I2C driver
// #define BUSY_PIN 12      // Audio player BUSY pin - NOT USE IN THIS CODE
#define WD_LEDS 3            // for wand LEDs chain
#define ROD_BUTTON_PIN 5    // same as fire button, but plays previous theme when pressed and released in "Themes mode"
#define FIRE_BUTTON_PIN 4   // if in Idle Two state, fires gun. Also plays next theme when pressed and released in "Themes mode"
#define CHARGE_SWITCH_PIN 6 // Goes from Idle One state to Idle Two state : gun can be fire when in Idle two state
#define BOOT_SWITCH_PIN 7   // Goes from power down to Idle One state, or powered down when switch off from any state
#define THEME_SWITCH_PIN 8  // Puts the pack in music mode, playings themes one after the other. Use fire/rod buttons to switch themes.
// Those pin are the REQUIRED pins for pack CORE as it's coded in this unmodified version.
// Others pins for options are defined later in the config file : smoker, rumbler, volume potentiometer.

/*********************************************/
/*          OPTION :SMOKE MACHINE            */
/*********************************************/
/* UNCOMMENT and DEFINE PIN if smoke module exist */
// #define SMOKE_RELAY_PIN 9     /* signal to relays for smoke module (fan and smoke) */
/* DEFINE the maximum time that the smoker can be ON */
const uint32_t SMOKER_MAX_ON_TIME = 15000; // in ms
/* DEFINE the minimum OFF time of the smoker, ti prevent short cylcing */
const uint32_t SMOKER_MIN_OFF_TIME = 2000; // in ms
/* Smoker is activated, if minimum off time is respected, when the pack goes into */
/* STATE_FIRING_OVERHEAT and stop after STATE_OVERHEATED tail */

/*********************************************/
/*          OPTION : RUMBLE MOTOR            */
/*********************************************/
/* UNCOMMENT and DEFINE PIN number if rumbler motor module exist */
// #define RUMBLER_RELAY_PIN 13     /* signal to rumbler motor relay */
/* DEFINE the maximum time that the rumbler can be ON */
const uint32_t RUMBLER_MAX_ON_TIME = 15000; // in ms
/* DEFINE rumbler minimum OFF time to prevent short cylcing */
const uint32_t RUMBLER_MIN_OFF_TIME = 2000; // in ms
                                            /* Rumbler is activated, if minimum off time is respected, when the pack goes into firing */

/*********************************************/
/*           BAR GRAPH & DRIVER(s)           */
/*********************************************/
/*  SELECT (uncomment) ONE DRIVER TYPE :     */
#define BG_HT16K33 /* I2C LEDs driver like Adafruit backpack, use 2 pins : SDA, SDC */
// #define BG_MAX72xx /* MAX7219/7221 use 3 pins serial communication : data, clock, load. */
/* DEFINE DRIVER I2C ADDRESS IF REQUIRED     */
#define BG_ADDRESS 0x70 // for I2C drivers type
/*  DEFINE the total segments number on bar graphe :     */
const uint8_t BARGRAPH_TOTAL_LEDS = 28;
/*  DEFINE animation direction : */
const bool BG_DIRECTION = FORWARD; // animation direction (FORWARD/REVERSE)
/*  DEFINE segments mapping on bar graph driver {ROW,COL}.    */
/*  This mapping works for SBK_BARGRAPH_28_SEG_SK_PCB    */
/*  or SBK_BARGRAPH_10_SEG_SK_PCB, those are common cathode    */
const uint8_t BG_SEG_MAP[28][2] = {
    {0, 0}, // SEG #1
    {0, 1}, // SEG #2
    {0, 2}, // SEG #3
    {0, 3}, // SEG #4
    {1, 0}, // SEG #5
    {1, 1}, // SEG #6
    {1, 2}, // SEG #7
    {1, 3}, // SEG #8
    {2, 0}, // SEG #9
    {2, 1}, // SEG #10
    {2, 2}, // SEG #11
    {2, 3}, // SEG #13
    {3, 0}, // SEG #12
    {3, 1}, // SEG #14
    {3, 2}, // SEG #15
    {3, 3}, // SEG #16
    {4, 0}, // SEG #17
    {4, 1}, // SEG #18
    {4, 2}, // SEG #19
    {4, 3}, // SEG #20
    {5, 0}, // SEG #21
    {5, 1}, // SEG #22
    {5, 2}, // SEG #23
    {5, 3}, // SEG #24
    {6, 0}, // SEG #25
    {6, 1}, // SEG #26
    {6, 2}, // SEG #27
    {6, 3}, // SEG #28
};

/*********************************************/
/*                                           */
/*          PACK WS2812 leds chain           */
/*                                           */
/*********************************************/
const uint8_t PACK_TOTAL_LEDS_NUMBER = 45; // Pack vent + powercell + cyclotron total WS21812 pixels
/***********************************************/
/*             PACK LEDS INDEX                 */
/***********************************************/
// The following sections contain indexes for the leds on the chain, starting from 0.
// You will need to update these indexes to match where things are on the chain
/***********************************************/
/*                 VENT LEDs                   */
/***********************************************/
const uint8_t VENT_1ST_LED = 0;  // vent first pixel index position in WS2812 pack chain
const uint8_t VENT_LAST_LED = 6; // vent last pixel index position in WS2812 pack chain
/***********************************************/
/*               POWERCELL LEDs                */
/***********************************************/
const uint8_t POWERCELL_1ST_LED = 35;     // Powercell first pixel index position in WS2812 pack chain
const uint8_t POWERCELL_LAST_LED = 44;    // Powercell last pixel index position in WS2812 pack chain
const bool POWERCELL_DIRECTION = FORWARD; // animation direction (FORWARD/REVERSE)
/***********************************************/
/*               CYCLOTRON LEDs                */
/***********************************************/
const bool CYCLOTRON_DIRECTION = FORWARD; // animation direction (FORWARD/REVERSE)
// >>> Uncomment only one cyclotron style and define cyclotron LEDs index on the PACK chain:
#define GB12 /* cyclotron with 4 rotating positions, could be use with ring LEDs or 4 pixels/jewels*/
// #define AFFE /* Only available for ring LEDS */
/*****************************/
/* >>>>> GB1/GB2 style <<<<< */
/*****************************/
#ifdef GB12
// Cyclotron 4 positions 1st and last pixels index (each jewel) in the WS2812 pack chain
const uint8_t CYC_POS1_1ST_LED = 7;
const uint8_t CYC_POS1_LAST_LED = 13;
const uint8_t CYC_POS2_1ST_LED = 14;
const uint8_t CYC_POS2_LAST_LED = 20;
const uint8_t CYC_POS3_1ST_LED = 21;
const uint8_t CYC_POS3_LAST_LED = 27;
const uint8_t CYC_POS4_1ST_LED = 28;
const uint8_t CYC_POS4_LAST_LED = 34;
#endif
/***************************/
/* >>>>> AF/FE style <<<<< */
/***************************/
#ifdef AFFE
const uint8_t CYC_RING_1ST_LED = 0;   // Cyclotron first pixel index position in WS2812 pack chain
const uint8_t CYC_RING_LAST_LED = 44; // Cyclotron last pixel index position in WS2812 pack chain
#endif

/*********************************************/
/*                                           */
/*          WAND WS2812 leds chain           */
/*                                           */
/*********************************************/
const uint8_t WAND_TOTAL_LEDS_NUMBER = 13; // Wand vent + indicators + firing jewel total WS21812 pixels
/***********************************************/
/*             WAND LEDS INDEX                 */
/***********************************************/
// The following sections contain indexes for the leds on the chain, starting from 0.
// You will need to update these indexes to match where things are in the chain
// Wand vent, indicators and firing tip index in the WS2812 wand chain :
const uint8_t WAND_LED_1ST_VENT = 1;
const uint8_t WAND_LED_LAST_VENT = 1;
const uint8_t WAND_LED_SLOWBLOW = 0;
const uint8_t WAND_LED_TOP_YELLOW = 2;
const uint8_t WAND_LED_TOP_WHITE = 3;
const uint8_t WAND_LED_FRONT_ORANGE = 5;
const uint8_t WAND_LED_FIRINGROD_YEL = 4;
const uint8_t WAND_LED_TIP_1ST = 6;
const uint8_t WAND_LED_TIP_LAST = 12;
// Indicator flashing speeds
const uint8_t INDICATOR_FAST_FLASH = 100;
const uint16_t INDICATOR_MEDIUM_FLASH = 500;
const uint16_t INDICATOR_SLOW_FLASH = 1000;

/*********************************************/
/*                                           */
/*    AUDIO PLAYER definition and helpers    */
/*                                           */
/*********************************************/
const uint8_t VOLUME_MAX = 25;             // 0-30 If you want to reduce the maximum possible volume according to your amp module, set this here
const uint8_t VOLUME_START = 15;           // 0-30 Volume at star-up, will not change if volume potentiometer doesn't exist
const uint16_t PLAYER_COMMAND_DELAY = 150; // short delay between query/ commands : some player(s) will behave weirdly if there is no delay
const uint16_t AUDIO_ADVANCE = 150;        // short advance to call the next track before the reel ending : assure smooth transition between track, aka endless playing
/****************************/
/*     OPTION : VOL POT     */
/****************************/
/* UNCOMMENT and DEFINE PIN if potentiometer exists for software volume control by audio player */
// #define VOL_POT_PIN A7    /* For software volume control with audio player*/
/* If no pot exists, do not define it, fixed volume will be used as defined above : VOLUME_START. */
/****************************/
/*    PLAYER definitions    */
/****************************/
/*  SELECT (uncommnent) your supported audio board and library : */
#define DFP_MINI_FAST /* To use DFPlayer Mini with the Fast library */
// #define DFP_MINI /* To use DFPlayer Mini with the DFRobot library */
/****************************/
/*  SOUND FX TRACKS INDEX   */
/****************************/
/* Audio track files numbers definition for soundboards DFPlayer Pro, DFPlayer mini DY-player */
/* Tracks should be mono WAVE type file, named 001.wav, 002.wav, etc. They should be place in the SD card */
/* root folder. They are played in the order they have been put on the flash drive, not by trackname */
/* Also, TRACK NUMBERS FIT PACK STATES, it's part of the core program, this order should be mainntain/corrected that way */
const int16_t BOOT_TRACK = 1;
const int16_t UNLOADED_IDLE_TRACK = 2; // Looping track
const int16_t CHARGED_IDLE_TRACK = 3;  // Looping track
const int16_t CHARGING_TRACK = 4;
const int16_t UNLOADING_TRACK = 5;
const int16_t FIRING_RAMP_TRACK = 6;
const int16_t FIRING_MAX_TRACK = 7; // Looping track
const int16_t FIRING_OVERHEAT_TRACK = 8;
const int16_t TAIL_TRACK = 9;
const int16_t TAIL_OVERHEAT_TRACK = 10;
const int16_t SHUTDOWN_TRACK = 11;
/****************************/
/* SOUND FX TRACKS LENGTHS  */
/****************************/
/* Tracks milliseconds lengths in index order : must be changed according to the yours tracks. */
/*  Those are used to determine the track's playing end in the CORE main loop to minimize delay in switching sound FX tracks, no BUSY pin is used. */
/*  It also prevent using the get track length functions that could cause some delay with some players */
/*  You can get your exact track lengths in Audacity or others audio software*/
/*  DEFINE the tracks lengths in milliseconde here :*/
const uint16_t TRACK_LENGTH[] = {
    0,     // no track, it just to offset with tracks index
    5000,  // track #1
    10000, // track #2
    10000, // track #3
    3000,  // track #4
    3000,  // track #5
    10000, // track #6
    7000,  // track #7
    7000,  // track #8
    3000,  // track #9
    5000,  // track #10
    3000   // track #11
};
/****************************/
/* SOUND FX TRACKS LOOPING  */
/****************************/
/* Tracks that need looping must be set to 'true' : */
const uint16_t TRACK_LOOPING[] = {
    false, // no track, powered down, no looping
    false, // track #1
    true,  // track #2 Idle track needs looping
    true,  // track #3 Idle track needs looping
    false, // track #4
    false, // track #5
    false, // track #6
    true,  // track #7 Idle firing needs looping
    false, // track #8
    false, // track #9
    false, // track #10
    false  // track #11
};
/*****************/
/* THEMES TRACKS */
/*****************/
//  You can choose and add your own tracks in the >>> "/01/" <<< folder of your player, tracks will play in cycle.
//  You can switch to next track with the fire button.

/****************************/
/*     PACK STATES LIST     */
/****************************/
/*  Pack goes trought states (switch/case loop) using flags and triggers. */
/*  Each state is a case, and each state case contain an othe switch/case : initital step case for this state, other possible steps and exit steps. */
/*  Possible Pack states for main loop switch cases: */
const uint8_t STATE_PWD_DOWN = 0;        // the pack is powered down
const uint8_t STATE_BOOTING = 1;         // pack is in booting process
const uint8_t STATE_IDLING_UNLOADED = 2; // has the pack booted up and is idling unloaded (not ready to fire)
const uint8_t STATE_IDLING_CHARGED = 3;  // has the pack booted up and is idling charged, ready to fire
const uint8_t STATE_CHARGING = 4;        // charging switch ON and pack charging
const uint8_t STATE_UNLOADING = 5;       // charging switch OFF and pack unloading
const uint8_t STATE_FIRING_RAMP = 6;     // firing state ramping up to max
const uint8_t STATE_FIRING_MAX = 7;      // firing state idling at max
const uint8_t STATE_FIRING_OVERHEAT = 8; // firing state with overheat
const uint8_t STATE_TAIL = 9;            // end of firing sequence, no overheating, short venting
const uint8_t STATE_OVERHEATED = 10;     // end of firing sequence, overheated, long venting
const uint8_t STATE_SHUTTING_DOWN = 11;  // the pack is in the process of shutting down

/****************************/
/*      GENERAL TIMER       */
/****************************/
const uint16_t FIRING_WARNING_DELAY = 10000; // in mS, how long before indicator is falshing yellow
const uint32_t FIRING_DURATION = 40000;      // in mS, how long max firing state before overheat

/****************************/
/* SOME ANIMATINONS OPTION  */
/****************************/
const bool POWERDOWN_BLINKING = false; // To show that power is still on on the pack, show some minimum ligths

#endif