#ifndef DISK_H
#define DISK_H
#include <iostream>
#include <string>
#include "memory.h"
using namespace std;

class Disk {
    public:
        void createFile();
        void writeToFile(int memory_size, int frame_size);
        int* getPageFrameElements(int page_table_index);
        PageTableEntry getPage(int page_table_index);
        void writePageToFile(int page_frame_number, int *page_elements);
        inline void setFileName(string file_name){ this->file_name = file_name; }
        inline string getFileName(){ return file_name; }
        inline FILE* getFile(){ return disk_file; }
    private:    
        FILE* disk_file;
        string file_name;
        int number_of_lines;
};


#endif // INFO_H