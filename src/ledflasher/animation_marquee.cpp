/*
 * Copyright 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "animation_marquee.h"

AnimationMarquee::AnimationMarquee(
    android::sp<brillo::examples::ledflasher::ILEDService> led_service,
    const base::TimeDelta& duration,
    Direction direction)
    : Animation{led_service, duration}, direction_{direction} {}

void AnimationMarquee::DoAnimationStep() {
  SetAllLEDs(false);
  SetLED(current_led_, true);
  if (direction_ == Direction::Right) {
    if (current_led_ == 0)
      current_led_ = num_leds;
    --current_led_;
  } else {
    current_led_++;
    if (current_led_ == num_leds)
      current_led_ = 0;
  }
}
