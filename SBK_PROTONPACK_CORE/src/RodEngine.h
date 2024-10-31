/*
 *  RodEngine.h is a part of SBK_PROTONPACK_CORE (VERSION 2.4) code for animations of a Proton Pack replica
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

#ifndef RODENGINE_H
#define RODENGINE_H

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

class FiringRod
{
public:
    FiringRod(Adafruit_NeoPixel &strip, uint8_t start, uint8_t end);
    ~FiringRod();
    void begin();
    void update();
    void clear();
    void fireStrobe(uint8_t updateInterval);
    void tail(uint16_t fadeOutTime);

private:
    void _setColorAll(uint8_t red, uint8_t green, uint8_t blue);
    void _setColor(uint16_t pixel, uint8_t red, uint8_t green, uint8_t blue);
    Adafruit_NeoPixel &_strip;
    uint8_t _start;
    uint8_t _end;
    uint8_t _numLeds;
    uint8_t** _ledState;
    unsigned long _prevUpdate;
};

#endif