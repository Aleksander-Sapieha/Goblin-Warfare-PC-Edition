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

SRC = src/Source.cpp src/Goblin.cpp src/Player.cpp src/save.cpp

SRC_C = src/SDD/sdd.c src/SDD/sdd_lexer.c src/SDD/sdd_parser.c

CC = gcc
CXX = g++
WIN_CC = x86_64-w64-mingw32-gcc
WIN_CXX = x86_64-w64-mingw32-g++

all: windows linux
	@echo "Make All Finished"

linux:
	@echo "Making Linux"
	@mkdir -p build/linux
	$(CC) -c $(SRC_C)
	$(CXX) $(SRC) sdd.o sdd_lexer.o sdd_parser.o -static -o $(LINPATH)
	@rm -f sdd.o sdd_lexer.o sdd_parser.o
	@echo "Make Linux Finished"

windows:
	@echo "Making Windows"
	@mkdir -p build/windows
	$(WIN_CC) -c $(SRC_C)
	$(WIN_CXX) $(SRC) sdd.o sdd_lexer.o sdd_parser.o -static -o $(WINPATH)
	@rm -f sdd.o sdd_lexer.o sdd_parser.o
	@echo "Make Windows Finished"

clean:
	@rm -f $(WINPATH) $(LINPATH)
	@rm -f sdd.o sdd_lexer.o sdd_parser.o
	@echo "make clean finished"
