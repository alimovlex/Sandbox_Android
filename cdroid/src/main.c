#include <stdlib.h>
#include <string.h>
#include "Tutorial.h"
#include "Logger.h"
#include "Object.h"

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