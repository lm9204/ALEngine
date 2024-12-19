cmake_minimum_required(VERSION 3.20)
project(GameEngine)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 서브 디렉토리 추가
add_subdirectory(AL)
add_subdirectory(Sandbox)