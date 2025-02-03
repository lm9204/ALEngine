cmake_minimum_required(VERSION 3.20)
project(AL-ScriptCore LANGUAGES CSharp)

# .NET Framework 버전 설정
set(DOTNET_FRAMEWORK_VERSION "v4.7.2")

# 출력 및 중간 디렉터리 설정
# set(TARGET_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Sandbox/Resources/Scripts")
# set(OBJ_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Sandbox/Resources/Intermediates")

# 소스 파일 및 프로퍼티 파일 검색
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cs")
file(GLOB_RECURSE PROPERTIES "${CMAKE_CURRENT_SOURCE_DIR}/properties/*.cs")

# C# 공유 라이브러리 추가
add_library(AL-ScriptCore SHARED ${SOURCES} ${PROPERTIES})

# 출력 디렉터리 설정
# set_target_properties(AL-ScriptCore PROPERTIES
#     VS_DOTNET_TARGET_FRAMEWORK_VERSION ${DOTNET_FRAMEWORK_VERSION}
# )

# # CMake의 RUNTIME_OUTPUT_DIRECTORY를 명시적으로 설정
# set_target_properties(AL-ScriptCore PROPERTIES
#     RUNTIME_OUTPUT_DIRECTORY ${TARGET_DIR}
#     LIBRARY_OUTPUT_DIRECTORY ${TARGET_DIR}
#     ARCHIVE_OUTPUT_DIRECTORY ${TARGET_DIR}
# )

# CMake의 중간 파일 디렉터리 설정
# set(CMAKE_CSharp_OUTPUT_DIR ${OBJ_DIR})
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TARGET_DIR})
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${TARGET_DIR})
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${TARGET_DIR})

# Debug / Release 빌드에 따라 값 설정
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Visual StudioDebug 빌드 설정
    set_target_properties(AL-ScriptCore PROPERTIES
        VS_GLOBAL_Optimize "false"
        VS_GLOBAL_DebugSymbols "true"
        VS_DOTNET_TARGET_FRAMEWORK_VERSION ${DOTNET_FRAMEWORK_VERSION}
    )
else()
    set_target_properties(AL-ScriptCore PROPERTIES
        VS_GLOBAL_Optimize "true"
        VS_GLOBAL_DebugSymbols "false"
        VS_DOTNET_TARGET_FRAMEWORK_VERSION ${DOTNET_FRAMEWORK_VERSION}
    )
endif()
