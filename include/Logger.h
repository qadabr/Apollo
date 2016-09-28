#ifndef LOGGER_H
#define LOGGER_H

#include <android/log.h>

#define  LOG_TAG "Horizon"

#define  LOG_D(...)							\
	do {								\
		__android_log_print(ANDROID_LOG_DEBUG, "[%s:ul %s]:\n",	\
				    __FILE__, __LINE__, __TIME__);	\
		__android_log_print(ANDROID_LOG_DEBUG,			\
				    LOG_TAG, __VA_ARGS__);		\
	} while (0);							

#define  LOG_E(...)							\
	do {								\
		__android_log_print(ANDROID_LOG_ERROR, "[%s:ul %s]:\n",	\
				    __FILE__, __LINE__, __TIME__);	\
		__android_log_print(ANDROID_LOG_DEBUG,			\
				    LOG_TAG, __VA_ARGS__);		\
	} while (0);							

#endif /* LOGGER_H */
