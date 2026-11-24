mkdir -p build/windows
mkdir -p build/linux

x86_64-w64-mingw32-g++ src/Source.cpp src/Goblin.cpp src/Player.cpp -static -o build/windows/goblin-warfare.exe
g++ src/Source.cpp src/Goblin.cpp src/Player.cpp -static -o build/linux/goblin-warfare