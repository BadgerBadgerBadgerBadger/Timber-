cmake_minimum_required(VERSION 3.2)
project(Timber)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(Timber ${SOURCE_FILES})

#set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "/Users/shak/Downloads/SFML-2.5.1-macos-clang/include/SFML")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
target_link_libraries(Timber sfml-graphics sfml-audio)
