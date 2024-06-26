include(ExternalProject)

set(ANDROID_SDK_ROOT $ENV{ANDROID_SDK_ROOT} CACHE PATH "$ENV{HOME}/android")

set(ANDROID_PLATFORM_TOOLS_DIR "${ANDROID_SDK_ROOT}/platform-tools")

file(GLOB ANDROID_BUILD_TOOLS_VERSIONS "${ANDROID_SDK_ROOT}/build-tools/*")
list(POP_BACK ANDROID_BUILD_TOOLS_VERSIONS ANDROID_BUILD_TOOLS_LATEST)
set(ANDROID_BUILD_TOOLS_DIR "${ANDROID_BUILD_TOOLS_LATEST}" CACHE PATH "$ENV{HOME}/android/build-tools/29.0.3")

file(GLOB ANDROID_NDK_VERSIONS "${ANDROID_SDK_ROOT}/ndk-bundle/*")
list(POP_BACK ANDROID_NDK_VERSIONS ANDROID_NDK_LATEST)
set(ANDROID_NDK "${ANDROID_NDK_LATEST}" CACHE PATH "$ENV{HOME}/android/ndk-bundle")

file(GLOB ANDROID_JAR_VERSIONS "${ANDROID_SDK_ROOT}/platforms/*")
list(POP_BACK ANDROID_JAR_VERSIONS ANDROID_JAR_LATEST)
set(ANDROID_JAR "${ANDROID_JAR_LATEST}/android.jar" CACHE PATH "$ENV{HOME}/android/platforms/android-29")

find_program(ADB       NAMES adb       REQUIRED PATHS ${ANDROID_PLATFORM_TOOLS_DIR})
find_program(AAPT2     NAMES aapt2     REQUIRED PATHS ${ANDROID_BUILD_TOOLS_DIR})
find_program(APKSIGNER NAMES apksigner REQUIRED PATHS ${ANDROID_BUILD_TOOLS_DIR})
find_program(ZIPALIGN  NAMES zipalign  REQUIRED PATHS ${ANDROID_BUILD_TOOLS_DIR})

set(APK_CONTENTS_ROOT "${CMAKE_CURRENT_BINARY_DIR}/apk")

macro(setup_variant VARIANT)
  ExternalProject_Add(${PROJECT_NAME}-${VARIANT}
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
    EXCLUDE_FROM_ALL TRUE
    CMAKE_ARGS
    -DANDROID_ABI=${VARIANT}
    -DANDROIDVERSION=29
    -DANDROID_NDK=$ENV{HOME}/android/ndk-bundle
    -DANDROID_STL=c++_static
    -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/install/${VARIANT}
    -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=${APK_CONTENTS_ROOT}/lib/${VARIANT}
    -DCMAKE_TOOLCHAIN_FILE=$ENV{HOME}/android/ndk-bundle/build/cmake/android.toolchain.cmake
    )
endmacro()
setup_variant(arm64-v8a)
setup_variant(armeabi-v7a)
setup_variant(x86)
setup_variant(x86_64)

set(MANIFEST "${CMAKE_CURRENT_SOURCE_DIR}/AndroidManifest.xml")
set(RESOURCES "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(KEYSTORE "${CMAKE_CURRENT_SOURCE_DIR}/keystore.jks")

set(VALUES_STRING "${CMAKE_CURRENT_BINARY_DIR}/values_strings.arsc.flat")

set(FINAL_APK "${CMAKE_CURRENT_BINARY_DIR}/app.apk")
set(RESOURCES_APK "${CMAKE_CURRENT_BINARY_DIR}/app.res.apk")
set(UNALIGNED_APK "${CMAKE_CURRENT_BINARY_DIR}/app.unaligned.apk")

#keytool -genkeypair -keystore keystore.jks -alias androidkey -validity 10000 -keyalg RSA -keysize 2048 -storepass android -keypass android

add_custom_command(
  OUTPUT ${VALUES_STRING}
  COMMAND ${AAPT2} compile ${RESOURCES}/values/strings.xml -o ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${RESOURCES}/values/strings.xml
  )
add_custom_command(
  OUTPUT ${RESOURCES_APK}
  COMMAND ${AAPT2} link ${VALUES_STRING} -o ${RESOURCES_APK} --manifest ${MANIFEST} -I $ENV{HOME}/android/platforms/android-29/android.jar
  DEPENDS ${MANIFEST} ${VALUES_STRING}
  )
add_custom_command(
  OUTPUT ${UNALIGNED_APK}
  COMMAND ${CMAKE_COMMAND} -E tar x ${RESOURCES_APK}
  COMMAND ${CMAKE_COMMAND} -E tar c ${UNALIGNED_APK} --format=zip .
  WORKING_DIRECTORY ${APK_CONTENTS_ROOT}
  DEPENDS
    ${RESOURCES_APK}
    ${PROJECT_NAME}-armeabi-v7a
    ${PROJECT_NAME}-arm64-v8a
    ${PROJECT_NAME}-x86
    ${PROJECT_NAME}-x86_64
  )
add_custom_command(
  OUTPUT ${FINAL_APK}
  COMMAND ${ZIPALIGN} -p -f -v 4 ${UNALIGNED_APK} ${FINAL_APK}
  COMMAND keytool -genkey -v -keystore my-release-key.keystore -alias standkey -keyalg RSA -keysize 2048 -validity 10000 -storepass password -keypass password -dname "CN=example.com, OU=ID, O=Example, L=Doe, S=John, C=GB"
  COMMAND ${APKSIGNER} sign --in ${FINAL_APK} -ks my-release-key.keystore --ks-key-alias standkey --ks-pass pass:password --key-pass pass:password
  DEPENDS ${UNALIGNED_APK}
  )

add_custom_target(apk ALL DEPENDS ${FINAL_APK})

install(CODE "execute_process(COMMAND ${ADB} install ${FINAL_APK})")
