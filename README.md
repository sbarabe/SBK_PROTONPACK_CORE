# SBK_PROTONPACK_CORE
Code for lights and sound effects of a Proton Pack replica or other props.

## Installation

After downloading, rename 'src' folder to 'SBK_PROTONPACK_CORE' and copy it into Arduino IDE sketches folder. Then open File->Sketchbook->SBK_PROTONPACK_CORE->SBK_PROTONPACK_CORE.ino.

### Library

Depending on your project hardware, the following library and their dependencies are required :

- For bar graph driver :
    -  MAX71xx >>>   LedControl.h (https://github.com/wayoda/LedControl)
    -  HT16K33 >>>   HT16K33.h (https://github.com/MikeS11/ProtonPack/tree/master/Source/Libraries/ht16k33-arduino-master)
- For WS2812 LEDs :       Adafruit_NeoPixel.h (https://github.com/adafruit/Adafruit_NeoPixel)
- For DFPlayer Pro (not fully tested yet) : DFRobot_DF1201S.h (https://github.com/DFRobot/DFRobot_DF1201S)
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

  The SBK_CONFIG.h file contains all the definitions and options : pins, player module, LEDs index, audio tracks, etc. This is the file you want to customize to adjust your pins setting, LEDs chain and index, audio tracks info and other options. If you like the animations and the pack workflow, you should not have to change anything else then the config file.

  Be careful when working/changing animations times/speed regarding your audio tracks/pact states durations. They are all gathered in the getLEDsSchemeForThisState() function after the Main Loop in the core file. CAUTION : playing with times and speeds can really mess up the animations flow. It is highly suggested taking notes of the original values and changing a few of them at the time and see the effects.

  #### *** ADVANCED USERS ***
  
  If you want to change animations styles and colors, you need to go in the engines files and modified the associated functions or create new ones. Then you will have to implement them in the getLEDsSchemeForThisState() function in the core file.

  If you want to change the pack state switch/cases workflow, you will have to modify pack states list and audio tracks list/length/looping in the CONFIG file, and the Main Loop pact states switch/cases contents.

  Sketch mechanic works with different pack states and transitions in the Pack state switches/cases defined in Main Loop. Each pack state has is initialization stage (stageFlag 0) and looping stage (stageFlag 1). The looping stage includes exit(s) to other stages : switches actions, buttons actions and audio track ending.


- ### Code splited in organized files  

  This code is based on objects programming, objects are defined in their *.h and *.cpp files. Objects instances are created and used in the core file SBK_PROTONPACK_CORE.ino. There is also a config file, SBK_CONFIG.h, where all basic parameters can be changed according to hardware used. This fully compartmented code helps keeping code cleaner and easier to update/maintain. This also prevent having an endless code hard to follow.

  
- ### Board tested

  This code has been fully tested on the following board, that doesn't mean it's the only boards working.

  - Arduino Nano
  - Arduino Nano Every
 
    
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


