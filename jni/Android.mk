LOCAL_PATH := $(call my-dir)
MAIN_LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := dhook

LOCAL_CFLAGS := -Wno-error=format-security -fpermissive 
LOCAL_CFLAGS += -fno-rtti -fno-exceptions -Wconstant-logical-operand

LOCAL_C_INCLUDES += $(MAIN_LOCAL_PATH)

LOCAL_SRC_FILES := dhook.cpp \
			dutils.cpp \
			
		  

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)