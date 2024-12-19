cmake_minimum_required(VERSION 3.20)
project(Sandbox)

# 실행 파일 생성
add_executable(Sandbox)

# 소스 및 헤더 파일 추가
target_sources(Sandbox PRIVATE
    src/main.cpp
    include/App.h
    include/App.cpp
)

# 헤더 파일 경로 추가
target_include_directories(Sandbox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Sandbox가 AL을 링크하도록 설정
target_link_libraries(Sandbox PRIVATE AL)

# Sandbox의 컴파일 옵션 설정 (필요에 따라 추가)
target_compile_options(Sandbox PRIVATE -Wall -Wextra)