# Wildcard pattern to include all .cpp files
SRC = $(wildcard src/*.cpp)

target: compile

compile:
	g++ -Wall file_system_maker.cpp $(SRC) -o bin/makeFileSystem
# g++ -Wall file_system_maker.cpp $(SRC) -o bin/fileSystemOper

run: compile
	./bin/makeFileSystem 4 sgFileSystem.dat

run2: compile
	./bin/fileSystemOper 

clean:
	rm -f bin/**

# DEBUGGING: gdb -q ./bin/makeFileSystem
# RUNNING: r 4 mySystem.dat
debug1:
	g++ -Wall -g3 -DDEBUG file_system_maker.cpp $(SRC) -o bin/makeFileSystem

debug2:
	g++ -Wall -g3 -DDEBUG file_system_maker.cpp $(SRC) -o bin/makeFileSystem

# Suleyman Golbol 1801042656