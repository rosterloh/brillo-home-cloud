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

#include <string>
#include <sysexits.h>

#include <base/bind.h>
#include <base/command_line.h>
#include <base/macros.h>
#include <binderwrapper/binder_wrapper.h>
#include <brillo/binder_watcher.h>
#include <brillo/daemons/daemon.h>
#include <brillo/syslog_logging.h>

#include "binder_constants.h"
#include "brillo/examples/ledflasher/BnLEDService.h"
#include "ledstatus.h"

using android::String16;

class LEDService : public brillo::examples::ledflasher::BnLEDService {
 public:
  android::binder::Status getLEDCount(int32_t* count) override {
    *count = leds_.GetLedCount();
    return android::binder::Status::ok();
  }

  android::binder::Status setLED(int32_t ledIndex, bool on) override {
    leds_.SetLedStatus(ledIndex, on);
    return android::binder::Status::ok();
  }

  android::binder::Status getLED(int32_t ledIndex, bool* on) override {
    *on = leds_.IsLedOn(ledIndex);
    return android::binder::Status::ok();
  }

  android::binder::Status getAllLEDs(std::vector<bool>* leds) override {
    *leds = leds_.GetStatus();
    return android::binder::Status::ok();
  }

  android::binder::Status getAllLEDNames(std::vector<String16>* leds) override {
    std::vector<std::string> ledNames = leds_.GetNames();
    for (const std::string& name : ledNames) {
      leds->push_back(String16{name.c_str()});
    }
    return android::binder::Status::ok();
  }

  android::binder::Status setAllLEDs(bool on) override {
    leds_.SetAllLeds(on);
    return android::binder::Status::ok();
  }

 private:
  LedStatus leds_;
};

class Daemon final : public brillo::Daemon {
 public:
  Daemon() = default;

 protected:
  int OnInit() override {
    android::BinderWrapper::Create();
    if (!binder_watcher_.Init())
      return EX_OSERR;

    led_service_ = new LEDService();
    android::BinderWrapper::Get()->RegisterService(
        ledservice::kBinderServiceName,
        led_service_);
    return brillo::Daemon::OnInit();
  }

 private:
  brillo::BinderWatcher binder_watcher_;
  android::sp<LEDService> led_service_;

  DISALLOW_COPY_AND_ASSIGN(Daemon);
};

int main(int argc, char* argv[]) {
  base::CommandLine::Init(argc, argv);
  brillo::InitLog(brillo::kLogToSyslog | brillo::kLogHeader);
  Daemon daemon;
  return daemon.Run();
}
