target: compile

compile:
	g++ -pthread -Wall operateArrays.cpp src/disk.cpp src/memory.cpp src/utils.cpp src/info.cpp -lm -o bin/operateArrays
	g++ -pthread -Wall second_chance.cpp src/disk.cpp src/memory.cpp src/utils.cpp src/info.cpp -lm -o bin/sc

operateArrays: compile
	./bin/operateArrays 1 1 2 SC normal 100 diskFileName.dat
# ./bin/operateArrays 4 5 10 SC normal 100 diskFileName.dat
# ./operateArrays frameSize numPhysical numVirtual pageReplacement tableType pageTablePrintInt diskFileName.dat

sc:
	g++ -pthread -Wall second_chance.cpp src/disk.cpp src/memory.cpp src/utils.cpp src/info.cpp -lm -o bin/sc
	# ./bin/sc 1 1 2 SC normal 100 diskFileName.dat
# ./bin/sc 4 5 10 SC normal 100 diskFileName.dat
# ./bin/sc frameSize numPhysical numVirtual pageReplacement tableType pageTablePrintInt diskFileName.dat

clean:
	rm -f bin/**

debug:
	g++ -pthread -Wall -g3 -DDEBUG operateArrays.cpp src/disk.cpp src/memory.cpp src/utils.cpp src/info.cpp -lm -o bin/operateArrays
	g++ -pthread -Wall -g3 -DDEBUG second_chance.cpp src/disk.cpp src/memory.cpp src/utils.cpp src/info.cpp -lm -o bin/sc

debug_sc:
	g++ -pthread -Wall -g3 -DDEBUG second_chance.cpp src/disk.cpp src/memory.cpp src/utils.cpp src/info.cpp -lm -o bin/sc
# gdb -q ./bin/sc 
# 1 1 2 SC inverted 100 diskFileName.dat

debug_operateArrays:
	g++ -pthread -Wall -g operateArrays.cpp src/disk.cpp src/memory.cpp src/utils.cpp src/info.cpp -lm -o bin/operateArrays
	gdb -q ./bin/operateArrays
# r 12 5 10 SC normal 100 diskFileName.dat

# Suleyman Golbol 1801042656