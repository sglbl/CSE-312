#include <iostream>
#include <fstream>
#include "include/utils.h"
#include "include/externs.h"
using namespace std;

int main(int argc, char *argv[]){
    // Handle arguments
    part1_argument_handler(argc, argv);
    // Create binary file and open as in and out
    file.open(fname, ios::binary | ios::in | ios::out | ios::trunc);
    // Create file system in binary file
    fat12_fs_creator();
    // Close binary file
    file.close();

    return 0;
}
