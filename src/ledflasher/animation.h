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

#ifndef LEDFLASHER_SRC_LEDFLASHER_ANIMATION_H_
#define LEDFLASHER_SRC_LEDFLASHER_ANIMATION_H_

#include <vector>
#include <memory>

#include <base/time/time.h>
#include <base/memory/weak_ptr.h>

#include "brillo/examples/ledflasher/ILEDService.h"

class Animation {
 public:
  Animation(android::sp<brillo::examples::ledflasher::ILEDService> led_service,
            const base::TimeDelta& step_duration);
  virtual ~Animation();

  void Start();
  void Stop();

  static std::unique_ptr<Animation> Create(
      android::sp<brillo::examples::ledflasher::ILEDService> led_service,
      const std::string& type,
      const base::TimeDelta& duration);


 protected:
  size_t num_leds;
  virtual void DoAnimationStep() = 0;

  bool GetLED(size_t index) const;
  void SetLED(size_t index, bool on);
  void SetAllLEDs(bool on);

 private:
  android::sp<brillo::examples::ledflasher::ILEDService> led_service_;
  base::TimeDelta step_duration_;

  base::WeakPtrFactory<Animation> weak_ptr_factory_{this};
  DISALLOW_COPY_AND_ASSIGN(Animation);
};

#endif  // LEDFLASHER_SRC_LEDFLASHER_ANIMATION_H_
