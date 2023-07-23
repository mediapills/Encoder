/*!
 *  @file       mpEncoder.cpp
 *  Project     mediapills::hardware
 *  @brief      Push Button Rotary Encoder handlers for Arduino platform.
 *  @version    0.0.1
 *  @author     Andrew Yatskovets
 *  @date       16/07/2023
 *  @license    MIT - (c) 2023 - Mediapills
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

#include "Arduino.h"
#include "mpEncoder.h"

namespace mediapills { namespace hardware {

mpEncoderClass::mpEncoderClass(uint8_t pad_pin_a, uint8_t pad_pin_b, uint8_t knob_pin) {
  padAPin = pad_pin_a;
  padBPin = pad_pin_b;
  knobPin = knob_pin;
}

void mpEncoderClass::init() {
  pinMode(padAPin, INPUT_PULLUP);
  pinMode(padBPin, INPUT_PULLUP);
  pinMode(knobPin, INPUT_PULLUP);

  // reset lastState
  padState = digitalRead(padAPin);
}

void mpEncoderClass::setEventListener(uint8_t mode, void (*callback)()) {
  switch(mode) {
    case ENCODER_ON_CLICK:
      onClickCallback = callback;
      break;
    case ENCODER_ON_DOUBLE_CLICK:
      onDoubleClickCallback = callback;
      break;
    case ENCODER_RELEASED:
      releasedCallback = callback;
      break;
    case ENCODER_CHANGED:
      changedCallback = callback;
      break;
    case ENCODER_INCREASED:
      increasedCallback = callback;
      break;
    case ENCODER_DECREASED:
      decreasedCallback = callback;
      break;
    default:
      return;
  }
}

void mpEncoderClass::dispatchEvent(uint8_t mode) {
  switch(mode) {
    case ENCODER_ON_CLICK:
      if ( onClickCallback != NULL )
        onClickCallback();
      break;
    case ENCODER_ON_DOUBLE_CLICK:
      if ( onDoubleClickCallback != NULL )
        onDoubleClickCallback();
      break;
    case ENCODER_RELEASED:
      if ( releasedCallback != NULL )
        releasedCallback();
      break;
    case ENCODER_CHANGED:
      if ( changedCallback != NULL )
        changedCallback();
      break;
    case ENCODER_INCREASED:
      if ( increasedCallback != NULL )
        increasedCallback();

      dispatchEvent(ENCODER_CHANGED);
      break;
    case ENCODER_DECREASED:
      if ( decreasedCallback != NULL )
        decreasedCallback();

      dispatchEvent(ENCODER_CHANGED);
      break;
    default:
      return;
  }
}

void mpEncoderClass::checkButtonState() {
  // Read the button state
  int btnState = digitalRead(knobPin);

  //If we detect LOW signal, button is pressed
  if (btnState == LOW) {
    //if 50ms have passed since last LOW pulse, it means that the
    //button has been pressed, released and pressed again
    if (millis() - lastLOWPulsetime > 50) {
      pressed = true;
      // TODO: add double click handler

      dispatchEvent(ENCODER_ON_CLICK);
    }
    
    // Remember last button press event
    lastLOWPulsetime = millis();
  } else if (pressed && btnState == HIGH) {
    pressed = false;

    dispatchEvent(ENCODER_RELEASED);
  }  else if (pressed) {
    // TODO: add long push handler
  }
}

void mpEncoderClass::checkEncoderState() {
  //Store the status of the clock pin (HIGH or LOW)
  int currentPadState = digitalRead(padAPin);

  // If last and current state of Clock are different, then "pulse occurred"
  // React to only 1 state change to avoid double count
  if (currentPadState != padState  && currentPadState == 1) {
    // If the Data state is different than the Clock state then
    // the encoder is rotating "CCW" so we decrement
    if (digitalRead(padBPin) != currentPadState) {
      // decreased();
      dispatchEvent(ENCODER_DECREASED);
    } else {
      // increased();
      dispatchEvent(ENCODER_INCREASED);
    }
  }

  // We save last Clock state for next loop
  padState = currentPadState; 
}

void mpEncoderClass::excute() {
  checkButtonState();
  checkEncoderState();
}

}} // end namespace mediapills::hardware
