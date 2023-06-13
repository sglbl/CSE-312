#include <iostream>
#include <random>
#include <pthread.h>
#include <unistd.h>
#include <fstream>
#include "../include/externs.h"
using namespace std;

void print_error_exit(string message){
    cerr << "Error: " << message;
    exit(EXIT_FAILURE);
}

/* PART 1 FUNCTIONS */
void part1_argument_handler(int argc, char *argv[]){
    if(argc!=3)
        print_error_exit("Totally there should be 2 extra parameters!\nThe usage is:\n"
            "./bin/makeFileSystem blockSize mySystem.dat");
    
    // Example: argv[1] = first argument, argv[2] = second argument, etc.

    block_size = atoi(argv[1]);
    if(block_size < 0 ) print_error_exit("Block size can be 0.5, 1, 2 or 4\n");
    cout << "Block size: " << block_size << endl;
    string disk_file(argv[2]);
    fname = disk_file;
    cout << "Disk file name: " << fname << endl;
}

void fat12_fs_creator(){
    block_size = block_size * 1024; // convert from KB to bytes by multiplying 2^10	
    num_of_blocks = pow(2, 12); // 2^12 = 4096 blocks since FAT12 is used

    // filep = fopen(fname.c_str(), "w+"); // create a file
    // if(filep == NULL) print_error_exit("File could not be created!\n");

    if (!wfile.is_open()) {
        print_error_exit("Failed to open file.");
    }
    /* Creating a FAT12 file system */

    // Write block size
    write_to_file(block_size, 2);

    // Write number of blocks
    write_to_file(num_of_blocks, 2);
    
    // Write number of free blocks
    int free_blocks = num_of_blocks - 1; // 1 block is reserved for FAT
    write_to_file(free_blocks, 2);

    // Write number of files
    int num_of_files = 0;
    write_to_file(num_of_files, 2);

    // Write number of directories
    int num_of_dirs = 0;
    write_to_file(num_of_dirs, 2);

    int i=-9;
    while(i < num_of_blocks){
        write_to_file(EMPTY_BLOCK, 2);
        i++;
    }

    i = 0;
    int max_size = num_of_blocks * block_size;
    while(i < max_size){
        write_to_file(0);
        i++;
    }

    wfile.close();

    // // read_from_file(fname, block_size);
    // char buf[48];
    // int block_siz = 48;
    // read_from_file(buf, block_siz);
    // cout << "buf: " << buf << endl;

}

bool write_to_file(int num, int size){
    short number = (short) num;
    // short = 2 bytes = 16 bits
    // so writing as 2 char bytes. first 8 bits and last 8 bits
    for (int i = 0; i < size; i++) {
        char byte;
        if (i == 0)
            byte = 0xFF & (number >> 8); // first 8 bits
        else if(i == 1)
            byte = 0xFF & number; // last 8 bits
        // cout << (int)byte << " ";
        wfile.write(&byte, sizeof(byte));
    }
    // cout << endl;
    // std::cout << "Integer written to file successfully." << std::endl;
}

/* PART2 FUNCTIONS */

void part2_argument_handler(int argc, char *argv[], string usage_message){
    if(argc < 3)
        print_error_exit(usage_message);
    
    // Example: argv[1] = first argument, argv[2] = second argument, etc.

    block_size = atoi(argv[1]);
    if(block_size < 0 ) print_error_exit("Block size can be 0.5, 1, 2 or 4\n");
    cout << "Block size: " << block_size << endl;
    string disk_file(argv[2]);
    fname = disk_file;
    cout << "Disk file name: " << fname << endl;
}

bool read_from_file(string buffer, int size){
    // std::ifstream file(fname, std::ios::binary);
    // if (!file.is_open()) {
    //     print_error_exit("Failed to open file.");
    // }
    // file.read(&buffer[0], size);
    // file.close();
    // std::cout << "Integer read from file successfully." << std::endl;

    std::ifstream file(fname, std::ios::binary);
    int number = 0;
    char byte;
    cout << "Reading: " << endl;
    for (int i = 0; i < size; i++) {
        file.read(&byte, sizeof(byte));
        cout << (int)byte << " ";
        number |= (byte & 0xFF) << (8 * i);
    }
    cout << endl;
    file.close();

}