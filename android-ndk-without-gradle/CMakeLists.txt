cmake_minimum_required(VERSION 3.17)
project(hello-world VERSION 1.0.0 LANGUAGES CXX)

if(UMBRELLA)
  include(apk_build.cmake)
  return()
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C++ Sandbox libraries list
file(GLOB Sandbox_CPP "source/inc/*.hpp" "source/src/*.cpp")
file(GLOB DATASTRUCT_CPP "source/Data_Struct/inc/*.hpp" "source/Data_Struct/src/*.cpp")
file(GLOB GRE_GAME "source/GRE_Game/inc/*.hpp" "source/GRE_Game/src/*.cpp")

# C++ includes
include_directories(source/Data_Struct/inc)
include_directories(source/GRE_Game/inc)
include_directories(source/inc)

# The Sandbox_C++ library (C code)
add_library(Sandbox_CPP SHARED ${Sandbox_CPP})
add_library(libDataStructCPP SHARED ${DATASTRUCT_CPP})
add_library(libGreGame SHARED ${GRE_GAME})

add_library(hello-world SHARED source/main.cpp)
target_link_libraries(hello-world PUBLIC log Sandbox_CPP libDataStructCPP libGreGame)
install(TARGETS hello-world)
