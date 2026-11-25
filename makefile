#
#     Goblin-Warfare-PC-Edition/makefile
#
#     Creator: Aleksander Sapieha (sourceguy68@gmail.com)
#
#     Contributors:
#     Aleksander Sapieha (sourceguy68@gmail.com)
#

WINPATH = build/windows/goblin-warfare.exe
LINPATH = build/linux/goblin-warfare

SRC = src/Source.cpp src/Goblin.cpp src/Player.cpp


 ifeq ($(UNAME),Darwin)
     SHELL := /opt/local/bin/bash
     OS_X  := true
 else ifneq (,$(wildcard /etc/redhat-release))
     OS_RHEL := true
 else
     OS_DEB  := true
     SHELL := /bin/bash
 endif 

ifneq ("$(wildcard $(LINPATH))","")
	LINUX_EXISTS = true
else
	LINUX_EXISTS = false
endif

ifneq ("$(wildcard $(WINPATH))","")
	WIN_EXISTS = true
else
	WIN_EXISTS = false
endif


all: windows linux
	@echo "Make All Finished"
	
linux:
	@echo "Making Linux"
	@mkdir -p build/linux
	@g++ $(SRC) -static -o $(LINPATH)
	@echo "Make Linux Finished"

windows:
	@echo "Making Windows"
	@mkdir -p build/windows
	@x86_64-w64-mingw32-g++ $(SRC) -static -o $(WINPATH)
	@echo "Make Windows Finished"


clean:
ifeq ($(WIN_EXISTS), true)
	@echo "Removing Windows build files..."
	@rm -rf $(WINPATH)
else
	@echo "Windows build files not found, skipping..."
endif
ifeq ($(LINUX_EXISTS), true)
	@echo "Removing Linux build files..."
	@rm -rf $(LINPATH)
else
	@echo "Linux build files not found, skipping..."
endif
	@echo "make clean finished"