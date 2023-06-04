target: compile

compile:
	g++ -pthread -Wall main.cpp src/disk.cpp src/memory.cpp src/utils.cpp src/info.cpp -lm -o bin/operateArrays

run: compile
	./bin/operateArrays 12 5 10 LRU inverted 100 diskFileName.dat
# ./bin/operateArrays 4 5 10 LRU inverted 100 diskFileName.dat
# ./bin/operateArrays 12 5 10 LRU inverted 10000 diskFileName.dat
# ./operateArrays frameSize numPhysical numVirtual pageReplacement tableType pageTablePrintInt diskFileName.dat

sc:
	g++ -pthread -Wall second_chance.cpp src/disk.cpp src/memory.cpp src/utils.cpp src/info.cpp -lm -o bin/sc
	./bin/sc 1 1 2 SC normal 100 diskFileName.dat
# ./bin/sc 2 4 6 LRU inverted 100 diskFileName.dat
# ./bin/sc 4 5 10 LRU inverted 100 diskFileName.dat
# ./bin/sc frameSize numPhysical numVirtual pageReplacement tableType pageTablePrintInt diskFileName.dat


clean:
	rm -f bin/**

debug_sc:
	g++ -pthread -Wall -g3 -DDEBUG second_chance.cpp src/disk.cpp src/memory.cpp src/utils.cpp src/info.cpp -lm -o bin/sc
# gdb -q ./bin/sc 
# 1 1 2 SC inverted 100 diskFileName.dat

debug_operateArrays:
	g++ -pthread -Wall -g main.cpp src/disk.cpp src/memory.cpp src/utils.cpp src/info.cpp -lm -o bin/operateArrays
	gdb -q ./bin/operateArrays
# r 12 5 10 LRU inverted 100 diskFileName.dat

valgrind:
	g++ -pthread -Wall main.cpp src/server.c src/common.c -lm -o bin/server
	g++ -pthread -Wall main.cpp src/servant.c src/common.c -lm -o bin/servant
	g++ -pthread -Wall main.cpp src/client.c src/common.c -lm -o bin/client
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/server -i files/file1.txt -j files/file2.txt -o output -n 4 -m 2

# Suleyman Golbol 1801042656