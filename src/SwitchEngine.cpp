/*
 *  switches.cpp is a part of SBK_PROTONPACK_CORE (VERSION 2.1) code for animations of a Proton Pack replica
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

#include "SwitchEngine.h"

const uint8_t DEBOUNCE_DELAY = 50;

Switch::Switch(uint8_t pin, bool reverse) : _pin(pin), _reverse(reverse)
{
    _state = false;
    _statePrev = false;
    _reading = false;
    _readingPrev = false;
    _toggleNow = 0;
}

void Switch::begin()
{
    pinMode(_pin, INPUT_PULLUP);
}

bool Switch::getState()
{
    // Register previous state for toggle functions
    _statePrev = _state;

    // Read switch pin
    if (_reverse)
    {
        _reading = digitalRead(_pin);
    }
    else
    {
        _reading = !digitalRead(_pin);
    }

    // check if reading has changed
    if (_reading != _readingPrev)
    {
        _readingPrev = _reading;
        _toggleNow = millis();
    }

    // check if reading is maintained for the debounce delay
    if (millis() - _toggleNow > DEBOUNCE_DELAY)
    {
        // If true and reading is different from current state, change state
        if (_reading != _state)
        {
            _state = _reading;
            if(DEBUG) Serial.print("Switch is "), Serial.println(_state);
        }
    }
    return _state;
}

bool Switch::isON()
{
    return _state;
}

bool Switch::toggleON()
{
    if (_state != _statePrev && _state)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Switch::toggleOFF()
{
    if (_state != _statePrev && !_state)
    {
        return true;
    }
    else
    {
        return false;
    }
}