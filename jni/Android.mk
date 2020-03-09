LOCAL_PATH := $(call my-dir)
MAIN_LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := hook

LOCAL_CFLAGS := -Wno-error=format-security -fpermissive 
LOCAL_CFLAGS += -fno-rtti -fno-exceptions -Wconstant-logical-operand

LOCAL_C_INCLUDES += $(MAIN_LOCAL_PATH)/dhook

LOCAL_SRC_FILES := entry_point.cpp \
				   dhook/dhook.cpp
			
		  

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
