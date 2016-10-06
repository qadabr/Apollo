#ifndef LOGGER_H
#define LOGGER_H

#define  LOG_TAG "Apollo"

#include <android/log.h>

#define LOG_E(M, ...) __android_log_print(ANDROID_LOG_ERROR,		\
					  LOG_TAG,			\
					  "[%s:%d][%s]: " M "",		\
					  __FILE__, __LINE__,		\
					  __func__,			\
					  ##__VA_ARGS__)


#define LOG_D(M, ...) __android_log_print(ANDROID_LOG_DEBUG,		\
					  LOG_TAG,			\
					  "[%s:%d][%s]: " M "",		\
					  __FILE__, __LINE__,		\
					  __func__,			\
					  ##__VA_ARGS__)

#define LOG_W(M, ...) __android_log_print(ANDROID_LOG_WARN,		\
					  LOG_TAG,			\
					  "[%s:%d][%s]: " M "",		\
					  __FILE__, __LINE__,		\
					  __func__,			\
					  ##__VA_ARGS__)

#define LOG_I(M, ...) __android_log_print(ANDROID_LOG_INFO,		\
					  LOG_TAG,			\
					  "[%s:%d][%s]: " M "",		\
					  __FILE__, __LINE__,		\
					  __func__,			\
					  ##__VA_ARGS__)

#endif /* LOGGER_H */
