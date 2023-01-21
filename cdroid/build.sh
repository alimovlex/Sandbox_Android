#!/bin/bash

mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE="/home/robot/android/ndk-bundle/build/cmake/android.toolchain.cmake" -DANDROID_SDK="/home/robot/android" -DANDROID_PLATFORM="android-29" -DANDROID_ABI=arm64-v8a
cmake --build . --target create_keystore cdroid
#adb shell cmd com.example.cdroid uninstall -k
adb install result.apk
rm -rf build/
adb shell am start -n com.example.cdroid/android.app.NativeActivity
adb logcat | grep -F "`adb shell ps | grep com.example.cdroid  | tr -s [:space:] ' ' | cut -d' ' -f2`"