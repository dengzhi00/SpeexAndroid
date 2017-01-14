#LOCAL_PATH := $(call my-dir)
#
#include $(CLEAR_VARS)
#
#LOCAL_MODULE    := SpeexAndroid
#LOCAL_SRC_FILES := SpeexAndroid.cpp
#
#include $(BUILD_SHARED_LIBRARY)


LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include 
$(info  $(LOCAL_PATH))
LOCAL_CFLAGS = -DFIXED_POINT -DUSE_KISS_FFT -DEXPORT="" -UHAVE_CONFIG_H 

LOCAL_SRC_FILES := SpeexAndroid.cpp \
$(subst $(LOCAL_PATH)/,, \
$(wildcard $(LOCAL_PATH)/libspeex/*.c*))

LOCAL_MODULE    := SpeexAndroid
LOCAL_LDLIBS    := -llog

include $(BUILD_SHARED_LIBRARY)
