all: windows linux
	@echo "Make All Finished"
	
linux:
	@echo "Making Linux"
	@mkdir -p build/linux
	@g++ src/Source.cpp src/Goblin.cpp src/Player.cpp -static -o build/linux/goblin-warfare
	@echo "Make Linux Finished"

windows:
	@echo "Making Windows"
	@mkdir -p build/windows
	@x86_64-w64-mingw32-g++ src/Source.cpp src/Goblin.cpp src/Player.cpp -static -o build/windows/goblin-warfare.exe
	@echo "Make Windows Finished"


clean:
	@echo "Removing build files"
	@rm -f build/linux/goblin-warfare
	@rm -f build/windows/goblin-warfare.exe
	@echo "Removed"