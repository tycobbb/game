#ifndef LOG_H
#define LOG_H

// -- constants --

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_INFO  1
#define LOG_LEVEL_DEBUG 2

// -- config --

#define LOG_LEVEL LOG_LEVEL_INFO

// -- interface --

#if LOG_LEVEL < LOG_LEVEL_ERROR
#define LOGE(...)
#else
#define LOGE(...) (printf(__VA_ARGS__))
#endif

#if LOG_LEVEL < LOG_LEVEL_INFO
#define LOGI(...)
#else
#define LOGI(...) (printf(__VA_ARGS__))
#endif

#if LOG_LEVEL < LOG_LEVEL_DEBUG
#define LOGD(...)
#else
#define LOGD(...) (printf(__VA_ARGS__))
#endif

#endif