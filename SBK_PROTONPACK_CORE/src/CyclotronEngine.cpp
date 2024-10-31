/*
 *  CyclotronEngine.cpp is a part of SBK_PROTONPACK_CORE (VERSION 2.4) code for animations of a Proton Pack replica
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

#include "CyclotronEngine.h"

///////////////////////////////////////////
///////////// GB1 / GB2 STYLE /////////////
///////////////////////////////////////////

// Cyclotron GB1/GB2 STYLE animations variables
// POWERDOWN animation targets
#define GB12_PWD_UPDATE_SP 30
#define GB12_PWD_FADE_SP 0
#define GB12_PWD_MAX_BRIGHTNESS 10
#define GB12_PWD_FLASH_DURATION 5
#define GB12_PWD_OFFSET 25
// IDLE ONE animation targets
#define GB12_IDLE1_UPDATE_SP 20
#define GB12_IDLE1_FADE_SP 5
#define GB12_IDLE1_MAX_BRIGHTNESS 100
#define GB12_IDLE1_FLASH_DURATION 100
#define GB12_IDLE1_OFFSET 50
// IDLE TWO animation targets
#define GB12_IDLE2_UPDATE_SP 15
#define GB12_IDLE2_FADE_SP 10
#define GB12_IDLE2_MAX_BRIGHTNESS 150
#define GB12_IDLE2_FLASH_DURATION 50
#define GB12_IDLE2_OFFSET 70
// STATE_FIRING animation targets
#define GB12_FIRE_UPDATE_SP 5
#define GB12_FIRE_FADE_SP 25
#define GB12_FIRE_MAX_BRIGHTNESS 255
#define GB12_FIRE_FLASH_DURATION 0
#define GB12_FIRE_OFFSET 200

// Cyclotron GB1/GB2 style object and functions

Cyclotron_GB1_GB2::Cyclotron_GB1_GB2(Adafruit_NeoPixel &strip, bool direction,
                                     uint8_t start1, uint8_t end1,
                                     uint8_t start2, uint8_t end2,
                                     uint8_t start3, uint8_t end3,
                                     uint8_t start4, uint8_t end4)
    : _strip(strip), _direction(direction),
      _start1(start1), _end1(end1),
      _start2(start2), _end2(end2),
      _start3(start3), _end3(end3),
      _start4(start4), _end4(end4)
{
    _numLeds = (_end4 - _start1 + 1);
    _ledState = new uint8_t *[_numLeds];
    for (uint8_t i = 0; i < _numLeds; ++i)
    {
        _ledState[i] = new uint8_t[3];
    }
    _prevTime = 0;
    // initial sequence variables
    _cycUpdateSp = GB12_PWD_UPDATE_SP;
    _cycFadeSp = GB12_PWD_FADE_SP;
    _cycBrightness = GB12_PWD_MAX_BRIGHTNESS;
    _cycFlashDuration = GB12_PWD_FLASH_DURATION;
    _cycPosOffset = GB12_PWD_OFFSET;
    // Idle One sequence variables
    _cell1Tracker = 0;
    _cell2Tracker = 0;
    _cell3Tracker = 0;
    _cell4Tracker = 0;
    _cell1State = true;
    _cell2State = false;
    _cell3State = false;
    _cell4State = false;
    _prevUpdateSpTime = 0;
    _prevFadeTime = 0;
    _prevBrightnessTime = 0;
    _prevFlasDurTime = 0;
    _prevOffsetTime = 0;
}

Cyclotron_GB1_GB2::~Cyclotron_GB1_GB2()
{
    for (int i = 0; i < _numLeds; ++i)
    {
        delete[] _ledState[i]; // Free memory for each row
    }
    delete[] _ledState;
}

void Cyclotron_GB1_GB2::begin() { clear(); }

void Cyclotron_GB1_GB2::clear()
{
    _setColorAll(0, 0, 0);
    _cycUpdateSp = GB12_PWD_UPDATE_SP;
    _cycFadeSp = GB12_PWD_FADE_SP;
    _cycBrightness = GB12_PWD_MAX_BRIGHTNESS;
    _cycFlashDuration = GB12_PWD_FLASH_DURATION;
    _cycPosOffset = GB12_PWD_OFFSET;
    _cell1Tracker = 0;
    _cell2Tracker = 0;
    _cell3Tracker = 0;
    _cell4Tracker = 0;
    _cell1State = true;
    _cell2State = false;
    _cell3State = false;
    _cell4State = false;
}

void Cyclotron_GB1_GB2::update()
{
    for (int8_t i = 0; i < _numLeds; i++)
    {
        // Offset index to fit ws2812 LEDs strip index
        uint8_t j;
        if (!_direction) // FORWARD durection
        {
            j = i + _start1;
        }
        else
        {
            j = _end4 - i;
        }
        // set segments according to mapping define in setting
        _strip.setPixelColor((j), _ledState[i][0], _ledState[i][1], _ledState[i][2]);
    }
}

void Cyclotron_GB1_GB2::rampToPoweredDown(uint16_t ramp_time, bool init)
{
    _rampCyc(ramp_time, init, GB12_PWD_UPDATE_SP, GB12_PWD_FADE_SP, GB12_PWD_MAX_BRIGHTNESS, GB12_PWD_FLASH_DURATION, GB12_PWD_OFFSET);
}

void Cyclotron_GB1_GB2::rampToIdleOne(uint16_t ramp_time, bool init) // Return true when animation is done
{
    _rampCyc(ramp_time, init, GB12_IDLE1_UPDATE_SP, GB12_IDLE1_FADE_SP, GB12_IDLE1_MAX_BRIGHTNESS, GB12_IDLE1_FLASH_DURATION, GB12_IDLE1_OFFSET);
}

void Cyclotron_GB1_GB2::rampToIdleTwo(uint16_t ramp_time, bool init) // Return true when animation is done
{
    _rampCyc(ramp_time, init, GB12_IDLE2_UPDATE_SP, GB12_IDLE2_FADE_SP, GB12_IDLE2_MAX_BRIGHTNESS, GB12_IDLE2_FLASH_DURATION, GB12_IDLE2_OFFSET);
}

void Cyclotron_GB1_GB2::rampToFiring(uint16_t ramp_time, bool init) // Return true when animation is done
{
    _rampCyc(ramp_time, init, GB12_FIRE_UPDATE_SP, GB12_FIRE_FADE_SP, GB12_FIRE_MAX_BRIGHTNESS, GB12_FIRE_FLASH_DURATION, GB12_FIRE_OFFSET);
}

void Cyclotron_GB1_GB2::setDirection(bool direction)
{
    _direction = direction;
}

void Cyclotron_GB1_GB2::_idleCyc(int16_t updateSp, int16_t fadeSp, int16_t maxBri, int16_t flashDur, int16_t offset)
{
    _cycUpdateSp = updateSp;
    _cycFadeSp = fadeSp;
    _cycBrightness = maxBri;
    _cycFlashDuration = flashDur;
    _cycPosOffset = offset;

    // Update cyclotron
    if (millis() - _prevTime >= _cycUpdateSp)
    {
        //_prevTime += _cycUpdateSp; //This cause problem at powerup MCU ????
        _prevTime = millis();

        // Update cyclotron _rotation
        _rotation();
    }
}

void Cyclotron_GB1_GB2::_rampCyc(int16_t rampTime, bool init, int16_t tg_updateSp, int16_t tg_fadeSp, int16_t tg_maxBri, int16_t tg_flashDur, int16_t tg_offset)
{
    // trackers
    static int16_t iniUpSp;
    static int16_t iniFadeSp;
    static int16_t iniBri;
    static int16_t iniDur;
    static int16_t iniOff;
    static int16_t int_updateSp;
    static int16_t int_fadeSp;
    static int16_t int_bri;
    static int16_t int_dur;
    static int16_t int_off;
    static int16_t incr_updateSp;
    static int16_t incr_fadeSp;
    static int16_t incr_bri;
    static int16_t incr_dur;
    static int16_t incr_off;

    // Records initial cyclotron parameters and computes ramp intervals and increments when ramp is initiated
    if (init)
    {
        // Initial parameters
        iniUpSp = _cycUpdateSp;
        iniFadeSp = _cycFadeSp;
        iniBri = _cycBrightness;
        iniDur = _cycFlashDuration;
        iniOff = _cycPosOffset;
        // Time intervals to rampup cycloton speeds and brightness
        int_updateSp = (rampTime / abs(tg_updateSp - iniUpSp));
        int_fadeSp = rampTime / abs(tg_fadeSp - iniFadeSp);
        int_bri = rampTime / abs(tg_maxBri - iniBri);
        int_dur = rampTime / abs(tg_flashDur - iniDur);
        int_off = rampTime / abs(tg_offset - iniOff);
        // Increments to rampup cycloton speeds and brightness
        incr_updateSp = 1;
        incr_fadeSp = 1;
        incr_bri = 1;
        incr_dur = 1;
        incr_off = 1;
        // Serial.println("cyclotron ramp initialisation !");
    }

    // Update cyclotron with current speeds and brightness
    _rotation();

    // Ramp cyclotron UPDATE SPEED
    if (millis() - _prevUpdateSpTime >= int_updateSp)
    {
        _prevUpdateSpTime = millis();
        _cycUpdateSp = _ramp_parameter(_cycUpdateSp, iniUpSp, tg_updateSp, incr_updateSp);
    }

    // Ramp cyclotron FADE SPEED
    if (millis() - _prevFadeTime >= int_fadeSp)
    {
        _prevFadeTime = millis();
        _cycFadeSp = _ramp_parameter(_cycFadeSp, iniFadeSp, tg_fadeSp, incr_fadeSp);
    }

    // Ramp cyclotron BRIGHTNESS
    if (millis() - _prevBrightnessTime >= int_bri)
    {
        _prevBrightnessTime = millis();
        _cycBrightness = _ramp_parameter(_cycBrightness, iniBri, tg_maxBri, incr_bri);
        // Helper to clear cyclotron at shutdown : if ramping down and brightness nearly 0, put it to 0...
        if (iniBri > tg_maxBri && _cycBrightness <= 10)
        {
            _cycBrightness = 0;
        }
    }

    // Ramp cyclotron FLASH DURATION
    if (millis() - _prevFlasDurTime >= int_dur)
    {
        _prevFlasDurTime = millis();
        _cycFlashDuration = _ramp_parameter(_cycFlashDuration, iniDur, tg_flashDur, incr_dur);
    }

    // Ramp cyclotron position OFFSET
    if (millis() - _prevOffsetTime >= int_off)
    {
        _prevOffsetTime = millis();
        _cycPosOffset = _ramp_parameter(_cycPosOffset, iniOff, tg_offset, incr_off);
    }
}

uint16_t Cyclotron_GB1_GB2::_ramp_parameter(int16_t param, int16_t ini, int16_t tg, int16_t incr)
{
    if ((tg - ini) >= 0)
    { // Ramp UP
        if (param + incr <= tg)
        {
            param += incr;
            // Serial.print("_cycUpdateSp "), Serial.println(_cycUpdateSp);
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
            param -= incr;
            if (param < 0)
            {
                param = 0;
            }
        }
        // Serial.print("_cycUpdateSp "), Serial.println(_cycUpdateSp);
    }
    return param;
}

void Cyclotron_GB1_GB2::_rotation()
{
    if (millis() - _prevTime >= _cycUpdateSp)
    {
        _prevTime = millis();
        // Update cyclotron one position cycle duration
        _cycPosDuration = ((2 * _cycBrightness) + _cycFlashDuration);

        _cell1();
        _cell2();
        _cell3();
        _cell4();

        /*
 Serial.print(tg_updateSp), Serial.print(" / "), Serial.println(_cycUpdateSp);
 Serial.print(tg_fadeSp), Serial.print(" / "), Serial.println(_cycFadeSp);
 Serial.print(tg_maxBri), Serial.print(" / "), Serial.println(_cycBrightness);
 Serial.print(tg_flashDur), Serial.print(" / "), Serial.println(_cycFlashDuration);
 Serial.print(iniDur), Serial.print(" / "), Serial.println(incr_dur);
 Serial.print(tg_offset), Serial.print(" / "), Serial.println(_cycPosOffset);
 Serial.println();
 */
    }
}

void Cyclotron_GB1_GB2::_cell1()
{

    if (_cell1State)
    {
        _cellIntensity(_cell1State, _cell1Tracker, _start1, _end1);
        if (_cell1Tracker < _cycPosDuration)
        {
            _cell1Tracker += _cycFadeSp;
        }
        else
        {
            _cellClear(_start1, _end1);
            _cell1Tracker = 0;
            _cell1State = false;
            // Serial.println("Cell 1 : OFF");
        }
    }

    // Start the next cell with offset
    if (!_cell2State && _cell1Tracker > _cycPosDuration - _cycPosOffset)
    {
        _cell2State = true;
        // Serial.println("Cell 2 : ON");
    }
}

void Cyclotron_GB1_GB2::_cell2()
{

    if (_cell2State)
    {
        _cellIntensity(_cell2State, _cell2Tracker, _start2, _end2);
        if (_cell2Tracker < _cycPosDuration)
        {
            _cell2Tracker += _cycFadeSp;
        }
        else
        {
            _cellClear(_start2, _end2);
            _cell2Tracker = 0;
            _cell2State = false;
            // Serial.println("Cell 2 : OFF");
        }
    }

    if (!_cell3State && _cell2Tracker > _cycPosDuration - _cycPosOffset)
    {
        _cell3State = true;
        // Serial.println("Cell 3 : ON");
    }
}

void Cyclotron_GB1_GB2::_cell3()
{
    _cellIntensity(_cell3State, _cell3Tracker, _start3, _end3);
    if (_cell3State)
    {
        _cellIntensity(_cell3State, _cell3Tracker, _start3, _end3);
        if (_cell3Tracker < _cycPosDuration)
        {
            _cell3Tracker += _cycFadeSp;
        }
        else
        {
            _cellClear(_start3, _end3);
            _cell3Tracker = 0;
            _cell3State = false;
            // Serial.println("Cell 3 : OFF");
        }
    }
    if (!_cell4State && _cell3Tracker > _cycPosDuration - _cycPosOffset)
    {
        _cell4State = true;
        // Serial.println("Cell 4 : ON");
    }
}

void Cyclotron_GB1_GB2::_cell4()
{

    if (_cell4State)
    {
        _cellIntensity(_cell4State, _cell4Tracker, _start4, _end4);
        if (_cell4Tracker < _cycPosDuration)
        {
            _cell4Tracker += _cycFadeSp;
        }
        else
        {
            _cellClear(_start4, _end4);
            _cell4Tracker = 0;
            _cell4State = false;
            // Serial.println("Cell 4 : OFF");
        }
    }
    if (!_cell1State && _cell4Tracker > _cycPosDuration - _cycPosOffset)
    {
        _cell1State = true;
        // Serial.println("Cell 1 : ON");
    }
}

uint8_t Cyclotron_GB1_GB2::_cellIntensity(bool state, int16_t tracker, uint8_t start, uint8_t end)
{
    uint8_t intensity;
    for (int8_t i = start; i < end + 1; i++)
    {
        // Offset index to ledState[28][3]array
        uint8_t j = i - _start1;

        if (state)
        {
            // ramp up intensity
            if (tracker < _cycBrightness + 1)
            {
                intensity = tracker;
            }
            // Flash at same brightness
            else if ((tracker < (_cycPosDuration - _cycBrightness)) || (_cycBrightness > tracker))
            {
                intensity = _cycBrightness;
            }
            // ramp down
            else
            {
                intensity = max(0, _cycPosDuration - tracker);
            }
        }
        else
        {
            intensity = 0;
        }

        _setColor(j, intensity, 0, 0);
    }
    return intensity;
}

void Cyclotron_GB1_GB2::_cellClear(uint8_t start, uint8_t end)
{
    for (int8_t i = start; i < end + 1; i++)
    {
        // Offset index to ledState[28][3]array
        uint8_t j = i - _start1;

        _setColor(j, 0, 0, 0);
    }
}

void Cyclotron_GB1_GB2::_setColorAll(uint8_t red, uint8_t green, uint8_t blue)
{
    for (uint16_t i = 0; i < _numLeds; i++)
    {
        _ledState[i][0] = red;
        _ledState[i][1] = green;
        _ledState[i][2] = blue;
    }
}

void Cyclotron_GB1_GB2::_setColor(uint16_t pixel, uint8_t red, uint8_t green, uint8_t blue)
{
    _ledState[pixel][0] = red;
    _ledState[pixel][1] = green;
    _ledState[pixel][2] = blue;
}

///////////////////////////////////////////////////////////
///////////// AFTERLIFE / FROZEN EMPIRE STYLE /////////////
///////////////////////////////////////////////////////////

// Cyclotron AF/FE STYLE animations variables
// POWERDOWN animation targets
#define AFFE_PWD_UPDATE_SP 60
#define AFFE_PWD_MAX_BRIGHTNESS 0
#define AFFE_PWD_HEAD 0
#define AFFE_PWD_FLASH 0
#define AFFE_PWD_TRAIL 0
// IDLE ONE animation targets
#define AFFE_IDLE1_UPDATE_SP 30
// #define AFFE_IDLE1_HEAD 1
// #define AFFE_IDLE1_FLASH 1
// #define AFFE_IDLE1_TRAIL 5
//  IDLE TWO animation targets
#define AFFE_IDLE2_UPDATE_SP 20
// #define AFFE_IDLE2_HEAD 2
// #define AFFE_IDLE2_FLASH 2
// #define AFFE_IDLE2_TRAIL 7
//  STATE_FIRING animation targets
#define AFFE_FIRE_UPDATE_SP 10
#define AFFE_FIRE_MAX_BRIGHTNESS 255
#define AFFE_FIRE_HEAD 4
#define AFFE_FIRE_FLASH 6
#define AFFE_FIRE_TRAIL 25

// Cyclotron GB1/GB2 style object and functions

Cyclotron_AF_FE::Cyclotron_AF_FE(Adafruit_NeoPixel &strip, bool direction, uint8_t start, uint8_t end)
    : _strip(strip), _direction(direction), _start(start), _end(end)
{
    _numLeds = (_end - _start + 1);
    _ledState = new uint8_t *[_numLeds];
    for (uint8_t i = 0; i < _numLeds; ++i)
    {
        _ledState[i] = new uint8_t[3];
    }
    _prevTime = 0;
    _cycPosTracker = 0;
    // initial sequence variables
    _cycUpdateSp = AFFE_PWD_UPDATE_SP;
    _cycBrightness = AFFE_PWD_MAX_BRIGHTNESS;
    _cycTrail = AFFE_PWD_TRAIL;
    _cycFlash = AFFE_PWD_FLASH;
    _cycHead = AFFE_FIRE_HEAD;
    // Idle One sequence variables
    _prevUpdateSpTime = 0;
    _prevFadeTime = 0;
    _prevBrightnessTime = 0;
    _prevOffsetTime = 0;
}

Cyclotron_AF_FE::~Cyclotron_AF_FE()
{
    for (int i = 0; i < _numLeds; ++i)
    {
        delete[] _ledState[i]; // Free memory for each row
    }
    delete[] _ledState;
}

void Cyclotron_AF_FE::begin() { clear(); }

void Cyclotron_AF_FE::clear()
{
    _setColorAll(0, 0, 0);
    _cycUpdateSp = AFFE_PWD_UPDATE_SP;
    _cycBrightness = AFFE_PWD_MAX_BRIGHTNESS;
    _cycTrail = AFFE_PWD_TRAIL;
    _cycFlash = AFFE_PWD_FLASH;
    _cycHead = AFFE_FIRE_HEAD;
}

void Cyclotron_AF_FE::update()
{
    for (int8_t i = 0; i < _numLeds; i++)
    {

        // set segments according to mapping define in setting
        if (!_direction)
        {
            _strip.setPixelColor((i + _start), _ledState[i][0], _ledState[i][1], _ledState[i][2]);
        }
        else
        {
            _strip.setPixelColor((_end - i), _ledState[i][0], _ledState[i][1], _ledState[i][2]);
        }
    }
}

void Cyclotron_AF_FE::rampToPoweredDown(uint16_t ramp_time, bool init)
{
    _ramp(ramp_time, init, AFFE_PWD_UPDATE_SP, 2);
}

void Cyclotron_AF_FE::rampToIdleOne(uint16_t ramp_time, bool init) // Return true when animation is done
{
    _ramp(ramp_time, init, AFFE_IDLE1_UPDATE_SP, 2);
}

void Cyclotron_AF_FE::rampToIdleTwo(uint16_t ramp_time, bool init) // Return true when animation is done
{
    _ramp(ramp_time, init, AFFE_IDLE2_UPDATE_SP, 3);
}

void Cyclotron_AF_FE::rampToFiring(uint16_t ramp_time, bool init) // Return true when animation is done
{
    _ramp(ramp_time, init, AFFE_FIRE_UPDATE_SP, 3);
}

void Cyclotron_AF_FE::setDirection(bool direction)
{
    _direction = direction;
}

void Cyclotron_AF_FE::_idle(uint16_t updateSp, uint8_t tracker_increment) // Return true when animation is done
{
    _cycUpdateSp = updateSp;
    // Update cyclotron
    if (millis() - _prevTime >= _cycUpdateSp)
    {
        /*
         Serial.print("_cycUpdateSp "), Serial.print(_cycUpdateSp);
         Serial.print("  _cycFadeSp "), Serial.print(_cycFadeSp);
         Serial.print("  _cycBrightness "), Serial.print(_cycBrightness);
         Serial.print("  _cycFlashDuration "), Serial.print(_cycFlashDuration);
         Serial.print("  _cycPosDuration "), Serial.print(_cycPosDuration);
         Serial.print("  _cycPosOffset "), Serial.println(_cycPosOffset);
        */

        //_prevTime += _cycUpdateSp; //This cause problem at powerup MCU ????
        _prevTime = millis();

        _rotation();
        _cycPosTracker += tracker_increment;
        if (_cycPosTracker > _numLeds - 1)
        {
            _cycPosTracker = 0;
        }
    }
}

void Cyclotron_AF_FE::_ramp(uint16_t rampTime, bool init, int16_t tg_updateSp, uint8_t track_inc)
{

    static int16_t iniUpSp;
    static uint16_t incr_updateSp;
    static uint16_t int_updateSp;

    // Ramp not done yet, ramp the cyclotron speeds and brightness

    // Record initial cyclotron speeds and brightness
    if (init)
    {
        iniUpSp = _cycUpdateSp;
        // Increments to rampup cycloton speeds and brightness
        incr_updateSp = 1;
        // Time intervals to rampup cycloton speeds and brightness
        int_updateSp = rampTime / abs(tg_updateSp - iniUpSp);
        // Update cyclotron with current speeds and brightness

        // Serial.println("cyclotron ramp initialisation !");
    }

    _idle(_cycUpdateSp, track_inc);

    // Ramp cyclotron UPDATE SPEED
    if (millis() - _prevUpdateSpTime >= int_updateSp)
    {
        _prevUpdateSpTime = millis();
        _cycUpdateSp = _ramp_parameter(_cycUpdateSp, iniUpSp, tg_updateSp, incr_updateSp);
    }
}

int16_t Cyclotron_AF_FE::_ramp_parameter(int16_t param, int16_t ini, int16_t tg, int16_t incr)
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

void Cyclotron_AF_FE::_rotation()
{
    uint16_t factor = 1000 * (AFFE_PWD_UPDATE_SP - _cycUpdateSp) / (AFFE_PWD_UPDATE_SP - AFFE_FIRE_UPDATE_SP);
    _cycBrightness = min(255, 100 + (AFFE_FIRE_MAX_BRIGHTNESS * factor) / 1000);
    _cycHead = (AFFE_FIRE_HEAD * factor) / 1000;
    _cycFlash = (AFFE_FIRE_FLASH * factor) / 1000;
    _cycTrail = (AFFE_FIRE_TRAIL * factor) / 1000;
    /*
      Serial.print("Position = "), Serial.print(_cycPosTracker);
      Serial.print("   _cycBrightness = "), Serial.print(_cycBrightness);
      Serial.print("  _cycHead = "), Serial.print(_cycHead);
      Serial.print("  _cycTrail = "), Serial.println(_cycTrail);
      Serial.print("_cycPosTracker - _cycTrail = "), Serial.print(_cycPosTracker - _cycTrail);
      Serial.print("  _cycPosTracker - _cycTrail + _numLeds = "), Serial.println(_cycPosTracker - _cycTrail + _numLeds);
   */

    for (int8_t i = (_cycPosTracker - _cycTrail); i < (_cycPosTracker - _cycTrail + _numLeds); i++)
    {
        // Correct i between 0 to _numleds
        int8_t j = i;
        if (i < 0)
        {
            j = _numLeds + i;
        }
        else if (i > _numLeds - 1)
        {
            j = i - _numLeds;
        }

        // Serial.print("   i = "), Serial.print(i), Serial.print("   j = "), Serial.println(j);
        //  Trail pixels
        if (i < _cycPosTracker)
        {
            uint8_t redTail = min(255, (_cycBrightness / 3) * ((_cycTrail + 1) - (_cycPosTracker - i)) / (_cycTrail + 1));
            _setColor(j, redTail, redTail / 25, 0);
        }
        // Flash pixels
        else if (i < _cycPosTracker + _cycFlash)
        {
            uint8_t flashBlue = 0;
            if (_cycUpdateSp < 10)
            {
                flashBlue = 1;
            }
            else if (_cycUpdateSp < 20)
            {
                flashBlue = 1;
            }
            _setColor(j, _cycBrightness, _cycBrightness / 5, flashBlue);
        }
        // Head pixels
        else if (i < _cycPosTracker + _cycFlash + _cycHead)
        {
            uint8_t redHead = min(255, (_cycBrightness / 5) * ((_cycHead + 1) - (i - (_cycPosTracker + _cycFlash))) / (_cycHead + 1));
            _setColor(j, redHead, redHead / 25, 0);
        }
        // All others pixel are OFF
        else
        {
            _setColor(j, 0, 0, 0);
        }
    }

    // Serial.println("After for loop");
}

void Cyclotron_AF_FE::_setColorAll(uint8_t red, uint8_t green, uint8_t blue)
{
    for (uint16_t i = 0; i < _numLeds; i++)
    {
        _ledState[i][0] = red;
        _ledState[i][1] = green;
        _ledState[i][2] = blue;
    }
}

void Cyclotron_AF_FE::_setColor(uint16_t pixel, uint8_t red, uint8_t green, uint8_t blue)
{
    _ledState[pixel][0] = red;
    _ledState[pixel][1] = green;
    _ledState[pixel][2] = blue;
}
