#include <stdlib.h>
#include <string.h>
#include "Tutorial.h"
#include "Logger.h"
#include "Object.h"

#define APPNAME "CDROID"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, APPNAME, __VA_ARGS__)
#define LOGA(cond, ...) __android_log_assert(cond, APPNAME, __VA_ARGS__)


void android_main(struct android_app *app) {
    //characterSetTest();
    listFiles();
    //fileTest();
    preprocessingTest();
    dataTypeSizeTest();
    timerFunction(pointersTest);
    localTimeCheck();
    fileTest(app);
    sandbox();

    test_Animals_Class();

    ANativeActivity_finish(app->activity);
}