target: compile

compile:
	g++ -pthread -Wall main.cpp src/memory.cpp src/utils.cpp src/info.cpp -lm -o bin/operateArrays

run: compile
	./bin/operateArrays 4 5 10 LRU inverted 100 diskFileName.dat
# ./operateArrays frameSize numPhysical numVirtual pageReplacement tableType pageTablePrintInt diskFileName.dat


run_only:
	./bin/operateArrays

clean:
	rm -f bin/**

debug:
	g++ -pthread -Wall -g src/server.c src/common.c -lm -o bin/server
	g++ -pthread -Wall -g src/servant.c src/common.c -lm -o bin/servant
	g++ -pthread -Wall -g src/client.c src/common.c -lm -o bin/client
# gdb -q ./bin/server

valgrind:
	g++ -pthread -Wall main.cpp src/server.c src/common.c -lm -o bin/server
	g++ -pthread -Wall main.cpp src/servant.c src/common.c -lm -o bin/servant
	g++ -pthread -Wall main.cpp src/client.c src/common.c -lm -o bin/client
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/server -i files/file1.txt -j files/file2.txt -o output -n 4 -m 2

# Suleyman Golbol 1801042656