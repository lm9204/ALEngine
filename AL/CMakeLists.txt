cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME AfterLife)
project(${PROJECT_NAME})

include(${CMAKE_SOURCE_DIR}/AL/Dependency.cmake)

# 소스 파일 및 헤더 파일 검색
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

# AL 라이브러리 생성
add_library(${PROJECT_NAME} SHARED ${SOURCES}) # SOURCES만 전달

# PCH 설정
target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/alpch.h)

# 헤더 파일 경로 포함
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})

# lib 경로 설정
target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS})

# 매크로 정의
target_compile_definitions(${PROJECT_NAME} PUBLIC AL_PLATFORM_WINDOWS AL_BUILD_DLL)

# 출력 디렉토리 설정
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

# Dependency들이 먼저 build 될 수 있게 관계 설정 / 뒤에서 부터 컴파일
add_dependencies(${PROJECT_NAME} ${DEP_LIST})

# AL의 컴파일 옵션 설정 (필요에 따라 추가)
# target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

