LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := mp3-player-service
LOCAL_INIT_RC := mp3-player-service.rc
LOCAL_REQUIRED_MODULES := mediaplayer.json

LOCAL_CFLAGS := -Wall -Werror -Wno-unused-parameter

LOCAL_SRC_FILES :=	\
	mp3-player-service.cpp	\

LOCAL_SHARED_LIBRARIES := \
	libbinder \
	libbinderwrapper \
	libbrillo \
	libbrillo-binder \
	libbrillo-stream \
	libchrome \
	libhardware \
	libmedia \
	libstagefright \
	libstagefright_foundation \
	libutils \

LOCAL_STATIC_LIBRARIES := \
	libmp3-player-service \

LOCAL_C_INCLUDES := \
	$(TOP)/device/generic/brillo/pts/audio/common \
	$(TOP)/frameworks/av/media/libstagefright \
	$(TOP)/frameworks/native/include/media/openmax \
	$(TOP)/system/media/audio_utils/include

include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := mediaplayer.json
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_ETC)/weaved/traits
LOCAL_SRC_FILES := etc/weaved/traits/$(LOCAL_MODULE)

include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := libmp3-player-service
LOCAL_AIDL_INCLUDES := $(LOCAL_PATH)/aidl
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)

LOCAL_SRC_FILES := \
	aidl/brillo/demo/IMp3PlayerService.aidl \
	binder_constants.cpp \

include $(BUILD_STATIC_LIBRARY)
