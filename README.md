# SBK_PROTONPACK_CORE
## Version 2.4
Code for lights and sound effects of a Proton Pack replica or other props.

## Installation

1) After downloading, copy 'SBK_PROTONPACK_CORE' folder to your Arduino IDE sketches folder.
2) If you want, you can use a pre-configure file that fits your needs : just rename the pre-configured SBK_CONFIG_***.txt file to ACONFIG.h.
3) In Arduino IDe, open File->Sketchbook->SBK_PROTONPACK_CORE->SBK_PROTONPACK_CORE.ino, you will see two tabs :
     - SBK_PROTONPACK_CORE.ino
     - ACONFIG.h
4) Select the ACONFIG.h tab and check/modify this file to fit your hardware : pins definition, WS2812 LEDs index, audio tracks length, etc.
5) Upload to your MCU.
   
### Library

Depending on your project hardware, the following library and their dependencies are required :

- For bar graph driver :
    -  MAX71xx:
         -  LedControl.h (https://github.com/wayoda/LedControl)
         -  FireTimer.h (https://github.com/PowerBroker2/FireTimer)
    -  HT16K33 : SBK_HT16K33.h (already included in the code, in the src subfolder)
- For WS2812 LEDs :       Adafruit_NeoPixel.h (https://github.com/adafruit/Adafruit_NeoPixel)
- For DFPlayer Mini :
    - DFRobotDFPlayerMini.h (https://github.com/DFRobot/DFRobotDFPlayerMini)
    - DFPlayerMine_Fast.h (https://github.com/PowerBroker2/DFPlayerMini_Fast)


## Features

 *    DFPlayer mini sound board.
 *    Wand BAR GRAPH : up to 28 segments (configurable) controlled by MAX72xx or HT16K33 drivers
 *    Pack Lights (WS2812) :
         *  Powercell
         *  Cyclotron : GB1/GB2 or Frozen Empire style
         *  Vent  
 *    Wand Lights (WS2812):
         *    Vent
         *    Slowblow indicator
         *    Top white indicator
         *    Top Yellow indicator
         *    Front orange indicator
         *    Firing Rod Indicator
 *    All LED pixel numbers and index are configurable.       
 *    LEDs animations are defined in object functions. Class objects and functions are in separated files to keep the main sketch as short as possible.
 *    There is an option for volume potentiometer that uses software volume control with the audio player. If not define, the volume will be the one declare in the AUDIO PLAYER configuration section.
 *    There is the following regular switches and buttons :
      -    Startup switch : power on/shutting down the pack by calling starting up / shutting down sequence.
      -    Charging switch :    when wnad charged, shooting is possible.
      -    Theme switch :    for playing themes song instead of the regular pack function.
      -    Fire button :    it does what it says... It also skips to next themes in themes playing mode.
      -    Fire2 button :    same as fire button but play previous themes in themes playing mode.
 *    Other options :
         -    Smoke module
         -    Rumble module


- ### Configurable

  The ACONFIG.h file contains all the definitions and options : pins, player module, LEDs index, audio tracks, etc. This is the file you want to customize to adjust your pins setting, LEDs chain and index, audio tracks info and other options. If you like the animations and the pack workflow, you should not have to change anything else then the config file.

  Be careful when working/changing animations times/speed regarding your audio tracks/pact states durations. They are all gathered in the getLEDsSchemeForThisState() function after the Main Loop in the core file. CAUTION : playing with times and speeds can really mess up the animations flow. It is highly suggested taking notes of the original values and changing a few of them at the time and see the effects.

    
  #### *** ADVANCED USERS ***
  
  If you want to change animations styles and colors, you need to go in the engines files and modified the associated functions or create new ones. Then you will have to implement them in the getLEDsSchemeForThisState() function in the core file.

  If you want to change the pack state switch/cases workflow, you will have to modify pack states list and audio tracks list/length/looping in the CONFIG file, and the Main Loop pact states switch/cases contents.

  Sketch mechanic works with different pack states and transitions in the Pack state switches/cases defined in Main Loop. Each pack state has is initialization stage (stageFlag 0) and looping stage (stageFlag 1). The looping stage includes exit(s) to other stages : switches actions, buttons actions and audio track ending.


- ### Code splited in organized files  

  This code is based on objects programming, objects are defined in their *.h and *.cpp files. Objects instances are created and used in the core file SBK_PROTONPACK_CORE.ino. There is also a config file, ACONFIG.h, where all basic parameters can be changed according to hardware used. This fully compartmented code helps keeping code cleaner and easier to update/maintain. This also prevent having an endless code hard to follow.
 
- ### Compatible HARDWARE

  This code is intended to be use with any PCB and MCU that fits minimum pins requirements :
  - 5 digital inputs for the switches/buttons, with internal pull up resistor
  - 2 digital outputs for WS2812 LEDs chains
  - 2 pins for serial communications with the audio player (hardware or software)
  - 2 or 3 pins for the bar graph driver : 2 for I2C (SDA/SCL), 3 pins for MAX72xx type (DATA, CLOCK, LOAD)
  - 1 digital output for smoker (optional)
  - 1 digital output for rumbler (optional)
  - 1 analog input for a potentiometer to control volume by audio player : software control (optional)
 
  LEDs should be WS2812 or any supported LEDs by the library.

  Bar graph driver could be MAX7219/7221 SPI/serial driver or the HTHT16K33 I2C driver.
  Bar graph could be 8 to 12 segements common cathode bar graph, or the 28 segements common cathode

  Supported audio player is the DFPlayer Mini, others could be added latter uppon request.

  The actual CONFIG.h file as it is should be setup to work with :
  - MikeS11 pcb
  - HT16K33.h with 28 segments bar graph with common cathode
  - 1 LEDs strip for the wand : wandvent, indicators and firing jewel
  - 1 LEDS strip for the pack : packvent, cyclotron and powercell
  Always check the PINs definition and LEDs index to fit yours.
  Beware that the pack vent is on pack LEDs strip, not at the start of the wand LEDs strip...

- ### MCU Board tested

  This code has been fully tested on the following board, that doesn't mean it's the only boards working.

  - Arduino Nano
  - Arduino Nano Every

## Sound effects

As examples, you'll find here some sounds effects that have been remixed for this code. The actual config file uses those track numbers and lengths. Note that the exact sources of the original sound files are unknowed, so it is impossible to say if they are copyrighted, but probably are in some way. Use these sound effects examples at your own risk : https://mega.nz/folder/GZ8TFIzK#W5bunWSMubMsOIHVNrYEIA

Note : the clipped files have been reduce in gain to lower distorsion on small speakers.
    
## Give back

The library is free, you don’t have to pay for anything. The author invests time and resources providing this open source code and open-source hardware, please support him with a little contribution at : [paypal.me/sbarab](https://www.paypal.com/paypalme/sbarab). 
You could also consider using the author specific pcb board for your project, they'll be available soon.

## Requests

Please post a new issue if you have request for this code, like new animations. It the best way to keep track and follow requests.

## Contribute

You have a great idea for this project, you can fork this project and make a pull request !

## GNU Lesser General Public License

SBK_PROTONPACK_CORE is free software: you can redistribute it and/or  modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
SBK_PROTONPACK_CORE is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the [GNU Lesser General Public License](https://www.gnu.org/licenses/lgpl-3.0.en.html) for more details.
You should have received a copy of the GNU Lesser General Public License along with SBK_PROTONPACK_CORE.  If not, see [this](https://www.gnu.org/licenses/)

## Credits

This library is written by Samuel Barabé, inspired by other members of the open source community in the GB props replica, like https://github.com/MikeS11, https://github.com/CountDeMonet and many others .

## History

2.1 Release in 2024-05-09

2.2 Release in 2024-05-10 :
- ventEngine fadeOut(uint8_t ramp_time) function corrected : program was stalling on divide by zero...
  
2.3 Release in 2024-05-13 :
- added exits to sequence to assure smooth firing endings if powerdown or safety engage while firing ;
- moved engines files to src subfolder : they are no longer visible in the Arduino IDE, only CORE and CONFIG file remain visible;
- included  a modified HT16K33 library in the src subfolder : SBK_HT16K33.h

2.4 Release 2024-10-31 :
- added LEDs update rate limitation (5ms, and toggling between wand and pack leds chain) in main loop to reduce MCU load : it seems to help the DFPlayer to get all commands with no flaw or delay;
- added dual boot switches mode : if you want to have a wand AND a pack power switches. You need to define a pack boot switch pin in the CONFIG.h file. Added function and helpers to get power state of both switches;
- Modified AF/FE ring speed and update rates for faster animation ;
- Fix wrong volume at startup with the volume potentiometer option ;
- Modified Vent Engine animations for less demanding calculations, modified animations implemented in the Core file getLEDsSchemeForThisState() function.
- Added the goes to over heat when both fire button and rod button push buttons are pressed while firing : you can fire with one button, and go straith to over heat when you hit the second button;
  
  
