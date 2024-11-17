/*
 *  WS2812TestEngine.h is a part of SBK_WS2812_LEDS_CHAINS_TEST (Version 0) code for testing WS2812 LEDs
 *  chains connections and wiring used in many Ghostbusters props replica.
 *  Copyright (c) 2024 Samuel Barabé
 *
 *  See this page for reference <https://github.com/sbarabe/SBK_PROTONPACK_CORE/tree/main/Tools/SBK_WS2812_LEDS_CHAINS_TEST">.
 *
 *  SBK_WS2812_LEDS_CHAINS_TEST is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Foundation, either version
 *  3 of the License, or (at your option) any later version.
 *
 *  SBK_WS2812_LEDS_CHAINS_TEST is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
 *  the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with Foobar. If not,
 *  see <https://www.gnu.org/licenses/>
 */

#ifndef WS2812TESTENGINE_H
#define WS2812TESTENGINE_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class WS2812Test
{
public:
    WS2812Test(Adafruit_NeoPixel &strip, uint8_t start, uint8_t end);
    void begin();
    void update();
    void clear();
    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    bool rampToBlue(int16_t ramp_time, bool init);
    bool rampToRed(int16_t ramp_time, bool init);
    bool rampToGreen(int16_t ramp_time, bool init);
    void test(uint16_t time);

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