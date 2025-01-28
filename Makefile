NAME :=	ALEngine

# Vulkan SDK 경로 (설치된 경로로 수정하세요)
VULKAN_SDK := C:/VulkanSDK/1.4.304.0

# C# 컴파일러 경로 (설치된 Visual Studio 버전에 맞게 수정하세요)
CSHARP_COMPILER := "C:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/Roslyn/csc.exe"

all: $(NAME)_debug

release: $(NAME)_release

# Debug 빌드
$(NAME)_debug:
	@cmake -Bbuild \
		-DCMAKE_BUILD_TYPE=Debug \
		-DCMAKE_CSharp_COMPILER=$(CSHARP_COMPILER) \
		-DVulkan_INCLUDE_DIR=$(VULKAN_SDK)/Include \
		-DVulkan_LIBRARY=$(VULKAN_SDK)/Lib/vulkan-1.lib \
		-DVulkan_GLSLANG_VALIDATOR_EXECUTABLE=$(VULKAN_SDK)/Bin/glslangValidator.exe \
		-DVulkan_GLSLC_EXECUTABLE=$(VULKAN_SDK)/Bin/glslc.exe .
	@cmake --build build --config Debug
	@echo [SUCCESS] $@ compiled successfully with debug mode and validation layers!

# Release 빌드
$(NAME)_release:
	@cmake -Bbuild \
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_CSharp_COMPILER=$(CSHARP_COMPILER) \
		-DVulkan_INCLUDE_DIR=$(VULKAN_SDK)/Include \
		-DVulkan_LIBRARY=$(VULKAN_SDK)/Lib/vulkan-1.lib \
		-DVulkan_GLSLANG_VALIDATOR_EXECUTABLE=$(VULKAN_SDK)/Bin/glslangValidator.exe \
		-DVulkan_GLSLC_EXECUTABLE=$(VULKAN_SDK)/Bin/glslc.exe .
	@cmake --build build --config Release
	@echo [SUCCESS] $@ compiled successfully without validation layers!

# 클린 빌드 디렉토리
clean:
	@if exist build rmdir /s /q build
	@echo [CLEAN] Build files have been removed!

# 모든 실행 파일 제거
fclean: clean
	@rm -rf $(NAME)_debug $(NAME)_release
	@echo [FCLEAN] Executable files have been fully removed!

# 클린 후 다시 빌드
re: fclean all

# 셰이더 컴파일
shader:
	@powershell -Command "New-Item -ItemType Directory -Force -Path spvs"
	@for %%f in (shaders\*.vert) do "$(VULKAN_SDK)\Bin\glslc.exe" "%%f" -o "spvs\%%~nf.vert.spv"
	@for %%f in (shaders\*.frag) do "$(VULKAN_SDK)\Bin\glslc.exe" "%%f" -o "spvs\%%~nf.frag.spv"
	@echo [SUCCESS] Shaders have been compiled successfully!

# 디버그 모드 실행
run: $(NAME)_debug
	@./build/bin/Debug/Sandbox.exe

.PHONY: all clean fclean re debug release