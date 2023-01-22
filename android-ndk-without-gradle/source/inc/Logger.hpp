//#include <android_native_app_glue.h>
#include <android/log.h>
#include <android/native_activity.h>
#include <android/asset_manager.h>
#define APPNAME "hello-world"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, APPNAME, __VA_ARGS__)
#define LOGA(cond, ...) __android_log_assert(cond, APPNAME, __VA_ARGS__)