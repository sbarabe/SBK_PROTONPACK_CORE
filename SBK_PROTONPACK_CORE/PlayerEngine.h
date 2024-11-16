/*
 *  PlayerEngine.h is a part of SBK_PROTONPACK_CORE (VERSION 2.4) code for sound effects and animations of a Proton Pack replica
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

#ifndef PLAYERENGINE_H
#define PLAYERENGINE_H

#include <Arduino.h>
#include <DFRobot_DF1201S.h> //for the DFPlayer Pro
#include <DFRobotDFPlayerMini.h>
#include <DFPlayerMini_Fast.h>

class Player_DFPlayerMini_Fast
{
public:
    Player_DFPlayerMini_Fast(const uint8_t max, uint8_t volume, uint8_t RX_pin, uint8_t TX_pin, uint8_t pot_pin, bool vol_pot_exist, const uint8_t commandDelay);
    bool begin(Stream &s);
    bool isPlaying();
    void setThemesPlaymode();
    void setSinglePlaymode();
    void setCyclingTrackPlaymode();
    void loopFileNum(int16_t track_num);
    void playFileNum(int16_t track_num);
    void playFileNum(int16_t track_num, uint16_t track_length);
    void stop();
    void pause();
    void next();
    void previous();
    void setVol(uint8_t volume);
    void defineVolumePot(uint8_t pin, bool active);
        uint8_t setVolWithPotatStart();
    uint8_t setVolWithPot();
    bool playing;

private:
    DFPlayerMini_Fast _player;
    unsigned long _startTime;
    unsigned long _startTimePrev;
    const uint8_t _VOLUME_MAX;
    const uint8_t _COMMAND_DELAY;
    uint8_t _volume;
    uint8_t _prevVolume;
    uint8_t _RX_pin;
    uint8_t _TX_pin;
    uint8_t _pot_pin;
    bool _volPotActive;
    unsigned long _TrackDuration;
    uint8_t _AUDIO_ADVANCE;
    uint8_t _gain;
};

class Player_DFPlayerMini
{
public:
    Player_DFPlayerMini(const uint8_t max, uint8_t volume, uint8_t RX_pin, uint8_t TX_pin, uint8_t pot_pin, bool vol_pot_exist, const uint8_t commandDelay);
    bool begin(Stream &s);
    bool isPlaying();
    void setThemesPlaymode();
    void setSinglePlaymode();
    void setCyclingTrackPlaymode();
    void loopFileNum(int16_t track_num);
    void playFileNum(int16_t track_num);
    void playFileNum(int16_t track_num, uint16_t track_length);
    void stop();
    void pause();
    void next();
    void previous();
    void setVol(uint8_t volume);
    void defineVolumePot(uint8_t pin, bool active);
        uint8_t setVolWithPotatStart();
    uint8_t setVolWithPot();
    bool playing;

private:
    DFRobotDFPlayerMini _player;
    unsigned long _startTime;
    unsigned long _startTimePrev;
    const uint8_t _VOLUME_MAX;
    const uint8_t _COMMAND_DELAY;
    uint8_t _volume;
    uint8_t _prevVolume;
    uint8_t _RX_pin;
    uint8_t _TX_pin;
    uint8_t _pot_pin;
    bool _volPotActive;
    unsigned long _TrackDuration;
    uint8_t _AUDIO_ADVANCE;
};

#endif