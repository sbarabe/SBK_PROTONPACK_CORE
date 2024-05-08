/*
 *  IndicatorEngine.cpp is a part of SBK_PROTONPACK_CORE (VERSION 2.1) code for animations of a Proton Pack replica
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

#include "IndicatorEngine.h"

Indicator::Indicator(Adafruit_NeoPixel &strip, uint8_t pixel)
    : _strip(strip), _pixel(pixel)
{
    _prevTime = 0;
}

void Indicator::begin() { clear(); }

void Indicator::setColor(uint8_t red, uint8_t green, uint8_t blue)
{
    _strip.setPixelColor(_pixel, red, green, blue);
}

void Indicator::show()
{
    _strip.show();
}

void Indicator::clear()
{
    setColor(0, 0, 0);
}

void Indicator::white()
{
    setColor(100, 100, 100);
}

void Indicator::white(uint16_t updateSp) // flashing
{
    static bool flashState = false;
    if ((millis() - _prevTime) >= updateSp)
    {
        _prevTime += updateSp;
        if (flashState == false)
        {
            white();
            flashState = true;
        }
        else
        {
            clear();
            flashState = false;
        }
    }
}

void Indicator::red()
{
    setColor(150, 0, 0);
}

void Indicator::red(uint16_t updateSp) // flashing
{
    static bool flashState = false;
    if ((millis() - _prevTime) >= updateSp)
    {
        _prevTime += updateSp;
        if (flashState == false)
        {
            red();
            flashState = true;
        }
        else
        {
            clear();
            flashState = false;
        }
    }
}

void Indicator::yellow()
{
    setColor(0, 125, 125);
}

void Indicator::yellow(uint16_t updateSp) // flashing
{
    static bool flashState = false;
    if ((millis() - _prevTime) >= updateSp)
    {
        _prevTime += updateSp;
        if (flashState == false)
        {
            yellow();
            flashState = true;
        }
        else
        {
            clear();
            flashState = false;
        }
    }
}

void Indicator::green()
{
    setColor(0, 150, 0);
}

void Indicator::green(uint16_t updateSp) // flashing
{
    static bool flashState = false;
    if ((millis() - _prevTime) >= updateSp)
    {
        _prevTime += updateSp;
        if (flashState == false)
        {
            green();
            flashState = true;
        }
        else
        {
            clear();
            flashState = false;
        }
    }
}

void Indicator::orange()
{
    setColor(255, 127, 0);
}

void Indicator::orange(uint16_t updateSp) // flashing
{
    static bool flashState = false;
    if ((millis() - _prevTime) >= updateSp)
    {
        _prevTime += updateSp;
        if (flashState == false)
        {
            orange();
            flashState = true;
        }
        else
        {
            clear();
            flashState = false;
        }
    }
}