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

#ifndef LEDFLASHER_SRC_LEDFLASHER_ANIMATION_BLINK_H_
#define LEDFLASHER_SRC_LEDFLASHER_ANIMATION_BLINK_H_

#include "animation.h"

class AnimationBlink : public Animation {
 public:
  AnimationBlink(
      android::sp<brillo::examples::ledflasher::ILEDService> led_service,
      const base::TimeDelta& duration);

 protected:
  void DoAnimationStep() override;

 private:
  size_t on_{true};
};

#endif  // LEDFLASHER_SRC_LEDFLASHER_ANIMATION_BLINK_H_
