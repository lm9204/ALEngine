cmake_minimum_required(VERSION 3.20)
project(AL)

# AL 라이브러리 생성
add_library(AL STATIC)

# 소스 및 헤더 파일 추가
target_sources(AL PRIVATE
    src/Core.cpp
    include/Core.h
)

# 헤더 파일 경로 추가
target_include_directories(AL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# AL의 컴파일 옵션 설정 (필요에 따라 추가)
target_compile_options(AL PRIVATE -Wall -Wextra)

# AL에 필요한 외부 라이브러리 링크 (필요 시)
# find_package(SomeLib REQUIRED)
# target_link_libraries(AL PRIVATE SomeLib::SomeLib)