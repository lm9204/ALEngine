cmake_minimum_required(VERSION 3.20)
project(GameEngine)

if(NOT WIN32)
    message(FATAL_ERROR "This project can only be built on Windows.")
endif()

message(STATUS "Building on Windows environment.")

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 서브 디렉토리 추가
add_subdirectory(AL-ScriptCore)
add_subdirectory(AL)
add_subdirectory(Sandbox)