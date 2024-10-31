/*
 *  RumbleEngine.h is a part of SBK_PROTONPACK_CORE (VERSION 2.4) code for animations of a Proton Pack replica
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

#ifndef RUMBLEENGINE_H
#define RUMBLEENGINE_H

#include "Arduino.h"

class Rumbler
{
public:
    Rumbler(uint8_t pin,
            const uint32_t *maxOnTime,
            const uint32_t *minOffTime,
            bool exist);
    void begin();
    void update();
    void startBurst(uint16_t duration);
    bool getState();
    void rumbleON();
    void rumbleOFF();

private:
    bool _exist;
    uint8_t _pin;
    uint8_t _state;
    unsigned long _prevStart;
    unsigned long _prevStop;
    unsigned long _prevUpdate;
    uint16_t _burstDuration;
    const uint32_t *_ptr_MAX_ON_TIME;
    const uint32_t *_ptr_MIN_OFF_TIME;
};

#endif