#ifndef DLOGGER_H_
#define DLOGGER_H_

#include <android/log.h>



#define LOGD(tag, fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, tag, fmt "\n--> %s\n----> %s:%d", ##__VA_ARGS__, __FILE__, __FUNCTION__, __LINE__)
#define LOGE(tag, fmt, ...) __android_log_print(ANDROID_LOG_ERROR, tag, fmt "\n--> %s\n----> %s:%d", ##__VA_ARGS__, __FILE__, __FUNCTION__, __LINE__)

#endif