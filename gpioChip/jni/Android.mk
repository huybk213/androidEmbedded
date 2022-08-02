LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := gpio
#LOCAL_CFLAGS := -std=c99
LOCAL_LDFLAGS := -Wl,--build-id
LOCAL_LDLIBS := \
	-llog

LOCAL_CPP_EXTENSION := .cpp

LOCAL_SRC_FILES := core.c helpers.c misc.c  iter.c  ctxless.c

LOCAL_SRC_FILES += gpio.cpp

LOCAL_C_INCLUDES += $(LOCAL_PATH)

include $(BUILD_SHARED_LIBRARY)
