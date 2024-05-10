/*
 *  PlayerEngine.cpp is a part of SBK_PROTONPACK_CORE (VERSION 2.1) code for sound effects and animations of a Proton Pack replica
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

#include "PlayerEngine.h"

/////////////////////////////////////////////////////
/*                                                 */
/************* DFPlayer Mini section ***************/
/*       (with DFPlayerMini_Fast.h library)       */
/////////////////////////////////////////////////////

Player_DFPlayerMini_Fast::Player_DFPlayerMini_Fast(const uint8_t max, uint8_t volume, uint8_t RX_pin, uint8_t TX_pin, uint8_t pot_pin, bool vol_pot_exist,const uint8_t commandDelay)
    : _VOLUME_MAX(min(30, max)), _volume(volume), _RX_pin(RX_pin), _TX_pin(TX_pin), _pot_pin(pot_pin), _volPotActive(vol_pot_exist), _COMMAND_DELAY(commandDelay)
{
    _startTime = 0;
    _startTimePrev = 0;
    _prevVolume = _volume;
    playing = false;
    _TrackDuration = 0;
}

bool Player_DFPlayerMini_Fast::begin(Stream &s)
{
    if (_player.begin(s, false, 50))
    {
        delay(_COMMAND_DELAY);
        setVol(_volume);
        delay(_COMMAND_DELAY);
        _player.playbackSource(2);
        delay(_COMMAND_DELAY);
        _player.EQSelect(1);
        delay(_COMMAND_DELAY);
        _player.stop();
        delay(_COMMAND_DELAY);
        _player.startDAC();
        delay(_COMMAND_DELAY);
        _player.stopRepeat();
        delay(_COMMAND_DELAY);
        return true;
    }
    else
    {
        return false;
    }
}

void Player_DFPlayerMini_Fast::defineVolumePot(uint8_t pin, bool active)
{
    _pot_pin = pin;
    pinMode(_pot_pin, INPUT);
    _volPotActive = active;
}

uint8_t Player_DFPlayerMini_Fast::setVolWithPot()
{
    if (_volPotActive)
    {
        long potValue;
        uint8_t newVolume = _volume;
        static unsigned long prevTime = 0;
        if (millis() - prevTime >= 250)
        {
            prevTime = millis();
            if (_volPotActive)
            {
                potValue = analogRead(_pot_pin);
                newVolume = (uint8_t)map(potValue, 10, 1000, 0, _VOLUME_MAX);
            }
        }
        if (newVolume != _volume)
        {
            _volume = newVolume;
            _player.volume(newVolume);
        }
    }
    return _volume;
}

bool Player_DFPlayerMini_Fast::isPlaying()
{
    if ((millis() - _startTime) < _TrackDuration)
    {
        if (!playing)
        {
            playing = true;
            // Serial.println("playing start !");
        }
    }
    else
    {
        if (playing)
        {
            playing = false;
            // Serial.println("playing stop !");
        }
    }
    return playing;
}

void Player_DFPlayerMini_Fast::setThemesPlaymode()
{
    _player.repeatFolder(1);
}

void Player_DFPlayerMini_Fast::setSinglePlaymode()
{
    // no function available for this in the library
}

void Player_DFPlayerMini_Fast::setCyclingTrackPlaymode()
{
    // no function available for this in the library
}

void Player_DFPlayerMini_Fast::loopFileNum(int16_t track_num)
{
    _player.loop(track_num);
    _TrackDuration = 0;
    //_startTime = millis();
    //_TrackDuration = track_length;
    /*
    Serial.print("before = "), Serial.print(_before);
    Serial.print("   after = "), Serial.println(_startTime);
    */
}

void Player_DFPlayerMini_Fast::playFileNum(int16_t track_num, uint16_t track_length)
{
    _player.play(track_num);
    _startTime = millis();
    _TrackDuration = track_length;
    /*
    Serial.print("before = "), Serial.print(_before);
    Serial.print("   after = "), Serial.println(_startTime);
    */
}

void Player_DFPlayerMini_Fast::stop()
{
    _player.stop();
}

void Player_DFPlayerMini_Fast::pause()
{
    _player.pause();
}

void Player_DFPlayerMini_Fast::next()
{
    _player.playNext();
}

void Player_DFPlayerMini_Fast::previous()
{
    _player.playPrevious();
}

void Player_DFPlayerMini_Fast::setVol(uint8_t volume)
{
    _volume = volume;
    constrain(_volume, 0, _VOLUME_MAX);
    _player.volume(_volume);
}

/////////////////////////////////////////////////////
/*                                                 */
/************* DFPlayer Mini section ***************/
/*             (with DFRobot library)              */
/////////////////////////////////////////////////////

Player_DFPlayerMini::Player_DFPlayerMini(const uint8_t max, uint8_t volume, uint8_t RX_pin, uint8_t TX_pin, uint8_t pot_pin, bool vol_pot_exist,const uint8_t commandDelay)
    : _VOLUME_MAX(min(30, max)), _volume(volume), _RX_pin(RX_pin), _TX_pin(TX_pin), _pot_pin(pot_pin), _volPotActive(vol_pot_exist), _COMMAND_DELAY(commandDelay)
{
    _startTime = 0;
    _startTimePrev = 0;
    _prevVolume = _volume;
    playing = false;
    _TrackDuration = 0;
}

bool Player_DFPlayerMini::begin(Stream &s)
{
    if (_player.begin(s, false, true))
    {
        delay(_COMMAND_DELAY);
        _player.outputDevice(DFPLAYER_DEVICE_SD);
        delay(_COMMAND_DELAY);
        _player.EQ(DFPLAYER_EQ_POP);
        delay(_COMMAND_DELAY);
        _player.setTimeOut(50);
        delay(_COMMAND_DELAY);
        _player.enableDAC();
        delay(_COMMAND_DELAY);
        _player.disableLoop();
        delay(_COMMAND_DELAY);
        setVol(_volume);
        return true;
    }
    else
    {
        return false;
    }
}

void Player_DFPlayerMini::defineVolumePot(uint8_t pin, bool active)
{
    _pot_pin = pin;
    pinMode(_pot_pin, INPUT);
    _volPotActive = active;
}

uint8_t Player_DFPlayerMini::setVolWithPot()
{
    if (_volPotActive)
    {
        long potValue;
        uint8_t newVolume = _volume;
        static unsigned long prevTime = 0;
        if (millis() - prevTime >= 250)
        {
            prevTime = millis();
            if (_volPotActive)
            {
                potValue = analogRead(_pot_pin);
                newVolume = (uint8_t)map(potValue, 10, 1000, 0, _VOLUME_MAX);
            }
        }
        if (newVolume != _volume)
        {
            _volume = newVolume;
            _player.volume(newVolume);
        }
    }
    return _volume;
}

bool Player_DFPlayerMini::isPlaying()
{
    if ((millis() - _startTime) < _TrackDuration)
    {
        if (!playing)
        {
            playing = true;
            // Serial.println("playing start !");
        }
    }
    else
    {
        if (playing)
        {
            playing = false;
            // Serial.println("playing stop !");
        }
    }
    return playing;
}

void Player_DFPlayerMini::setThemesPlaymode()
{
    _player.loopFolder(1);
    _TrackDuration = 0;
}

void Player_DFPlayerMini::setSinglePlaymode()
{
    _player.disableLoop();
}

void Player_DFPlayerMini::setCyclingTrackPlaymode()
{
    _player.enableLoop();
}

void Player_DFPlayerMini::loopFileNum(int16_t track_num)
{
    _player.loop(track_num);
    //_startTime = millis();
    //_TrackDuration = track_length;
    /*
    Serial.print("before = "), Serial.print(_before);
    Serial.print("   after = "), Serial.println(_startTime);
    */
}

void Player_DFPlayerMini::playFileNum(int16_t track_num, uint16_t track_length)
{
    _player.play(track_num);
    _startTime = millis();
    _TrackDuration = track_length;
    /*
    Serial.print("before = "), Serial.print(_before);
    Serial.print("   after = "), Serial.println(_startTime);
    */
}

void Player_DFPlayerMini::stop()
{
    _player.stop();
}

void Player_DFPlayerMini::pause()
{
    _player.pause();
}

void Player_DFPlayerMini::next()
{
    _player.next();
}

void Player_DFPlayerMini::previous()
{
    _player.previous();
}

void Player_DFPlayerMini::setVol(uint8_t volume)
{
    _volume = volume;
    constrain(_volume, 0, _VOLUME_MAX);
    _player.volume(_volume);
}
