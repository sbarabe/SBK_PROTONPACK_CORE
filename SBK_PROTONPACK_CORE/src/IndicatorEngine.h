/*
 *  IndicatorEngine.h is a part of SBK_PROTONPACK_CORE (VERSION 2.4) code for animations of a Proton Pack replica
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

#ifndef INDICATORENGINE_H
#define INDICATORENGINE_H

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

class Indicator
{
public:
    Indicator(Adafruit_NeoPixel &strip, uint8_t pixel);
    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    void begin();
    void show();
    void clear();
    void red(uint16_t updateSp);    // flashing
    void yellow(uint16_t updateSp); // flashing
    void white(uint16_t updateSp);  // flashing
    void green(uint16_t updateSp);  // flashing
    void orange(uint16_t updateSp); // flashing

private:
    void _red();
    void _yellow();
    void _white();
    void _green();
    void _orange();
    Adafruit_NeoPixel &_strip;
    unsigned long _prevTime;
    uint8_t _pixel;
    bool _flashingState;
    bool _pulse;
};

class SingleColorIndicator
{
public:
    SingleColorIndicator(uint8_t indicator_pin, bool state);
    void begin();
    void clear();
    void on();    
    void off();   
    void flash(uint16_t updateSp); // flashing

private:
    void _write(bool state);
    uint8_t _indicator_pin;
    unsigned long _prevTime;
    bool _state;
    bool _flashingState;
    bool _pulse;
};

#endif