#ifndef DISK_H
#define DISK_H
#include <iostream>
#include <string>
#include "memory.h"
using namespace std;

class Disk {
    public:
        void createFile();
        void writeToFile(int* memory, int memory_size, PageTableEntry* page_table, int page_table_size);
        int* getPageFrameElements(int page_table_index);
        PageTableEntry getPage(int page_table_index);
        inline void setFileName(string file_name){ this->file_name = file_name; }
        inline FILE* getFile(){ return disk_file; }
    private:    
        FILE* disk_file;
        string file_name;
};


#endif // INFO_H