LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := minnative
LOCAL_CFLAGS := -std=c99
LOCAL_LDFLAGS := -Wl,--build-id
LOCAL_LDLIBS := \
	-llog
LOCAL_SRC_FILES := \
    min.c

LOCAL_C_INCLUDES += $(LOCAL_PATH)

include $(BUILD_SHARED_LIBRARY)
