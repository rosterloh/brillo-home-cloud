LOCAL_PATH := $(call my-dir)

# Brillo service
include $(CLEAR_VARS)
LOCAL_MODULE := home_cloud_service
ifdef BRILLO
LOCAL_MODULE_TAGS := eng
endif
LOCAL_SRC_FILES := home_cloud_service.cpp
LOCAL_CFLAGS := -Wno-unused-parameter
LOCAL_SHARED_LIBRARIES := libsensor
include $(BUILD_EXECUTABLE)


# TEST APP
include $(CLEAR_VARS)
LOCAL_C_INCLUDES:= external/tinyalsa/include \
  $(TOP)/frameworks/av/media/libstagefright \
  $(TOP)/frameworks/native/include/media/openmax \
  $(TOP)/device/generic/brillo/pts/audio/common
LOCAL_SRC_FILES:= \
    include/peripherals/gpio/gpio.cpp \
    hc_test.cpp
LOCAL_MODULE := home_cloud_test
LOCAL_SHARED_LIBRARIES:= libcutils libutils libtinyalsa libbrillo libbase \
    libstagefright libstagefright_foundation libutils libbase libbinder \
    libmedia
LOCAL_MODULE_TAGS := optional
include $(BUILD_EXECUTABLE)
