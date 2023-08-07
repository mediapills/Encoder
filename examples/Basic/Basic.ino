/*!
 *  @file       Basic.ino
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

#include "mpEncoder.h"

using namespace mediapills::hardware;

// Digital Encoder Clock pin number
#define ENCODERS_PAD_A_PIN_NO A0

// Digital Encoder Data pin number
#define ENCODERS_PAD_B_PIN_NO A1

// Digital Encoder Knob pin number
#define ENCODERS_PAD_KNOB_PIN_NO A2

mpEncoderClass encoder(ENCODERS_PAD_A_PIN_NO, ENCODERS_PAD_B_PIN_NO,
                       ENCODERS_PAD_KNOB_PIN_NO);
/*
 * Define encoder handler listeners:
 * - ENCODER_ON_CLICK,
 * - ENCODER_ON_DOUBLE_CLICK,
 * - ENCODER_RELEASED,
 * - ENCODER_CHANGED,
 * - ENCODER_INCREASED,
 * - ENCODER_DECREASED
 */

void onClick() { Serial.println("On button rotary encoder click action"); }

void onDoubleClick() {
  Serial.println("On button rotary encoder double click action");
}

void onReleased() { Serial.println("On button rotary encoder relese action"); }

void onChanged() { Serial.println("On button rotary encoder rotation action"); }

void onIncreased() {
  Serial.println("On button rotary encoder clockwise rotation action");
}

void onDecreased() {
  Serial.println("On button rotary encoder counterclockwise rotation action");
}

void setup() {
  Serial.begin(9600);

  encoder.init();

  // Set callbacks
  encoder.setEventListener(encoder.ENCODER_ON_CLICK, onClick);
  encoder.setEventListener(encoder.ENCODER_ON_DOUBLE_CLICK, onDoubleClick);
  encoder.setEventListener(encoder.ENCODER_RELEASED, onReleased);
  encoder.setEventListener(encoder.ENCODER_CHANGED, onChanged);
  encoder.setEventListener(encoder.ENCODER_INCREASED, onIncreased);
  encoder.setEventListener(encoder.ENCODER_DECREASED, onDecreased);
}

void loop() { encoder.excute(); }
