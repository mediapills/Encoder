/*!
 *  @file       mpEncoder.cpp
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

#include "mpEncoder.h"

#include "Arduino.h"

namespace mediapills {

namespace hardware {

mpEncoderClass::mpEncoderClass(uint8_t pad_pin_a, uint8_t pad_pin_b,
                               uint8_t knob_pin) {
  padAPin = pad_pin_a;
  padBPin = pad_pin_b;
  knobPin = knob_pin;

  onClickCallback = NULL;
  onDoubleClickCallback = NULL;
  onReleasedCallback = NULL;
  onChangedCallback = NULL;
  onIncreasedCallback = NULL;
  onDecreasedCallback = NULL;

  padState = NULL;
}

void mpEncoderClass::init() {
  pinMode(padAPin, INPUT_PULLUP);
  pinMode(padBPin, INPUT_PULLUP);
  pinMode(knobPin, INPUT_PULLUP);

  // reset lastState
  padState = digitalRead(padAPin);
}

void mpEncoderClass::setEventListener(uint8_t mode, void (*callback)()) {
  switch (mode) {
    case CLICK:
      onClickCallback = callback;
      break;
    case DOUBLE_CLICK:
      onDoubleClickCallback = callback;
      break;
    case RELEASED:
      onReleasedCallback = callback;
      break;
    case CHANGED:
      onChangedCallback = callback;
      break;
    case INCREASED:
      onIncreasedCallback = callback;
      break;
    case DECREASED:
      onDecreasedCallback = callback;
      break;
    default:
      return;
  }
}

void mpEncoderClass::dispatchEvent(uint8_t mode) {
  switch (mode) {
    case CLICK:
      if (onClickCallback != NULL) onClickCallback();
      break;
    case DOUBLE_CLICK:
      if (onDoubleClickCallback != NULL) onDoubleClickCallback();
      break;
    case RELEASED:
      if (onReleasedCallback != NULL) onReleasedCallback();
      break;
    case CHANGED:
      if (onChangedCallback != NULL) onChangedCallback();
      break;
    case INCREASED:
      if (onIncreasedCallback != NULL) onIncreasedCallback();

      dispatchEvent(CHANGED);
      break;
    case DECREASED:
      if (onDecreasedCallback != NULL) onDecreasedCallback();

      dispatchEvent(CHANGED);
      break;
    default:
      return;
  }
}

void mpEncoderClass::checkButtonState() {
  // Read the button state
  int btnState = digitalRead(knobPin);

  // If we detect LOW signal, button is pressed
  if (btnState == LOW) {
    // if 50ms have passed since last LOW pulse, it means that the
    // button has been pressed, released and pressed again
    if (millis() - lastLOWPulsetime > 50) {
      pressed = true;
      // TODO(mediapills): add double click handler

      dispatchEvent(CLICK);
    }

    // Remember last button press event
    lastLOWPulsetime = millis();
  } else if (pressed && btnState == HIGH) {
    pressed = false;

    dispatchEvent(RELEASED);
  } else if (pressed) {
    // TODO(mediapills): add long push handler
  }
}

void mpEncoderClass::checkEncoderState() {
  // Store the status of the clock pin (HIGH or LOW)
  int currentPadState = digitalRead(padAPin);

  // If last and current state of Clock are different, then "pulse occurred"
  // React to only 1 state change to avoid double count
  if (currentPadState != padState && currentPadState == 1) {
    // If the Data state is different than the Clock state then
    // the encoder is rotating "CCW" so we decrement
    if (digitalRead(padBPin) != currentPadState) {
      // decreased();
      dispatchEvent(DECREASED);
    } else {
      // increased();
      dispatchEvent(INCREASED);
    }
  }

  // We save last Clock state for next loop
  padState = currentPadState;
}

}  // namespace hardware

}  // namespace mediapills
