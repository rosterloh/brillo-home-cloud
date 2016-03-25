# Copyright 2015 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := ledflasher
LOCAL_INIT_RC := ledflasher.rc
LOCAL_REQUIRED_MODULES := ledflasher.json

LOCAL_SRC_FILES := \
	animation.cpp \
	animation_blink.cpp \
	animation_marquee.cpp \
	ledflasher.cpp \

LOCAL_SHARED_LIBRARIES := \
	libbinder \
	libbinderwrapper \
	libbrillo \
	libbrillo-binder \
	libbrillo-stream \
	libchrome \
	libutils \
	libweaved \

LOCAL_STATIC_LIBRARIES := \
	libledservice-common \

LOCAL_C_INCLUDES := external/gtest/include
LOCAL_CFLAGS := -Wall -Werror
LOCAL_CLANG := true

include $(BUILD_EXECUTABLE)

# Weave schema files
# ========================================================
include $(CLEAR_VARS)
LOCAL_MODULE := ledflasher.json
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_ETC)/weaved/traits
LOCAL_SRC_FILES := etc/weaved/traits/$(LOCAL_MODULE)
include $(BUILD_PREBUILT)
