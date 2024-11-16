/*
 *  SwitchEngine.h is a part of SBK_PROTONPACK_CORE (VERSION 2.4) code for bar graph animations of a Proton Pack replica
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

#ifndef SWITCHESENGINE_H
#define SWITCHESENGINE_H

#include <Arduino.h>

class Switch
{
public:
    Switch(uint8_t pin, bool reverse);
    void begin();
    void setDebounce(uint8_t delay);
    bool getState();
    bool isON();
    bool toggleON();
    bool toggleOFF();
    
private:
void _getReading();
unsigned long getDebounce(bool reading, bool prev_state, unsigned long last_debounce);
bool getPushButtonState(bool reading, bool state, unsigned long last_debounce);
    uint8_t _pin;
    bool _reverse;
    bool _state;
    bool _statePrev;
    bool _reading;
    bool _readingPrev;
    unsigned long _toggleNow;
    uint8_t _debounceDelay;
};


#endif
