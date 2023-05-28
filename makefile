target: compile

compile:
	g++ -pthread -Wall main.cpp src/memory.cpp src/utils.cpp -lm -o bin/exe

run:
	g++ -std=c++17 -pthread -Wall main.cpp src/memory.cpp src/utils.cpp -lm -o bin/exe
	./bin/exe

run_only:
	./bin/exe

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