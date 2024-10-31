/*
 *  PowercellEngine.h is a part of SBK_PROTONPACK_CORE (VERSION 2.4) code for animations of a Proton Pack replica
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

#ifndef POWERCELLENGINE_H
#define POWERCELLENGINE_H

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

class Powercell
{
public:
    Powercell(Adafruit_NeoPixel &strip, bool direction, uint8_t start, uint8_t end);
    ~Powercell();
    void begin();
    void setDirection(bool direction);
    void update();
    void clear();
    void poweredDown();
    void boot(int16_t bootTime, bool init);
    void rampToIdleOne(uint16_t ramp_time, bool init);
    void rampToIdleTwo(uint16_t ramp_time, bool init);
    void rampToFiring(uint16_t ramp_time, bool init);
    void shuttingDown(int16_t shutdownTime, bool init);
    bool bootState;

private:
    void _idleOne(int16_t updateSp);
    void _idleTwo(int16_t updateSp);
    void _firing(int16_t updateSp);
    void _rampPowercell(int16_t rampTime, bool init, int16_t tg_speed);
    int16_t _ramp_parameter(int16_t param, int16_t ini, int16_t tg, int16_t incr);
    void _idlePowercell(int16_t updateSp);
    void _setColorAll(uint8_t red, uint8_t green, uint8_t blue);
    void _setColor(uint16_t pixel, uint8_t red, uint8_t green, uint8_t blue);
    Adafruit_NeoPixel &_strip;
    bool _direction;
    uint8_t _start;
    uint8_t _end;
    unsigned long _prevTime;
    uint8_t _numLeds;
    uint8_t** _ledState;
    int8_t _levelTracker;
    int8_t _shutdownTracker;
    int16_t _updateSp;
};

#endif
