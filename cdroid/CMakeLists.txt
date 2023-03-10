cmake_minimum_required(VERSION 3.10)
project(cdroid C)

set(CMAKE_C_STANDARD 11)

# cmake module with pack_apk and create[_debug]_keystore
include(cmake/cdroid.cmake)
include_directories(inc)

find_library(log_lib log REQUIRED)

# we use add_library instead of add_executable for android
add_library(cdroid SHARED 
                src/main.c 
                src/Tutorial.c
                src/mman.c
                src/array.c
                src/OODbg.c
                src/Object.c)

target_link_libraries(cdroid PRIVATE android ${log_lib})

# create a keystore file and use it when packaging apk
set(keystore_file "${CMAKE_CURRENT_BINARY_DIR}/keystore.jks")
create_debug_keystore(${keystore_file})
# tie packaging process to the cdroid target build
pack_apk(cdroid # target that declares anroid_main
        "com.example.cdroid" "cdroid app" # android package name and app title
        "latest" # function will try to use latest build tools, can be a string like "30.0.1"
        android/AndroidManifest.xml.in # manifest to be configured (see cmake configure_file)
        android/assets # dir containing assets
        android/res # dir containing resources
        "${keystore_file}" "androidkey" "password" # default keystore alias and password values for debug keystore
        ${CMAKE_CURRENT_BINARY_DIR}/result.apk) # resulting apk file
