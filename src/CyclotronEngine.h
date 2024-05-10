/*
 *  CyclotronEngine.h is a part of SBK_PROTONPACK_CORE (VERSION 2.1) code for animations of a Proton Pack replica
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

#ifndef CYCLOTRONENGINE_H
#define CYCLOTRONENGINE_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class Cyclotron_GB1_GB2
{
public:
    Cyclotron_GB1_GB2(Adafruit_NeoPixel &strip, bool direction, uint8_t start1, uint8_t end1, uint8_t start2, uint8_t end2, uint8_t start3, uint8_t end3, uint8_t start4, uint8_t end4);
    ~Cyclotron_GB1_GB2();
    void begin();
    void clear();
    void update();
    void rampToPoweredDown(uint16_t ramp_time, bool init);
    void rampToIdleOne(uint16_t ramp_time, bool init);
    void rampToIdleTwo(uint16_t ramp_time, bool init);
    void rampToFiring(uint16_t ramp_time, bool init);
    void setDirection(bool direction);

private:
    void _setColorAll(uint8_t red, uint8_t green, uint8_t blue);
    void _setColor(uint16_t pixel, uint8_t red, uint8_t green, uint8_t blue);
    void _cell1();
    void _cell2();
    void _cell3();
    void _cell4();
    void _rotation();
    uint8_t _cellIntensity(bool state, int16_t tracker, uint8_t start, uint8_t end);
    void _cellClear(uint8_t start, uint8_t end);
    void _rampCyc(int16_t rampTime, bool init, int16_t tg_updateSp, int16_t tg_fadeSp, int16_t tg_maxBri, int16_t tg_flashDur, int16_t tg_offset);
    void _idleCyc(int16_t updateSp, int16_t fadeSp, int16_t maxBri, int16_t flashDur, int16_t offset);
    uint16_t _ramp_parameter(int16_t param, int16_t ini, int16_t tg, int16_t incr);
    Adafruit_NeoPixel &_strip;
    bool _direction;
    unsigned long _prevTime;
    uint8_t _start1;
    uint8_t _end1;
    uint8_t _start2;
    uint8_t _end2;
    uint8_t _start3;
    uint8_t _end3;
    uint8_t _start4;
    uint8_t _end4;
    uint8_t _numLeds;
    uint8_t **_ledState;
    int16_t _cycUpdateSp;
    int16_t _cycFadeSp;
    int16_t _cycBrightness;
    int16_t _cycFlashDuration;
    int16_t _cycPosOffset;
    int16_t _cycPosDuration;
    uint16_t _cell1Tracker;
    uint16_t _cell2Tracker;
    uint16_t _cell3Tracker;
    uint16_t _cell4Tracker;
    bool _cell1State;
    bool _cell2State;
    bool _cell3State;
    bool _cell4State;
    unsigned long _prevUpdateSpTime;
    unsigned long _prevFadeTime;
    unsigned long _prevBrightnessTime;
    unsigned long _prevFlasDurTime;
    unsigned long _prevOffsetTime;
};

class Cyclotron_AF_FE
{
public:
    Cyclotron_AF_FE(Adafruit_NeoPixel &strip, bool direction, uint8_t start, uint8_t end);
    ~Cyclotron_AF_FE();
    void begin();
    void clear();
    void update();
    void rampToPoweredDown(uint16_t ramp_time, bool init);
    void rampToIdleOne(uint16_t ramp_time, bool init);
    void rampToIdleTwo(uint16_t ramp_time, bool init);
    void rampToFiring(uint16_t ramp_time, bool init);
    void setDirection(bool direction);

private:
    void _setColorAll(uint8_t red, uint8_t green, uint8_t blue);
    void _setColor(uint16_t pixel, uint8_t red, uint8_t green, uint8_t blue);
    void _rotation();
    void _ramp(uint16_t rampTime, bool init, int16_t tg_updateSp);
    int16_t _ramp_parameter(int16_t param, int16_t ini, int16_t tg, int16_t incr);
    void _idle(uint16_t updateSp);

    Adafruit_NeoPixel &_strip;
    bool _direction;
    unsigned long _prevTime;
    uint8_t _start;
    uint8_t _end;
    uint8_t _numLeds;
    uint8_t **_ledState;
    uint16_t _cycUpdateSp;
    int16_t _cycBrightness;
    int16_t _cycTrail;
    int16_t _cycFlash;
    int16_t _cycHead;
    int8_t _cycPosTracker;
    unsigned long _prevUpdateSpTime;
    unsigned long _prevFadeTime;
    unsigned long _prevBrightnessTime;
    unsigned long _prevFlasDurTime;
    unsigned long _prevOffsetTime;
};

#endif
