#!/bin/bash

adb shell am force-stop com.github.m4c0.androidNdkWithoutGradle
adb uninstall com.github.m4c0.androidNdkWithoutGradle
cmake -S . -B build -GNinja -DUMBRELLA=1 
cmake --build build

adb install build/app.apk
rm -rf build/
adb shell am start -n com.github.m4c0.androidNdkWithoutGradle/android.app.NativeActivity
adb logcat | grep -F "`adb shell ps | grep com.github.m4c0.androidNdkWithoutGradle  | tr -s [:space:] ' ' | cut -d' ' -f2`"

