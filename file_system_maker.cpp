#include <iostream>
#include <fstream>
#include <random>
#include <cstring>
#include <string>
#include "include/utils.h"
#include "include/externs.h"
using namespace std;

// external definitions are in externs.h
FATDirectoryEntry* directory_table;   // FAT directory
ofstream wfile; // write file
FAT12 fat12;
string fname;
int block_size;
int num_of_blocks;

int main(int argc, char *argv[]){
    part1_argument_handler(argc, argv);

    wfile.open(fname, std::ios::binary);
    fat12_fs_creator();
    wfile.close();

    return 0;
}
