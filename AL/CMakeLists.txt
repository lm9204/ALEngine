cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME AfterLife)
project(${PROJECT_NAME})

# 소스 파일 및 헤더 파일 검색
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

# AL 라이브러리 생성
add_library(${PROJECT_NAME} STATIC ${SOURCES}) # SOURCES만 전달

# PCH 설정
target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/alpch.h)

# 헤더 파일 경로 포함
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# AL의 컴파일 옵션 설정 (필요에 따라 추가)
# target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

# AL에 필요한 외부 라이브러리 링크 (필요 시)
# find_package(SomeLib REQUIRED)
# target_link_libraries(AL PRIVATE SomeLib::SomeLib)