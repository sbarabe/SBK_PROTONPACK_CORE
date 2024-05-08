/*
 *  RumbleEngine.cpp is a part of SBK_PROTONPACK_CORE (VERSION 2.1) code for animations of a Proton Pack replica
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

#include "RumbleEngine.h"

Rumbler::Rumbler(uint8_t pin, bool exist) : _pin(pin), _exist(exist)
{
    _prevStart = 0;
    _prevUpdate = 0;
    _burstDuration = 0;
}

void Rumbler::begin()
{
    if (_exist)
    {
        pinMode(_pin, OUTPUT);
        rumbleOFF();
    }
    else
    {
        _state = false;
    }
}

void Rumbler::update()
{
    if (_exist)
    {
        if (_state)
        {
            if ((_burstDuration > 0) && ((millis() - _prevStart) >= _burstDuration))
            {
                _burstDuration = 0;
                rumbleOFF();
            }

            if (millis() - _prevStart >= RUMBLER_MAX_ON_TIME)
            {
                _burstDuration = 0;
                rumbleOFF();
            }
        }
    }
}

void Rumbler::startBurst(uint16_t duration)
{
    if (_exist)
    {
        if (!_state)
        {
            _burstDuration = duration;
            rumbleON();
        }
    }
}

void Rumbler::rumbleON()
{
    if (_exist)
    {
        if (!_state && ((millis() - _prevStop) >= RUMBLER_MIN_OFF_TIME))
        {
            digitalWrite(_pin, HIGH);
            _prevStart = millis();
            _state = true;
        }
    }
}

void Rumbler::rumbleOFF()
{
    if (_exist)
    {
        if (_state)
        {
            digitalWrite(_pin, LOW);
            _prevStop = millis();
            _state = false;
        }
    }
}

bool Rumbler::getState()
{
    return _state;
}
