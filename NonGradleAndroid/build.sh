#!/bin/bash

set -e

GET_OS=$(uname -s)

case $GET_OS in
    "Linux")
    BUILD_TOOLS="$HOME/android/build-tools/29.0.3"
    PLATFORM="$HOME/android/platforms/android-29/android.jar"
    ;;
    "Darwin")
    BUILD_TOOLS="$HOME/Library/Android/sdk/build-tools/30.0.3"
    PLATFORM="$HOME/Library/Android/sdk/platforms/android-30/android.jar"
    ;;
    *)
    if [[ -z "${ANDROID_HOME}" ]]; then
    echo "No Compatible platform (Linux or Mac) found, Please set $ANDROID_HOME in your PATH."
    exit 1
    else
    BUILD_TOOLS="$ANDROID_HOME/build-tools/30.0.3"
    PLATFORM="$ANDROID_HOME/platforms/android-30/android.jar"
    fi
    ;;
esac

AAPT2="$BUILD_TOOLS/aapt2"
D8="$BUILD_TOOLS/d8"
ZIPALIGN="$BUILD_TOOLS/zipalign"
APKSIGNER="$BUILD_TOOLS/apksigner"

echo "Cleaning..."
rm -rf classes/
rm -rf compiled/
rm -rf build/
rm -rf src/io/github/synix/helloworld/R.java
rm my-release-key.keystore

echo "Compiling resources..."
mkdir -p compiled
$AAPT2 compile res/values/strings.xml res/layout/activity_main.xml -o compiled/


echo "Linking resources & Generating R.java.."
mkdir -p build
$AAPT2 link -o build/unsigned_app.apk -I $PLATFORM \
    -R compiled/*.flat \
    --manifest AndroidManifest.xml \
    --java src/ \
    -v

echo "Compiling Java source code..."
mkdir -p classes
javac -d classes -classpath src -bootclasspath $PLATFORM -source 1.8 -target 1.8 src/io/github/synix/helloworld/MainActivity.java
javac -d classes -classpath src -bootclasspath $PLATFORM -source 1.8 -target 1.8 src/io/github/synix/helloworld/R.java

echo "Translating to Dalvik bytecode..."
$D8 classes/io/github/synix/helloworld/*.class --lib $PLATFORM --output build/

echo "Packaging APK..."
# Maybe have better solution than using zip 
zip -uj build/unsigned_app.apk build/classes.dex

echo "Aligning APK..."
$ZIPALIGN -f 4 build/unsigned_app.apk build/app.apk

echo "Signing APK..."
keytool -genkey -v -keystore my-release-key.keystore -alias standkey -keyalg RSA -keysize 2048 -validity 10000 -storepass password -keypass password -dname "CN=example.com, OU=ID, O=Example, L=Doe, S=John, C=GB"
$APKSIGNER sign --key-pass pass:password --ks-pass pass:password --ks my-release-key.keystore build/app.apk 

adb install -r build/app.apk
adb shell am start -n io.github.synix.helloworld/.MainActivity