# Wildcard pattern to include all .cpp files
SRC = $(wildcard src/*.cpp)
WARN = -Wall -Wextra
DEBUG = -g3 -DDEBUG

target: compile1 compile2

compile1:
	g++ $(WARN) file_system_maker.cpp $(SRC) -o bin/makeFileSystem
	
compile2:
	g++ $(WARN) file_system_operator.cpp $(SRC) -o bin/fileSystemOper

run: compile1
	./bin/makeFileSystem 4 sgFileSystem.dat

run2: compile2
	./bin/fileSystemOper  sgFileSystem.dat mkdir "/bin/"

clean:
	rm -f bin/**

# DEBUGGING: gdb -q ./bin/makeFileSystem
# RUNNING: r 4 mySystem.dat
debug1:
	g++ $(WARN) $(DEBUG) file_system_maker.cpp $(SRC) -o bin/makeFileSystem

debug2:
	g++ $(WARN) $(DEBUG) file_system_maker.cpp $(SRC) -o bin/makeFileSystem

# Suleyman Golbol 1801042656