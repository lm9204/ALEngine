cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME AfterLife)
project(${PROJECT_NAME})

# 실행 파일 생성 - window면 exe 파일
add_executable(${PROJECT_NAME})

# 소스 및 헤더 파일 추가
target_sources(${PROJECT_NAME} PRIVATE
    src/SandboxApp.cpp
)

# 헤더 파일 경로 추가
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Sandbox가 AL을 링크하도록 설정
target_link_libraries(${PROJECT_NAME} PRIVATE AL)

# Sandbox의 컴파일 옵션 설정 (필요에 따라 추가)
# target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)