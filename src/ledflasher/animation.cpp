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

#include "animation.h"
#include "animation_blink.h"
#include "animation_marquee.h"

#include <base/bind.h>
#include <base/message_loop/message_loop.h>

Animation::Animation(
    android::sp<brillo::examples::ledflasher::ILEDService> led_service,
    const base::TimeDelta& step_duration)
  : led_service_{led_service}, step_duration_{step_duration} {
  int led_count;
  led_service_->getLEDCount(&led_count);
  num_leds = static_cast<size_t>(led_count);
  step_duration_ /= num_leds;
}

Animation::~Animation() {
  SetAllLEDs(false);
}

void Animation::Start() {
  DoAnimationStep();
  base::MessageLoop::current()->PostDelayedTask(
      FROM_HERE,
      base::Bind(&Animation::Start, weak_ptr_factory_.GetWeakPtr()),
      step_duration_);
}

void Animation::Stop() {
  weak_ptr_factory_.InvalidateWeakPtrs();
}

bool Animation::GetLED(size_t index) const {
  bool on = false;
  led_service_->getLED(index, &on);
  return on;
}

void Animation::SetLED(size_t index, bool on) {
  led_service_->setLED(index, on);
}

void Animation::SetAllLEDs(bool on) {
  for (size_t i = 0; i < num_leds; i++)
    SetLED(i, on);
}

std::unique_ptr<Animation> Animation::Create(
    android::sp<brillo::examples::ledflasher::ILEDService> led_service,
    const std::string& type,
    const base::TimeDelta& duration) {
  std::unique_ptr<Animation> animation;
  if (type == "blink") {
    animation.reset(new AnimationBlink{led_service, duration});
  } else if (type == "marquee_left") {
    animation.reset(new AnimationMarquee{led_service, duration,
                                         AnimationMarquee::Direction::Left});
  } else if (type == "marquee_right") {
    animation.reset(new AnimationMarquee{led_service, duration,
                                         AnimationMarquee::Direction::Right});
  }
  return animation;
}
