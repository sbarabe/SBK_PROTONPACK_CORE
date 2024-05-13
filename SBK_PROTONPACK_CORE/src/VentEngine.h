/*
 *  VentEngine.h is a part of SBK_PROTONPACK_CORE (VERSION 2.1) code for animations of a Proton Pack replica
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

#ifndef VENTENGINE_H
#define VENTENGINE_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class Vent
{
public:
    Vent(Adafruit_NeoPixel &strip, uint8_t start, uint8_t end);
    void begin();
    void update();
    void clear();
    void boot(int16_t boot_time, bool init);
    void warming(int16_t warming_time, bool init);
    void cooling(int16_t cooling_time, bool init);
    void shutdown(int16_t cooling_time, bool init);
    void rampToCoolBlue(int16_t ramp_time, bool init);
    void fadeOut(int16_t ramp_time, bool init);
    void setColor(uint8_t red, uint8_t green, uint8_t blue);

private:
    bool _rampColor(int16_t *colorTracker, int16_t rampTime, bool init, int16_t *initTracker, int16_t tg, int16_t *interval, int16_t increment, unsigned long *prevTime);
    int16_t _ramp_parameter(int16_t color, int16_t ini, int16_t tg, int16_t incr);
    Adafruit_NeoPixel &_strip;
    unsigned long _prevTime;
    unsigned long _prevTimeRed;
    unsigned long _prevTimeGreen;
    unsigned long _prevTimeBlue;
    uint8_t _start;
    uint8_t _end;
    uint8_t _numLeds;
    int16_t _redTracker;
    int16_t _greenTracker;
    int16_t _blueTracker;
    int16_t _initRedTracker;
    int16_t _initGreenTracker;
    int16_t _initBlueTracker;
    int16_t _intervalRed;
    int16_t _intervalGreen;
    int16_t _intervalBlue;
};

#endif