/*
 *  PowercellEngine.cpp is a part of SBK_PROTONPACK_CORE (VERSION 2.1) code for animations of a Proton Pack replica
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

#include "PowercellEngine.h"

// Cyclotron GB1/GB2 STYLE animations variables
#define PC_BRIGHTNESS 100
// POWERDOWN animation targets
#define PC_PWD_FLASH true // true = powercell 1st pixel blinking when powered down. false = no blinking, pixels OFF
#define PC_PWD_FLASH_ON 500
#define PC_PWD_FLASH_OFF 3000
// IDLE ONE animation targets
#define PC_IDLE1_UPDATE_SP 60
// IDLE TWO animation targets
#define PC_IDLE2_UPDATE_SP 40
// STATE_FIRING animation targets
#define PC_FIRE_UPDATE_SP 15

Powercell::Powercell(Adafruit_NeoPixel &strip, bool direction, uint8_t start, uint8_t end)
    : _strip(strip), _direction(direction), _start(start), _end(end)
{
    _prevTime = 0;
    _numLeds = (_end - _start + 1);
    _ledState = new uint8_t *[_numLeds];
    for (int i = 0; i < _numLeds; i++)
    {
        _ledState[i] = new uint8_t[3];
    }
    bootState = false;
    _levelTracker = 0;
    _shutdownTracker = _numLeds - 1;
}

Powercell::~Powercell()
{
    for (int i = 0; i < _numLeds; ++i)
    {
        delete[] _ledState[i]; // Free memory for each row
    }
    delete[] _ledState;
}

void Powercell::begin() { clear(); }

void Powercell::setDirection(bool direction)
{
    _direction = direction;
}

void Powercell::update()
{
    for (int8_t i = 0; i < _numLeds; i++)
    {
        // Offset index to fit ws2812 LEDs strip index
        uint8_t j;
        if (!_direction) // FORWARD durection
        {
            j = i + _start;
        }
        else
        {
            j = _end - i;
        }
        // set segments according to mapping define in setting
        _strip.setPixelColor((j), _ledState[i][0], _ledState[i][1], _ledState[i][2]);
    }
}

void Powercell::clear()
{
    _setColorAll(0, 0, 0);
    bootState = false;
    _levelTracker = 0;
    _shutdownTracker = _numLeds - 1;
}

void Powercell::poweredDown()
// All bar graph pixels are OFF execpt pixels one blinking
{
    // Powercell 1st pixel blinking
    static bool flash = false;
    if (PC_PWD_FLASH) // Blinking is enable
    {
        if (!flash && (millis() - _prevTime > PC_PWD_FLASH_OFF))
        {
            _prevTime = millis();
            flash = true;
        }
        if (flash && (millis() - _prevTime > PC_PWD_FLASH_ON))
        {
            _prevTime = millis();
            flash = false;
        }
    }
    else // Blinking DISABLE
    {
        flash = false;
    }

    for (uint8_t i = 0; i < _numLeds; i++)
    {
        if (i == 0 && flash)
        {
            _setColor(i, 0, 0, PC_BRIGHTNESS);
        }
        else
        {
            _setColor(i, 0, 0, 0);
        }
    }
    _levelTracker = 0;
    _shutdownTracker = _numLeds - 1;
}

void Powercell::boot(int16_t bootTime, bool init)
{ // Pixels drop down powercell and pile up!

    // trackers
    static int8_t bootTracker = _numLeds;
    if (init)
    {
        bootTracker = _numLeds;
        _levelTracker = 0;
        bootState = false;

        // Time intervals to update pixels
        uint16_t time_divider = 0;
        for (uint8_t i = _numLeds; i > 0; i--)
        {
            time_divider += i;
        }
        _updateSp = (bootTime / time_divider);
    }

    if (!bootState)
    {
        if (millis() - _prevTime >= _updateSp)
        {
            // Boot sequence not done
            {
                //_prevTime += bootSp;
                _prevTime = millis();

                if (_levelTracker < _numLeds)
                {
                    for (int8_t i = 0; i < _numLeds; i++)
                    {
                        if (i < _levelTracker || i == bootTracker)
                        {
                            _setColor(i, 0, 0, PC_BRIGHTNESS);
                        }
                        else
                        {
                            _setColor(i, 0, 0, 0);
                        }
                    }
                    bootTracker--;
                    if (bootTracker == _levelTracker)
                    {
                        _levelTracker++;
                        bootTracker = _numLeds;
                    }
                }
                else
                {
                    _setColorAll(0, 0, PC_BRIGHTNESS);
                    _levelTracker = 0;
                    bootTracker = _numLeds;
                    bootState = true;
                }
            }
        }
    }
    else
    // Sequence done,
    {
        _updateSp = PC_IDLE1_UPDATE_SP;
        _idleOne(_updateSp);
    }
}

void Powercell::rampToIdleOne(uint16_t ramp_time, bool init)
{
    _rampPowercell(ramp_time, init, PC_IDLE1_UPDATE_SP);
    _idleOne(_updateSp);
}

void Powercell::rampToIdleTwo(uint16_t ramp_time, bool init)
{
    _rampPowercell(ramp_time, init, PC_IDLE2_UPDATE_SP);
    _idleTwo(_updateSp);
}

void Powercell::rampToFiring(uint16_t ramp_time, bool init)
{
    _rampPowercell(ramp_time, init, PC_FIRE_UPDATE_SP);
    _idleOne(_updateSp);
}

void Powercell::shuttingDown(int16_t shutdownTime, bool init)
{

    // trackers
    static int8_t shutdownTracker = _numLeds;
    if (init)
    {
        shutdownTracker = _numLeds;
        _levelTracker = _numLeds - 1;
        bootState = true;

        // Time intervals to update pixels
        uint16_t time_divider = 0;
        for (uint8_t i = _numLeds; i > 0; i--)
        {
            time_divider += i;
        }
        _updateSp = (shutdownTime / time_divider);
    }

    if (bootState)
    {
        if (millis() - _prevTime >= _updateSp)
        {
            {
                //_prevTime += _updateSp;
                _prevTime = millis();

                if (_levelTracker >= 0)
                {
                    for (int8_t i = 0; i < _numLeds; i++)
                    {
                        if (i < _levelTracker || i == shutdownTracker)
                        {
                            _setColor(i, 0, 0, PC_BRIGHTNESS);
                        }
                        else
                        {
                            _setColor(i, 0, 0, 0);
                        }
                    }
                    shutdownTracker++;
                    if (shutdownTracker >= _numLeds)
                    {
                        _levelTracker--;
                        shutdownTracker = _levelTracker;
                    }
                }
                else
                {
                    _setColorAll(0, 0, 0);
                    _levelTracker = 0;
                    shutdownTracker = 0;
                    bootState = false;
                }
            }
        }
    }
    else
    // Sequence done,
    {
        poweredDown();
    }
}

void Powercell::_idleOne(int16_t idle1Sp)
{ // bar graph ramp up
    if (millis() - _prevTime >= idle1Sp)
    {
        _prevTime = millis();

        for (int8_t i = 0; i < _numLeds; i++)
        {
            if (i < _levelTracker)
            {
                _setColor(i, 0, 0, PC_BRIGHTNESS);
            }
            else
            {
                _setColor(i, 0, 0, 0);
            }
        }
        if (_levelTracker < _numLeds)
        {
            _levelTracker++;
        }
        else
        {
            _levelTracker = 0;
        }
    }
}

void Powercell::_idleTwo(int16_t idle2Sp)
{
    _idleOne(idle2Sp);
}

void Powercell::_firing(int16_t firingSp)
{
    _idleOne(firingSp);
}

void Powercell::_rampPowercell(int16_t rampTime, bool init, int16_t tg_speed)
{
    // trackers
    static int16_t iniUpSp;
    static int16_t int_updateSp;
    static int16_t incr_updateSp;
    static uint32_t prevRampUpdate = 0;

    // Records initial parameters and computes ramp intervals and increments when ramp is initiated
    if (init)
    {
        // Initial parameters
        iniUpSp = _updateSp;
        // Time intervals to rampup speeds
        int_updateSp = (rampTime / abs(tg_speed - iniUpSp));
        // Increments to rampup speeds
        incr_updateSp = 1;
    }

    // Ramp UPDATE SPEED
    if (millis() - prevRampUpdate >= int_updateSp)
    {
        prevRampUpdate = millis();
        _updateSp = _ramp_parameter(_updateSp, iniUpSp, tg_speed, incr_updateSp);
    }
}

int16_t Powercell::_ramp_parameter(int16_t param, int16_t ini, int16_t tg, int16_t incr)
{
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

void Powercell::_setColorAll(uint8_t red, uint8_t green, uint8_t blue)
{
    for (uint16_t i = 0; i < _numLeds; i++)
    {
        _ledState[i][0] = red;
        _ledState[i][1] = green;
        _ledState[i][2] = blue;
    }
}

void Powercell::_setColor(uint16_t pixel, uint8_t red, uint8_t green, uint8_t blue)
{
    _ledState[pixel][0] = red;
    _ledState[pixel][1] = green;
    _ledState[pixel][2] = blue;
}
