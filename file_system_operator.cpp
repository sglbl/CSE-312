#include <iostream>
#include <fstream>
#include <random>
#include <cstring>
#include <string>
#include "include/utils.h"
#include "include/externs.h"
using namespace std;

int main(int argc, char *argv[]){
    string usage_message = 
        "Usages:\n./makeFileSystem blockSizeKB sgFileSystem.dat \
        ./fileSystemOper sgFileSystem.dat mkdir \"/usr\" \
        ./fileSystemOper sgFileSystem.dat mkdir \"/usr/ysa\" \
        ./fileSystemOper sgFileSystem.dat write \"/usr/ysa/file1\" linuxFile.data \
        ./fileSystemOper sgFileSystem.dat write \"/file2\" linuxFile.data \
        ./fileSystemOper sgFileSystem.dat dir \"/\" \
        ./fileSystemOper sgFileSystem.dat del \"/usr/ysa/file1\" \
        ./fileSystemOper sgFileSystem.dat dumpe2fs \
        ./fileSystemOper sgFileSystem.dat read \"/usr/file2\" linuxFile2.data \
        cmp linuxFile2.data linuxFile.data";
    
    part2_argument_handler(argc, argv, usage_message);
    
    file.open(fname, ios::binary | ios::in | ios::out);
    
    fat12_fs_operator();
    file.close();

    return 0;
}
