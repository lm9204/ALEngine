cmake_minimum_required(VERSION 3.20)
project(SandboxProject LANGUAGES CSharp)

# .NET Framework 버전 설정
set(DOTNET_FRAMEWORK_VERSION "v4.7.2")

# 출력 및 중간 디렉터리 설정
set(TARGET_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(OBJ_DIR "${CMAKE_CURRENT_SOURCE_DIR}//Intermediates")

# 소스 파일 검색
file(GLOB_RECURSE SANDBOX_PROJECT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cs")
file(GLOB_RECURSE SANDBOX_PROJECT_PROPERTIES "${CMAKE_CURRENT_SOURCE_DIR}/properties/*.cs")

# C# 공유 라이브러리 생성
add_library(SandboxProject SHARED ${SANDBOX_PROJECT_SOURCES} ${SANDBOX_PROJECT_PROPERTIES})

# 출력 디렉터리 설정
set_target_properties(SandboxProject PROPERTIES
    OUTPUT_DIRECTORY ${TARGET_DIR}
    INTERMEDIATE_DIRECTORY ${OBJ_DIR}
    VS_DOTNET_TARGET_FRAMEWORK_VERSION ${DOTNET_FRAMEWORK_VERSION}
)

# CMake의 RUNTIME_OUTPUT_DIRECTORY 설정
set_target_properties(SandboxProject PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TARGET_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${TARGET_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${TARGET_DIR}
)

# Debug 빌드 설정
set_target_properties(SandboxProject PROPERTIES
    VS_GLOBAL_Optimize "false"
    VS_GLOBAL_DebugSymbols "true"
)

# AL-ScriptCore를 SandboxProject에 링크
target_link_libraries(SandboxProject PRIVATE AL-ScriptCore)
