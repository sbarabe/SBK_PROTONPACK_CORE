/*
 *  RodEngine.cpp is a part of SBK_PROTONPACK_CORE (VERSION 2.1) code for animations of a Proton Pack replica
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

#include "RodEngine.h"

FiringRod::FiringRod(Adafruit_NeoPixel &strip, uint8_t start, uint8_t end)
    : _strip(strip), _start(start), _end(end)
{
    _numLeds = end - start + 1;
    _ledState = new uint8_t *[_numLeds];
    for (int i = 0; i < _numLeds; i++)
    {
        _ledState[i] = new uint8_t[3];
    }
    _prevUpdate = 0;
}

FiringRod::~FiringRod()
{
    for (int i = 0; i < _numLeds; i++)
    {
        delete[] _ledState[i];
    }
    delete[] _ledState;
}

void FiringRod::begin()
{
    clear();
}

void FiringRod::update()
{
    for (uint8_t i = 0; i < _numLeds; i++)
    {
        _strip.setPixelColor(_start + i, _ledState[i][0], _ledState[i][1], _ledState[i][2]);
    }
}

void FiringRod::clear()
{
    _setColorAll(0, 0, 0);
}

void FiringRod::fireStrobe(uint8_t updateInterval)
{
    if (millis() - _prevUpdate > updateInterval)
    {
        _prevUpdate += updateInterval;
        /* for (uint8_t i = 0; i <= _numLeds; i++)
        {
            _ledState[i][0] = random(50, 255);
            _ledState[i][1] = random(0, 50);
            _ledState[i][2] = random(50, 255);
        } */
        uint8_t i = random(0, _numLeds);
        _ledState[i][0] = random(50, 255);
        _ledState[i][1] = random(0, 50);
        _ledState[i][2] = random(50, 255);
    }
}

void FiringRod::_setColorAll(uint8_t red, uint8_t green, uint8_t blue)
{
    for (uint16_t i = 0; i <= _numLeds; i++)
    {
        _ledState[i][0] = red;
        _ledState[i][1] = green;
        _ledState[i][2] = blue;
    }
}

void FiringRod::_setColor(uint16_t pixel, uint8_t red, uint8_t green, uint8_t blue)
{
    _ledState[pixel][0] = red;
    _ledState[pixel][1] = green;
    _ledState[pixel][2] = blue;
}

void FiringRod::tail(uint16_t fadeOutTime)
{
    static unsigned long prev = 0;
    uint8_t increment = 3;
    uint16_t interval = (int32_t)fadeOutTime * (int32_t)increment / 255;
    if (millis() - prev > interval)
    {
        prev += interval;
        for (uint8_t i = 0; i < _numLeds; i++)
        {
            uint8_t red = _ledState[i][0];
            uint8_t green = _ledState[i][1];
            uint8_t blue = _ledState[i][2];
            if (red > increment)
            {
                red -= increment;
            }
            else
            {
                red = 0;
            }
            if (green > increment)
            {
                green -= increment;
            }
            else
            {
                green = 0;
            }
            if (blue > increment)
            {
                blue -= increment;
            }
            else
            {
                blue = 0;
            }
            _ledState[i][0] = red;
            _ledState[i][1] = green;
            _ledState[i][2] = blue;
        }
    }
}
