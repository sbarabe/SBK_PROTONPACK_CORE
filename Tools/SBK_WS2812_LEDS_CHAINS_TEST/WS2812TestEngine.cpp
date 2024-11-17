/*
 *  WS2812TestEngine.cpp is a part of SBK_WS2812_LEDS_CHAINS_TEST (Version 0) code for testing WS2812 LEDs
 *  chains connections and wiring used in many Ghostbusters props replica.
 *  Copyright (c) 2024 Samuel Barabé
 *
 *  See this page for reference <https://github.com/sbarabe/SBK_PROTONPACK_CORE/tree/main/Tools/SBK_WS2812_LEDS_CHAINS_TEST">.
 *
 *  SBK_WS2812_LEDS_CHAINS_TEST is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Foundation, either version
 *  3 of the License, or (at your option) any later version.
 *
 *  SBK_WS2812_LEDS_CHAINS_TEST is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
 *  the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with Foobar. If not,
 *  see <https://www.gnu.org/licenses/>
 */

#include "WS2812TestEngine.h"

WS2812Test::WS2812Test(Adafruit_NeoPixel &strip, uint8_t start, uint8_t end)
  : _strip(strip), _start(start), _end(end) {
  _prevTime = 0;
  _prevTimeRed = 0;
  _prevTimeGreen = 0;
  _prevTimeBlue = 0;
  _numLeds = (_end - _start + 1);
  _redTracker = 0;
  _greenTracker = 0;
  _blueTracker = 0;
}

void WS2812Test::begin() {
  clear();
}

void WS2812Test::update() {
  for (int8_t i = 0; i < _numLeds; i++) {
    // Offset index to fit ws2812 LEDs strip index
    uint8_t j = i + _start;
    // set segments according to mapping define in setting
    _strip.setPixelColor(j, _redTracker, _greenTracker, _blueTracker);
  }
}

void WS2812Test::clear() {
  _redTracker = 0;
  _greenTracker = 0;
  _blueTracker = 0;
}

void WS2812Test::setColor(uint8_t red, uint8_t green, uint8_t blue) {
  _redTracker = red;
  _greenTracker = green;
  _blueTracker = blue;
}

bool WS2812Test::rampToRed(int16_t ramp_time, bool init) {
  static unsigned long rampStartTime;
  if (init) {
    rampStartTime = millis();
  }
  int16_t redTarget = 255;
  int16_t greenTarget = 0;
  int16_t blueTarget = 0;

  int16_t redIncrement = 5;
  int16_t greenIncrement = 5;
  int16_t blueIncrement = 5;

  _rampColor(&_redTracker, ramp_time, init, &_initRedTracker, redTarget, &_intervalRed, redIncrement, &_prevTimeRed);
  _rampColor(&_greenTracker, ramp_time, init, &_initGreenTracker, greenTarget, &_intervalGreen, greenIncrement, &_prevTimeGreen);
  _rampColor(&_blueTracker, ramp_time, init, &_initBlueTracker, blueTarget, &_intervalBlue, blueIncrement, &_prevTimeBlue);

  if ((_redTracker == redTarget && _greenTracker == greenTarget && _blueTracker == blueTarget) || (millis() - rampStartTime > ramp_time)) {
    return true;
  } else {
    return false;
  }
}

bool WS2812Test::rampToGreen(int16_t ramp_time, bool init) {
  static unsigned long rampStartTime;
  if (init) {
    rampStartTime = millis();
  }

  int16_t redTarget = 0;
  int16_t greenTarget = 255;
  int16_t blueTarget = 0;

  int16_t redIncrement = 5;
  int16_t greenIncrement = 5;
  int16_t blueIncrement = 5;

  _rampColor(&_redTracker, ramp_time, init, &_initRedTracker, redTarget, &_intervalRed, redIncrement, &_prevTimeRed);
  _rampColor(&_greenTracker, ramp_time, init, &_initGreenTracker, greenTarget, &_intervalGreen, greenIncrement, &_prevTimeGreen);
  _rampColor(&_blueTracker, ramp_time, init, &_initBlueTracker, blueTarget, &_intervalBlue, blueIncrement, &_prevTimeBlue);

  if ((_redTracker == redTarget && _greenTracker == greenTarget && _blueTracker == blueTarget) || (millis() - rampStartTime > ramp_time)) {
    return true;
  } else {
    return false;
  }
}

bool WS2812Test::rampToBlue(int16_t ramp_time, bool init) {
  static unsigned long rampStartTime;
  if (init) {
    rampStartTime = millis();
  }
  int16_t redTarget = 0;
  int16_t greenTarget = 0;
  int16_t blueTarget = 255;

  int16_t redIncrement = 5;
  int16_t greenIncrement = 5;
  int16_t blueIncrement = 5;

  _rampColor(&_redTracker, ramp_time, init, &_initRedTracker, redTarget, &_intervalRed, redIncrement, &_prevTimeRed);
  _rampColor(&_greenTracker, ramp_time, init, &_initGreenTracker, greenTarget, &_intervalGreen, greenIncrement, &_prevTimeGreen);
  _rampColor(&_blueTracker, ramp_time, init, &_initBlueTracker, blueTarget, &_intervalBlue, blueIncrement, &_prevTimeBlue);

  if ((_redTracker == redTarget && _greenTracker == greenTarget && _blueTracker == blueTarget) || (millis() - rampStartTime > ramp_time)) {
    return true;
  } else {
    return false;
  }
}

bool WS2812Test::_rampColor(int16_t *colorTracker, int16_t rampTime, bool init, int16_t *initTracker, int16_t tg, int16_t *interval, int16_t increment, unsigned long *prevTime) {
  // Record initial vent color trackers
  if (init) {
    *initTracker = *colorTracker;
    if (tg == *initTracker) {
      *interval = 0;
    } else {
      *interval = rampTime / abs(tg - *initTracker);
    }
  }

  // check if ramp is done
  if (rampTime == 0) {
    *colorTracker = tg;
  }
  if (*colorTracker == tg) {
    return true;
  } else
  // Ramp not done yet
  {

    // Update color tracker
    if (*colorTracker != tg) {
      if (millis() - *prevTime >= *interval) {
        *prevTime = millis();
        *colorTracker = _ramp_parameter(*colorTracker, *initTracker, tg, increment);
      }
    }
    return false;
  }
}

int16_t WS2812Test::_ramp_parameter(int16_t param, int16_t ini, int16_t tg, int16_t incr) {
  // int16_t newParam = param;
  if ((tg - ini) >= 0) {  // Ramp UP
    if (param + incr <= tg) {
      param = param + incr;
    } else {
      param = tg;
    }
  } else {  // Ramp DOWN
    if (incr >= param || param - incr < tg) {
      param = tg;
    } else {
      param = param - incr;
      if (param < 0) {
        param = 0;
      }
    }
  }
  return param;
}

void WS2812Test::test(uint16_t time) {
  static bool rampToRedInit = true;
  static bool rampToRedDone = false;
  static bool rampToGreenInit = false;
  static bool rampToGreenDone = false;
  static bool rampToBlueInit = false;
  static bool rampToBlueDone = false;
  static bool fadeOutInit = false;

  // reset the sequence if init is true
  if (rampToRedInit) {
    rampToRedDone = false;
    rampToGreenDone = false;
    rampToBlueDone = false;
  }

  // Ramp to red phase
  if (!rampToRedDone && !rampToGreenDone && !rampToBlueDone) {
    if (rampToRed(time, rampToRedInit)) {
      rampToRedDone = true;
      rampToGreenInit = true;
    };
    rampToRedInit = false;
  }
  // Ramp to green phase
  else if (rampToRedDone && !rampToGreenDone && !rampToBlueDone) {
    if (rampToGreen(time, rampToGreenInit)) {
      rampToGreenDone = true;
      rampToBlueInit = true;
    };
    rampToGreenInit = false;
  }
  // Ramp to blue phase
  else {
    if (rampToBlue(time, rampToBlueInit)) {
      rampToBlueDone = true;
      rampToRedInit = true;
    };
    rampToBlueInit = false;
  }
}
