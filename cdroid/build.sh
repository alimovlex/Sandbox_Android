#!/bin/bash

adb shell am force-stop com.example.cdroid
adb uninstall com.example.cdroid
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE="$HOME/android/ndk-bundle/build/cmake/android.toolchain.cmake" -DANDROID_SDK="$HOME/android" -DANDROID_PLATFORM="android-29" -DANDROID_ABI=x86_64
cmake --build . --target create_keystore cdroid

adb install result.apk
rm -rf build/
adb shell am start -n com.example.cdroid/android.app.NativeActivity
adb logcat | grep -F "`adb shell ps | grep com.example.cdroid  | tr -s [:space:] ' ' | cut -d' ' -f2`"

