#!/bin/sh

BUILD_DIR=build_android
ANDROID_BUILD_DIR=$BUILD_DIR/android-build

rm -rf $BUILD_DIR
cmake --preset=ci-ninja-android-debug
cmake --build $BUILD_DIR --target apk -j4
rm $ANDROID_BUILD_DIR/build.gradle 
rm $ANDROID_BUILD_DIR/gradle/wrapper/gradle-wrapper.properties
cp build.gradle $ANDROID_BUILD_DIR/build.gradle
cp gradle-wrapper.properties $ANDROID_BUILD_DIR/gradle/wrapper
cmake --build build_android --target apk -j4
