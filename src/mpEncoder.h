/*!
 *  @file       mpEncoder.h
 *  Project     mediapills::hardware
 *  @brief      Push Button Rotary Encoder handlers for Arduino platform.
 *  @version    0.0.1
 *  @author     Andrew Yatskovets
 *  @date       16/07/2023
 *
 * Copyright (c) 2023 MediaPills
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef _MP_ENCODER_
#define _MP_ENCODER_

#include "Arduino.h"

namespace mediapills {

namespace hardware {

class mpEncoderClass {
 private:
  uint8_t padAPin = NULL;
  uint8_t padBPin = NULL;
  uint8_t knobPin = NULL;

  // Use this variable to store encoder button state
  bool pressed = false;
  // Use this to store if the push button was pressed or not
  unsigned long lastLOWPulsetime = 0;

  // Store the PREVIOUS status of the clock pin (HIGH or LOW)
  int padState;

  void (*onClickCallback)();
  void (*onDoubleClickCallback)();
  void (*onReleasedCallback)();
  void (*onChangedCallback)();
  void (*onIncreasedCallback)();  //  Clockwise rotation
  void (*onDecreasedCallback)();  //  Counterclockwise rotation

  void dispatchEvent(uint8_t mode);

  void checkButtonState();
  void checkEncoderState();

 public:
  enum { CLICK, DOUBLE_CLICK, RELEASED, CHANGED, INCREASED, DECREASED };

  // constructor
  mpEncoderClass(uint8_t pad_pin_a, uint8_t pad_pin_b, uint8_t knob_pin);
  void init();
  void handler() {
    checkButtonState();
    checkEncoderState();
  }

  void setEventListener(byte mode, void (*callback)());
};

}  // namespace hardware

}  // namespace mediapills

#endif
