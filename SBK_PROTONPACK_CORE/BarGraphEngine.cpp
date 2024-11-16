/*
 *  BarGraphEngine.cpp is a part of SBK_PROTONPACK_CORE (VERSION 2.4) code for bar graph animations of a Proton Pack replica
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

#include "BarGraphEngine.h"

BarGraphAnimation::BarGraphAnimation(uint8_t numLeds) : _numLeds(numLeds) {}

bool BarGraphAnimation::getLedState(uint8_t index)
{
    return _ledState[index];
}

void BarGraphAnimation::clear()
{
    // reset the sequence
    _reverseSeqTracker = false;
    _runningLedTracker = 0;
    _fireSeqTracker = 0;
    _bootState = false;
    // shut all led's off
    setLow();
}

bool BarGraphAnimation::boot(uint8_t bootSp, uint8_t idle1Sp, bool init)
{
    static bool flag = false;
    if (init)
    {
        _runningLedTracker = _numLeds;
        _bootState = false;
        flag = false;
    }

    if (!_bootState)
    {
        if (millis() - _prevTime >= bootSp)
        {
            //_prevTime += (unsigned long)bootSp;
            _prevTime = millis();
            if (!flag)
            {
                for (int8_t i = _numLeds - 1; i >= 0; i--)
                {
                    if (i >= _runningLedTracker)
                    {
                        _ledState[i] = true;
                    }
                    else
                    {
                        _ledState[i] = false;
                    }
                }
                if (_runningLedTracker >= 0)
                {
                    _runningLedTracker--;
                }
                if (_runningLedTracker < 0)
                {
                    flag = true;
                    _runningLedTracker = _numLeds;
                }
            }
            else
            {
                for (int8_t i = _numLeds - 1; i >= 0; i--)
                {
                    if (i >= _runningLedTracker)
                    {
                        _ledState[i] = false;
                    }
                    else
                    {
                        _ledState[i] = true;
                    }
                }
                if (_runningLedTracker >= 0)
                {
                    _runningLedTracker--;
                }
                if (_runningLedTracker < 0)
                {
                    _bootState = true;
                    _runningLedTracker = 0;
                }
            }
        }
    }
    else
    {
        idleOne(idle1Sp);
    }
    return _bootState;
}

void BarGraphAnimation::idleOne(uint8_t idle1Sp)
{ // In Idle mode safety ON
    // normal sync animation on the bar graph while safety ON
    if (millis() - _prevTime >= idle1Sp)
    {
        _prevTime = millis();
        for (int8_t i = 0; i < _numLeds; i++)
        {
            // All segements equal and below running led tracker will be ON
            if (_runningLedTracker <= 0)
            {
                _ledState[i] = false;
            }
            else
            {
                if (i <= _runningLedTracker - 1)
                {
                    _ledState[i] = true;
                }
                else
                {
                    _ledState[i] = false;
                }
            }
        }
        if (_reverseSeqTracker == false)
        {
            _runningLedTracker++;
            if (_runningLedTracker > _numLeds)
            {
                _runningLedTracker = _numLeds;
                _reverseSeqTracker = true;
            }
        }
        else
        {
            if (_runningLedTracker > 0)
            {
                _runningLedTracker--;
            }
            if (_runningLedTracker == 0)
            {
                _reverseSeqTracker = false;
            }
        }
    }
}

void BarGraphAnimation::idleTwo(uint8_t idle2Sp)
{
    // normal sync animation on the bar graph while safety OFF
    if (millis() - _prevTime >= idle2Sp)
    {
        _prevTime = millis();
        // constrain(_runningLedTracker, 1, 10);
        for (int8_t i = 0; i < _numLeds; i++)
        {
            // Only segement equal to running led tracker will be ON
            if (i == _runningLedTracker - 1)
            {
                _ledState[i] = true;
            }
            else
            {
                _ledState[i] = false;
            }
        }
        if (_reverseSeqTracker == false)
        {
            _runningLedTracker++;
            if (_runningLedTracker >= _numLeds)
            {
                _runningLedTracker = _numLeds;
                _reverseSeqTracker = true;
            }
        }
        else
        {
            _runningLedTracker--;
            if (_runningLedTracker <= 1)
            {
                _runningLedTracker = 1;
                _reverseSeqTracker = false;
            }
        }
    }
}

void BarGraphAnimation::firing(uint8_t firingSp)
{
    if (millis() - _prevTime >= firingSp)
    {
        _prevTime = millis();
        for (int8_t i = 0; i < _numLeds; i++)
        {
            // For low segments (16 and less) bar graph
            if (_numLeds <= 16)
            {
                if ((i == _fireSeqTracker) || (i == (_numLeds - 1) - _fireSeqTracker))
                {
                    _ledState[i] = true;
                }
                else
                {
                    _ledState[i] = false;
                }
            }
            // For many segements bar graph (17 and more)
            else
            {
                if ((i == _fireSeqTracker) ||
                    (i == _fireSeqTracker + 1) ||
                    (i == (_numLeds - 1) - _fireSeqTracker) ||
                    (i == (_numLeds - 1) - (_fireSeqTracker - 1)))
                {
                    _ledState[i] = true;
                }
                else
                {
                    _ledState[i] = false;
                }
            }
        }
        if (_reverseSeqTracker == false)
        {
            _fireSeqTracker++;
            if (_fireSeqTracker >= ((_numLeds / 2) - 1))
            {
                _fireSeqTracker = ((_numLeds / 2) - 1);
                _reverseSeqTracker = true;
            }
        }
        else
        {
            if (_fireSeqTracker > 0)
            {
                _fireSeqTracker--;
            }
            if (_fireSeqTracker <= 0)
            {
                _reverseSeqTracker = false;
            }
        }
    }
}

bool BarGraphAnimation::shuttingDown(uint8_t shutdownSp, bool init)
{
    static bool flag = false;
    if (init)
    {
        _runningLedTracker = 0;
        _bootState = true;
        flag = false;
    }

    if (_bootState)
    {
        if (millis() - _prevTime >= shutdownSp)
        {
            //_prevTime += shutdownSp;
            _prevTime = millis();
            if (!flag)
            {
                for (int8_t i = 0; i < _numLeds; i++)
                {
                    if (i < _runningLedTracker)
                    {
                        _ledState[i] = true;
                    }
                    else
                    {
                        _ledState[i] = false;
                    }
                }
                if (_runningLedTracker < _numLeds)
                {
                    _runningLedTracker++;
                }
                if (_runningLedTracker >= _numLeds)
                {
                    flag = true;
                    _runningLedTracker = 0;
                }
            }
            else
            {
                for (int8_t i = 0; i < _numLeds; i++)
                {
                    if (i < _runningLedTracker)
                    {
                        _ledState[i] = false;
                    }
                    else
                    {
                        _ledState[i] = true;
                    }
                }
                if (_runningLedTracker < _numLeds)
                {
                    _runningLedTracker++;
                }
                if (_runningLedTracker >= _numLeds)
                {
                    _bootState = false;
                    _runningLedTracker = 0;
                }
            }
        }
    }
    else
    {
        clear();
    }
    return _bootState;
}

void BarGraphAnimation::setHigh()
{
    for (uint8_t i = 0; i < _numLeds; i++)
    {
        _ledState[i] = 1; // All LEDs ON
    }
}

void BarGraphAnimation::setLow()
{
    for (uint8_t i = 0; i < _numLeds; i++)
    {
        _ledState[i] = 0; // All LEDs OFF
    }
}

/*************************************************************************************************************/
/*************************************************************************************************************/
/*************************************************************************************************************/

HT16K33Driver::HT16K33Driver(uint8_t numLeds, bool direction, uint8_t dataPin, uint8_t clockPin, uint8_t address)
    : BarGraphAnimation(numLeds), _numLeds(numLeds), _direction(direction), _clockPin(clockPin), _dataPin(dataPin), _address(address)
{
}

void HT16K33Driver::begin(const uint8_t segMap[][2], uint8_t rows, uint8_t cols)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            _segMap[i][j] = segMap[i][j];
        }
    }
    _driver.init(_address);
    _driver.setBrightness(15); // Set maxBri level (0 is min, 15 is max)
    _driver.clear();
    clear();
}

void HT16K33Driver::update()
{
    // To be configure for in relation with bar graph total leds number and connections matrix to the MAX72xx
    // Leds mapping might be different for your setup, check rows and columns orders : _bargraph.setLed(0, ROW, COL, _ledState[i]))
    for (uint8_t i = 0; i < _numLeds; i++)
    {
        uint8_t j = i;
        if (_direction) // If animation is REVERSED
        {
            j = (_numLeds - 1) - i;
        }
        // set segments according to mapping define in setting
        _driver.setPixel(_segMap[j][0], _segMap[j][1], getLedState(i));
    }
    _driver.write();
}

/*************************************************************************************************************/
/*************************************************************************************************************/
/*************************************************************************************************************/

MAX72xxDriver::MAX72xxDriver(uint8_t numLeds, bool direction, uint8_t dataPin, uint8_t clockPin, uint8_t loadPin)
    : BarGraphAnimation(numLeds), _numLeds(numLeds), _direction(direction), _loadPin(loadPin), _clockPin(clockPin), _dataPin(dataPin), _driver(LedControl(dataPin, clockPin, loadPin, 1))
{
}

void MAX72xxDriver::begin(const uint8_t segMap[][2], uint8_t rows, uint8_t cols)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            _segMap[i][j] = segMap[i][j];
        }
    }
    pinMode(_loadPin, OUTPUT);
    pinMode(_clockPin, OUTPUT);
    pinMode(_dataPin, OUTPUT);
    _driver.shutdown(0, false);
    _driver.setIntensity(0, 8); // Set maxBri level (0 is min, 15 is max)
    _driver.clearDisplay(0);
    clear();
}

void MAX72xxDriver::update()
{
    // To be configure for in relation with bar graph total leds number and connections matrix to the MAX72xx
    // Leds mapping might be different for your setup, check rows and columns orders : _bargraph.setLed(0, ROW, COL, _ledState[i]))
    for (uint8_t i = 0; i < _numLeds; i++)
    {
        uint8_t j = i;
        if (_direction) // If animation is REVERSED
        {
            j = (_numLeds - 1) - i;
        }
        // set segments according to mapping define in setting
        _driver.setLed(0, _segMap[j][0], _segMap[j][1], getLedState(i));
    }
}
