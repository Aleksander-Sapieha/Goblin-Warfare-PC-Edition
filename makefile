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

SRC = src/Source.cpp src/Goblin.cpp src/Player.cpp src/save.cpp src/ModLoader.cpp src/ItemRegistry.cpp src/PluginLoader.cpp
SRC_C = src/SDD/sdd.c src/SDD/sdd_lexer.c src/SDD/sdd_parser.c

CC = gcc
CXX = g++
WIN_CC = x86_64-w64-mingw32-gcc
WIN_CXX = x86_64-w64-mingw32-g++
CPPFLAGS = -IModSDK

SDK = src/ModSDK
PLUGIN_SRC = mods/Dragon\ Mod/Dragon.cpp

APPLE_SRC=  mods/Apple\ Mod/Apple.cpp

all: windows linux
	@echo "Make All Finished"

linux:
	@echo "Making Linux"
	@mkdir -p build/linux
	$(CC) -c $(SRC_C)
	$(CXX) $(CPPFLAGS) $(SRC) sdd.o sdd_lexer.o sdd_parser.o -ldl -o $(LINPATH)
	@rm -f sdd.o sdd_lexer.o sdd_parser.o
	@echo "Make Linux Finished"

windows:
	@echo "Making Windows"
	@mkdir -p build/windows
	$(WIN_CC) -c $(SRC_C)
	$(WIN_CXX) $(CPPFLAGS) $(SRC) sdd.o sdd_lexer.o sdd_parser.o -static -o $(WINPATH)
	@rm -f sdd.o sdd_lexer.o sdd_parser.o
	@echo "Make Windows Finished"

clean:
	@rm -f $(WINPATH) $(LINPATH)
	@rm -f sdd.o sdd_lexer.o sdd_parser.o
	@rm -f "mods/Dragon Mod/enemies.so" "mods/Dragon Mod/enemies.dll"
	@echo "make clean finished"