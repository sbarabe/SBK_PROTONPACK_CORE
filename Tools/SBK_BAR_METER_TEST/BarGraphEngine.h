/*
 *  BarGraphEngine.h is a part of SBK_BAR_METER_TEST (VERSION 0) code for testing the bar meter
 *  connections, mapping and communication used in many Ghostbusters props replica.
 *  Copyright (c) 2024 Samuel Barabé
 *
 *  See this page for reference <https://github.com/sbarabe/SBK_PROTONPACK_CORE/tree/main/Tools/SBK_BAR_METER_TEST>.
 *
 *  SBK_BAR_METER_TEST is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Foundation, either version
 *  3 of the License, or (at your option) any later version.
 *
 *  SBK_BAR_METER_TEST is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
 *  the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with Foobar. If not,
 *  see <https://www.gnu.org/licenses/>
 */

#ifndef BARGRAPHENGINE_H
#define BARGRAPHENGINE_H

#include <Arduino.h>
#include <LedControl.h>
#include <SPI.h>
#include "SBK_HT16K33.h"

class BarGraphAnimation {
public:
  // Accessing lEDs states
  BarGraphAnimation(uint8_t numLeds);
  bool getLedState(uint8_t index);
  void clear();
  void setHigh();
  void setLow();
  void testAnimation(uint16_t speed);


private:
  unsigned long _prevTime;
  uint8_t _numLeds;
  bool _ledState[28];
  int8_t _runningLedTracker;
  bool _reverseSeqTracker;
};


class HT16K33Driver : public BarGraphAnimation {
public:
  HT16K33Driver(uint8_t numLeds, bool direction, uint8_t dataPin, uint8_t clockPin, uint8_t address);
  void begin(const uint8_t segMap[][2], uint8_t rows, uint8_t cols);
  void update();

private:
  void _setLed(uint8_t ledNum, bool state);
  void _setLeds(uint32_t ledsState);
  uint8_t _numLeds;
  bool _direction;
  uint8_t _clockPin;
  uint8_t _dataPin;
  uint8_t _address;
  HT16K33 _driver;
  uint8_t _segMap[28][2];
};


class MAX72xxDriver : public BarGraphAnimation {
public:
  MAX72xxDriver(uint8_t numLeds, bool direction, uint8_t dataPin, uint8_t clockPin, uint8_t loadPin);
  void begin(const uint8_t segMap[][2], uint8_t rows, uint8_t cols);
  void update();

private:
  void _setLed(uint8_t ledNum, bool state);
  void _setLeds(uint32_t ledsState);
  uint8_t _numLeds;
  bool _direction;
  uint8_t _loadPin;
  uint8_t _clockPin;
  uint8_t _dataPin;
  LedControl _driver;
  uint8_t _segMap[28][2];
};

#endif
