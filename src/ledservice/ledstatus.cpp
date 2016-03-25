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

#include "ledstatus.h"

#include <string>

#include <base/files/file_path.h>
#include <base/format_macros.h>
#include <base/logging.h>
#include <base/strings/stringprintf.h>
#include <base/strings/string_number_conversions.h>
#include <base/strings/string_util.h>
#include <brillo/streams/file_stream.h>
#include <brillo/streams/stream_utils.h>

namespace {

brillo::StreamPtr GetLEDDataStream(size_t index, bool write) {
  std::string led_path;
  if( index == 3 ) {
      led_path = "/sys/class/leds/boot/brightness";
  } else {
      led_path = base::StringPrintf("/sys/class/leds/led%" PRIuS "/brightness",
                                    index + 1);
  }
  base::FilePath dev_path{led_path};
  auto access_mode = brillo::stream_utils::MakeAccessMode(!write, write);
  return brillo::FileStream::Open(
      dev_path, access_mode, brillo::FileStream::Disposition::OPEN_EXISTING,
      nullptr);
}

}  // anonymous namespace

LedStatus::LedStatus() {
  // Try to open the lights HAL.
  int ret = hw_get_module(LIGHTS_HARDWARE_MODULE_ID, &lights_hal_);
  if (ret) {
    LOG(ERROR) << "Failed to load the lights HAL.";
    return;
  }
  CHECK(lights_hal_);
  LOG(INFO) << "Loaded lights HAL.";

  // If we can open the HAL, then we map each number to one of the LEDs
  // available on the board.
  const std::initializer_list<const char*> kLogicalLights = {
    LIGHT_ID_BACKLIGHT, LIGHT_ID_KEYBOARD, LIGHT_ID_BUTTONS, LIGHT_ID_BATTERY,
    LIGHT_ID_NOTIFICATIONS, LIGHT_ID_ATTENTION, LIGHT_ID_BLUETOOTH,
    LIGHT_ID_WIFI};
  for (const char* light_name : kLogicalLights) {
    light_device_t* light_device = nullptr;
    ret = lights_hal_->methods->open(
        lights_hal_, light_name,
        reinterpret_cast<hw_device_t**>(&light_device));
    // If a given light device couldn't be opened, don't map it to a number.
    if (ret || !light_device) {
      continue;
    }
    hal_leds_.push_back(light_name);
    hal_led_status_.push_back(false);
  }

  // If the size of the map is zero, then the lights HAL doesn't have any valid
  // leds.
  if (hal_leds_.empty()) {
    LOG(INFO) << "Unable to open any light devices using the HAL.";
    lights_hal_ = nullptr;
    return;
  }
}

size_t LedStatus::GetLedCount() const {
  return lights_hal_ ? hal_leds_.size() : 1;
}

std::vector<bool> LedStatus::GetStatus() const {
  if (lights_hal_)
    return hal_led_status_;

  std::vector<bool> leds(GetLedCount());
  for (size_t index = 0; index < GetLedCount(); index++)
    leds[index] = IsLedOn(index);
  return leds;
}

std::vector<std::string> LedStatus::GetNames() const {
  return lights_hal_ ? hal_leds_ : std::vector<std::string>(GetLedCount());
}

bool LedStatus::IsLedOn(size_t index) const {
  CHECK(index < GetLedCount());
  if (lights_hal_)
    return hal_led_status_[index];

  brillo::StreamPtr stream = GetLEDDataStream(index, false);
  if (!stream)
    return false;

  char buffer[10];
  size_t size_read = 0;
  if (!stream->ReadNonBlocking(buffer, sizeof(buffer), &size_read, nullptr,
                               nullptr)) {
    return false;
  }

  std::string value{buffer, size_read};
  base::TrimWhitespaceASCII(value, base::TrimPositions::TRIM_ALL, &value);
  int brightness = 0;
  if (!base::StringToInt(value, &brightness))
    return false;
  return brightness > 0;
}

void LedStatus::SetLedStatus(size_t index, bool on) {
  CHECK(index < GetLedCount());
  if (lights_hal_) {
    light_state_t state = {};
    state.color = on;
    state.flashMode = LIGHT_FLASH_NONE;
    state.flashOnMS = 0;
    state.flashOffMS = 0;
    state.brightnessMode = BRIGHTNESS_MODE_USER;
    light_device_t* light_device = nullptr;
    int rc = lights_hal_->methods->open(
        lights_hal_,
        hal_leds_[index].c_str(),
        reinterpret_cast<hw_device_t**>(&light_device));
    if (rc) {
      LOG(ERROR) << "Unable to open " << hal_leds_[index];
      return;
    }
    CHECK(light_device);
    rc = light_device->set_light(light_device, &state);
    if (rc) {
      LOG(ERROR) << "Unable to set " << hal_leds_[index];
      return;
    }
    hal_led_status_[index] = on;
    light_device->common.close(
        reinterpret_cast<hw_device_t*>(light_device));
    if (rc) {
      LOG(ERROR) << "Unable to close " << hal_leds_[index];
      return;
    }
    return;
  }

  brillo::StreamPtr stream = GetLEDDataStream(index, true);
  if (!stream)
    return;

  std::string brightness = on ? "255" : "0";
  stream->WriteAllBlocking(brightness.data(), brightness.size(), nullptr);
}

void LedStatus::SetAllLeds(bool on) {
  for (size_t i = 0; i < GetLedCount(); i++)
    SetLedStatus(i, on);
}
