/*
 *  VentEngine.cpp is a part of SBK_PROTONPACK_CORE (VERSION 2.1) code for animations of a Proton Pack replica
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

#include "VentEngine.h"

Vent::Vent(Adafruit_NeoPixel &strip, uint8_t start, uint8_t end)
    : _strip(strip), _start(start), _end(end)
{
    _prevTime = 0;
    _prevTimeRed = 0;
    _prevTimeGreen = 0;
    _prevTimeBlue = 0;
    _numLeds = (_end - _start + 1);
    _redTracker = 0;
    _greenTracker = 0;
    _blueTracker = 0;
}

void Vent::begin() { clear(); }

void Vent::update()
{
    for (int8_t i = 0; i < _numLeds; i++)
    {
        // Offset index to fit ws2812 LEDs strip index
        uint8_t j = i + _start;
        // set segments according to mapping define in setting
        _strip.setPixelColor(j, _redTracker, _greenTracker, _blueTracker);
    }
}

void Vent::clear()
{
    _redTracker = 0;
    _greenTracker = 0;
    _blueTracker = 0;
}

void Vent::setColor(uint8_t red, uint8_t green, uint8_t blue)
{
    _redTracker = red;
    _greenTracker = green;
    _blueTracker = blue;
}

void Vent::boot(int16_t boot_time, bool init)
{
    int16_t increment = 10;
    int16_t fadeoutTime = (uint32_t)boot_time * 30 * increment / 100;

    // Fade out vent
    _rampColor(&_redTracker, fadeoutTime, init, &_initRedTracker, 0, &_intervalRed, increment, &_prevTimeRed);
    _rampColor(&_blueTracker, fadeoutTime, init, &_initBlueTracker, 0, &_intervalBlue, increment, &_prevTimeBlue);
}

void Vent::warming(int16_t warming_time, bool init)
{
    int16_t redIncrement = 1;
    int16_t blueIncrement = 10;
    int16_t redFadeInTime = (uint32_t)warming_time * 75 * redIncrement / 100;
    int16_t blueFadeOutTime = (uint32_t)warming_time * 10 * blueIncrement / 100;
    _rampColor(&_redTracker, redFadeInTime, init, &_initRedTracker, 255, &_intervalRed, redIncrement, &_prevTimeRed);
    _rampColor(&_blueTracker, blueFadeOutTime, init, &_initBlueTracker, 0, &_intervalBlue, blueIncrement, &_prevTimeBlue);
}

void Vent::cooling(int16_t cooling_time, bool init)
{
    static bool fadeOutBlue = false;
    static bool fadeOutBlueInit = true;
    int16_t fadeOutRedRatio = 100;
    int16_t redIncrement = 2;
    int16_t fadeOutRedTime = (uint32_t)cooling_time * fadeOutRedRatio * redIncrement / 100;
    int16_t fadeInBlueRatio = 70;
    int16_t blueIncrement = 2;
    int16_t blueFadeInTime = (uint32_t)cooling_time * fadeInBlueRatio * blueIncrement / 100;
    int16_t blueFadeOutTime = (uint32_t)cooling_time * (100 - fadeInBlueRatio) * blueIncrement / 100;

    // Ramp red to zero
    _rampColor(&_redTracker, fadeOutRedTime, init, &_initRedTracker, 0, &_intervalRed, redIncrement, &_prevTimeRed);

    // Ramp blue from zero to full, then full to zero
    if (init)
    {
        fadeOutBlue = false;
        fadeOutBlueInit = true;
    }
    // Fade IN blue
    if (!fadeOutBlue)
    {
        fadeOutBlue = _rampColor(&_blueTracker, blueFadeInTime, init, &_initBlueTracker, 255, &_intervalBlue, blueIncrement, &_prevTimeBlue);
    }
    // Then fade OUT blue
    else
    {
        _rampColor(&_blueTracker, blueFadeOutTime, fadeOutBlueInit, &_initBlueTracker, 0, &_intervalBlue, blueIncrement, &_prevTimeBlue);
        if (fadeOutBlueInit)
        {
            fadeOutBlueInit = false;
        }
        // Serial.println("2nd cool");
    }
}

void Vent::shutdown(int16_t cooling_time, bool init)
{
    static unsigned long redFlashStart;
    int16_t redFlashTime = (uint32_t)cooling_time * 40 / 100;
    static bool fadeOutRed = false;
    static bool fadeOutRedInit = true;
    int16_t fadeInRedIncr = 10;
    int16_t fadeInRedTime = (uint32_t)cooling_time * 10 * fadeInRedIncr / 100;
    int16_t fadeOutRedIncr = 10;
    int16_t fadeOutRedTime = (uint32_t)cooling_time * 20 * fadeOutRedIncr / 100;
    // static bool fadeInBlue = false;
    static bool fadeInBlueInit = true;
    static bool fadeOutBlue = false;
    static bool fadeOutBlueInit = true;
    int16_t fadeInblueIncr = 3;
    int16_t fadeOutblueIncr = 5;
    int16_t blueFadeInTime = (uint32_t)cooling_time * 40 * fadeInblueIncr / 100;
    int16_t blueFadeOutTime = (uint32_t)cooling_time * 20 * fadeOutblueIncr / 100;

    // Ramp blue from zero to full, then full to zero
    if (init)
    {
        fadeOutRed = false;
        fadeOutRedInit = true;
        // fadeInBlue = false;
        fadeInBlueInit = true;
        fadeOutBlue = false;
        fadeOutBlueInit = true;
        setColor(_redTracker, 0, 0);
    }
    // Fade IN RED
    if (!fadeOutRed)
    {
        if ((millis() - redFlashStart + 10 > redFlashTime) &&
            _rampColor(&_redTracker, fadeInRedTime, init, &_initRedTracker, 255, &_intervalRed, fadeInRedIncr, &_prevTimeRed))
        {
            fadeOutRed = true;
        }
    }
    else
    {
        // Then fade out RED
        _rampColor(&_redTracker, fadeOutRedTime, fadeOutRedInit, &_initRedTracker, 0, &_intervalRed, fadeOutRedIncr, &_prevTimeRed);
        if (fadeOutRedInit)
        {
            fadeOutRedInit = false;
        }

        // And fade in BLUE
        if (!fadeOutBlue)
        {
            // Serial.println(_blueTracker);
            fadeOutBlue = _rampColor(&_blueTracker, blueFadeInTime, fadeInBlueInit, &_initBlueTracker, 255, &_intervalBlue, fadeInblueIncr, &_prevTimeBlue);
            if (fadeInBlueInit)
            {
                fadeInBlueInit = false;
            }
        }
        // Then fade OUT blue
        else
        {
            // Serial.println(_blueTracker);
            _rampColor(&_blueTracker, blueFadeOutTime, fadeOutBlueInit, &_initBlueTracker, 0, &_intervalBlue, fadeOutblueIncr, &_prevTimeBlue);
            if (fadeOutBlueInit)
            {
                fadeOutBlueInit = false;
            }
        }
    }
}

void Vent::rampToCoolBlue(int16_t ramp_time, bool init)
{
    int16_t redTarget = 50;
    int16_t greenTarget = 50;
    int16_t blueTarget = 255;

    int16_t redIncrement = 1;
    int16_t greenIncrement = 1;
    int16_t blueIncrement = 1;

    _rampColor(&_redTracker, ramp_time, init, &_initRedTracker, redTarget, &_intervalRed, redIncrement, &_prevTimeRed);
    _rampColor(&_greenTracker, ramp_time, init, &_initGreenTracker, greenTarget, &_intervalGreen, greenIncrement, &_prevTimeGreen);
    _rampColor(&_blueTracker, ramp_time, init, &_initBlueTracker, blueTarget, &_intervalBlue, blueIncrement, &_prevTimeBlue);
}

void Vent::fadeOut(int16_t ramp_time, bool init)
{
    int16_t redIncrement = 1;
    int16_t greenIncrement = 1;
    int16_t blueIncrement = 1;

    _rampColor(&_redTracker, ramp_time, init, &_initRedTracker, 0, &_intervalRed, redIncrement, &_prevTimeRed);
    _rampColor(&_greenTracker, ramp_time, init, &_initGreenTracker, 0, &_intervalGreen, greenIncrement, &_prevTimeGreen);
    _rampColor(&_blueTracker, ramp_time, init, &_initBlueTracker, 0, &_intervalBlue, blueIncrement, &_prevTimeBlue);
}

bool Vent::_rampColor(int16_t *colorTracker, int16_t rampTime, bool init, int16_t *initTracker, int16_t tg, int16_t *interval, int16_t increment, unsigned long *prevTime)
{
    // Record initial vent color trackers
    if (init)
    {
        *initTracker = *colorTracker;
        if (tg == *initTracker)
        {
            *interval = 0;
        }
        else
        {
            *interval = rampTime / abs(tg - *initTracker);
        }
    }

    // check if ramp is done
    if (rampTime == 0)
    {
        *colorTracker = tg;
    }
    if (*colorTracker == tg)
    {
        return true;
    }
    else
    // Ramp not done yet
    {

        // Update color tracker
        if (*colorTracker != tg)
        {
            if (millis() - *prevTime >= *interval)
            {
                *prevTime = millis();
                *colorTracker = _ramp_parameter(*colorTracker, *initTracker, tg, increment);
            }
        }
        return false;
    }
}

int16_t Vent::_ramp_parameter(int16_t param, int16_t ini, int16_t tg, int16_t incr)
{
    // int16_t newParam = param;
    if ((tg - ini) >= 0)
    { // Ramp UP
        if (param + incr <= tg)
        {
            param = param + incr;
        }
        else
        {
            param = tg;
        }
    }
    else
    { // Ramp DOWN
        if (incr >= param || param - incr < tg)
        {
            param = tg;
        }
        else
        {
            param = param - incr;
            if (param < 0)
            {
                param = 0;
            }
        }
    }
    return param;
}
