###############################################################################
#  Makefile (최상위)
###############################################################################
#  이 파일은 운영체제/환경을 감지하여, 알맞은 Makefile (msvc/mingw)을 include 합니다.
###############################################################################

# OS 감지 (Windows_NT 여부)
ifeq ($(OS),Windows_NT)
  # MinGW(Git Bash) 여부 판단
  UNAME_S := $(shell uname -s 2>/dev/null)
  ifneq (,$(findstring MINGW,$(UNAME_S)))
    # MinGW 환경
    include Makefile.mingw
  else
    # MSVC CMD 환경
    include makefiles/Makefile.msvc
  endif
else
  include makefiles/Makefile.mingw
endif
