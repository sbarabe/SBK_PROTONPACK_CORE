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
    _prevUpdate = 0;
}

void FiringRod::begin()
{
    _setColorAll(0, 0, 0);
}

void FiringRod::update()
{
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
        for (uint8_t i = _start; i <= _end; i++)
        {
            _setColor(i, random(100, 255), random(0, 100), random(100, 255));
        }
    }
}

void FiringRod::_setColorAll(uint8_t red, uint8_t green, uint8_t blue)
{
    for (uint16_t i = _start; i <= _end; i++)
    {
        _strip.setPixelColor(i, red, green, blue);
    }
}

void FiringRod::_setColor(uint16_t pixel, uint8_t red, uint8_t green, uint8_t blue)
{
    _strip.setPixelColor(pixel, red, green, blue);
}
