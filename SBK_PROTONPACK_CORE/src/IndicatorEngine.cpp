/*
 *  IndicatorEngine.cpp is a part of SBK_PROTONPACK_CORE (VERSION 2.4) code for animations of a Proton Pack replica
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
    _flashingState = false;
    _pulse = false;
}

void Indicator::begin() { clear(); }

void Indicator::setColor(uint8_t red, uint8_t green, uint8_t blue)
{
   // _strip.setPixelColor(_pixel, _strip.gamma8(red), _strip.gamma8(green), _strip.gamma8(blue));
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

void Indicator::white(uint16_t updateSp) // flashing
{
    if (updateSp > 0)
    {
        if (!_flashingState)
        {
            _pulse = false;
            _flashingState = true;
        }
        if ((millis() - _prevTime) >= updateSp)
        {
            _prevTime = millis();
            if (_pulse == true)
            {
                _white();
                _pulse = false;
            }
            else
            {
                clear();
                _pulse = true;
            }
        }
    }
    else
    {
        _white();
        _flashingState = 0;
    }
}

void Indicator::red(uint16_t updateSp) // flashing
{
    if (updateSp > 0)
    {
        if (!_flashingState)
        {
            _pulse = false;
            _flashingState = true;
        }
        if ((millis() - _prevTime) >= updateSp)
        {
            _prevTime = millis();
            if (_pulse == true)
            {
                _red();
                _pulse = false;
            }
            else
            {
                clear();
                _pulse = true;
            }
        }
    }
    else
    {
        _red();
        _flashingState = 0;
    }
}

void Indicator::yellow(uint16_t updateSp) // flashing
{
    if (updateSp > 0)
    {
        if (!_flashingState)
        {
            _pulse = false;
            _flashingState = true;
        }
        if ((millis() - _prevTime) >= updateSp)
        {
            _prevTime = millis();
            if (_pulse == true)
            {
                _yellow();
                _pulse = false;
            }
            else
            {
                clear();
                _pulse = true;
            }
        }
    }
    else
    {
        _yellow();
        _flashingState = 0;
    }
}

void Indicator::green(uint16_t updateSp) // flashing
{
    if (updateSp > 0)
    {
        if (!_flashingState)
        {
            _pulse = false;
            _flashingState = true;
        }
        if ((millis() - _prevTime) >= updateSp)
        {
            _prevTime = millis();
            if (_pulse == true)
            {
                _green();
                _pulse = false;
            }
            else
            {
                clear();
                _pulse = true;
            }
        }
    }
    else
    {
        _green();
        _flashingState = 0;
    }
}

void Indicator::orange(uint16_t updateSp) // flashing
{
    if (updateSp > 0)
    {
        if (!_flashingState)
        {
            _pulse = false;
            _flashingState = true;
        }
        if ((millis() - _prevTime) >= updateSp)
        {
            _prevTime = millis();
            if (_pulse == true)
            {
                _orange();
                _pulse = false;
            }
            else
            {
                clear();
                _pulse = true;
            }
        }
    }
    else
    {
        _orange();
        _flashingState = 0;
    }
}

void Indicator::_white()
{
    setColor(150, 150, 150);
}

void Indicator::_red()
{
    // setColor( _strip.gamma8(201),  _strip.gamma8(0),  _strip.gamma8(0));
    setColor(255, 0, 0);
}

void Indicator::_yellow()
{
    // setColor( _strip.gamma8(255),  _strip.gamma8(255),  _strip.gamma8(90));
    setColor(255, 200, 0);
}

void Indicator::_green()
{

    // setColor( _strip.gamma8(173),  _strip.gamma8(255),  _strip.gamma8(90));
    setColor(12, 189, 24);
}

void Indicator::_orange()
{
    // setColor( _strip.gamma8(253),  _strip.gamma8(166),  _strip.gamma8(0));
    setColor(255, 100, 0);
}




SingleColorIndicator::SingleColorIndicator(uint8_t indicator_pin, bool state)
    : _indicator_pin(indicator_pin), _state(state)
{
    _prevTime = 0;
    _flashingState = false;
    _pulse = false;
}

void SingleColorIndicator::begin() { 
  pinMode(_indicator_pin,OUTPUT);
  _write(_state); }

void SingleColorIndicator::_write(bool state){
    if(state !=_state){
    digitalWrite(_indicator_pin,state);
    _state = state;}
}

void SingleColorIndicator::on()
{
    _write(true);
}

void SingleColorIndicator::off()
{
    _write(false);
}

void SingleColorIndicator::clear()
{
    _write(false);
}

void SingleColorIndicator::flash(uint16_t updateSp) // flashing
{
    if (updateSp > 0)
    {
        if (!_flashingState)
        {
            _pulse = false;
            _flashingState = true;
        }
        if ((millis() - _prevTime) >= updateSp)
        {
            _prevTime = millis();
            if (_pulse == true)
            {
                on();
                _pulse = false;
            }
            else
            {
                off();
                _pulse = true;
            }
        }
    }
    else
    {
        on();
        _flashingState = 0;
    }
}
