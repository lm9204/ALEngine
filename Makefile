NAME :=	ALEngine

all: $(NAME)_debug

release: $(NAME)_release

$(NAME)_debug:
	@make shader
	@cmake -Bbuild -DCMAKE_BUILD_TYPE=Debug .
	@cmake --build build --config Debug
	@echo [SUCCESS] $@ compiled successfully with debug mode and validation layers!

$(NAME)_release:
	@make shader
	@cmake -Bbuild -DCMAKE_BUILD_TYPE=Release .
	@cmake --build build --config Release
	@echo [SUCCESS] $@ compiled successfully without validation layers!

clean:
	@if exist build rmdir /s /q build
	@echo [CLEAN] Build files have been removed!

fclean: clean
	@rm -rf $(NAME)_debug $(NAME)_release
	@echo [FCLEAN] Executable files have been fully removed!

re: fclean all

shader :
	@mkdir -p ./spvs
	@for file in ./shaders/*.vert; do \
		glslc $$file -o ./spvs/$$(basename $$file .vert).vert.spv; \
	done
	@for file in ./shaders/*.frag; do \
		glslc $$file -o ./spvs/$$(basename $$file .frag).frag.spv; \
	done
	@echo [SUCCESS] Shaders have been compiled successfully!

run : $(NAME)_debug
	@./build/bin/Debug/Sandbox.exe

.PHONY: all clean fclean re debug release