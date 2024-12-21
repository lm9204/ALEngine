cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME Sandbox)
project(${PROJECT_NAME})

# 소스 파일 설정
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# lib 경로 설정
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/build/AL/install/include")
set(LIB_DIR "${CMAKE_SOURCE_DIR}/build/AL/install/lib")

# 실행 파일 생성
add_executable(${PROJECT_NAME} ${SOURCES})

# Sandbox가 AL 라이브러리를 링크하도록 설정
target_link_libraries(${PROJECT_NAME} PRIVATE AfterLife)


# 헤더 파일 경로 추가
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/AL/include)

# 매크로 정의
target_compile_definitions(${PROJECT_NAME} PUBLIC AL_PLATFORM_WINDOWS)

# DLL의 런타임 경로 설정 (옵션)
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Sandbox의 컴파일 옵션 설정 (필요에 따라 추가)
# target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)