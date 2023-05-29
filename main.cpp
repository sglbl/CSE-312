#include <iostream>
#include <random>
#include <cstring>
#include <string>
#include "include/memory.h"
#include "include/info.h"
using namespace std;

// static PageTableEntry* physical_frame;
// static PageTableEntry* virtual_pages;
static PageTableEntry* page_table;
static VirtualMemory virtual_memory;
static PhysicalMemory physical_memory;
static PrintStatInfo print_stat_info[3];

void print_error_exit(string message){
    cerr << message;
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    // initialize info class
    Info information;
    // writing arguments to info class
    information.argument_handler(argc, argv);
    // initialize memory class using *arguments*.
    virtual_memory.setMemorySize(information.getSizeOfVirtualMemory());
    physical_memory.setMemorySize(information.getSizeOfPhysicalMemory());

    // filling virtual memory with random numbers
    virtual_memory.fillMemoryWithRandoms(virtual_memory.getMemory(), information.getSizeOfVirtualMemory());
    cout << virtual_memory.getMemory()[0] << endl;

    // no need to fill physical memory because it will be filled with virtual memory pages from disk
    // physical_memory.fillMemoryWithRandoms(virtual_memory.getMemory(), information.getSizeOfVirtualMemory());

    // initialize page table entries for physical and virtual memory
	// virtual_pages = (PageTableEntry*)calloc(information.getFramesOfVirtualMemory(), sizeof(PageTableEntry));
    // physical_frame = (PageTableEntry*)calloc(information.getFramesOfPhysicalMemory(), sizeof(PageTableEntry));

    // // initialize page table entries for physical and virtual memory
    // for (int i = 0; i < information.getFramesOfVirtualMemory(); i++){
    //     virtual_pages[i].present = 0;
    //     virtual_pages[i].modified_bit = 0;
    //     virtual_pages[i].referenced_bit = 0;
    //     virtual_pages[i].page_frame_number = -1;
    // }

    // for (int i = 0; i < information.getFramesOfPhysicalMemory(); i++){
    //     physical_frame[i].present = 0;
    //     physical_frame[i].modified_bit = 0;
    //     physical_frame[i].referenced_bit = 0;
    //     physical_frame[i].page_frame_number = -1;
    // }

    information.fillPageTableWith0s(page_table, information.getFramesOfVirtualMemory());
    information.fillPrintStatWith0s(print_stat_info, 3);

    // initialize disk file
    FILE* disk_file = fopen(information.getDiskFileName().c_str(), "w");
    if (disk_file == NULL){
        print_error_exit("Error: Disk file could not be created.\n");
    }
    fclose(disk_file);



}


// int main(){
//     // 64 kb virtual memory size
//     int virtual_table_size = 64 * 1024;
//     // 16 kb physical memory size
//     int physical_table_size = 16 * 1024;
//     // create virtual and physical memory table
//     VirtualTable virtual_table(virtual_table_size);
//     PhysicalTable physical_table(physical_table_size);

//     // filling with random numbers
//     fill_table_with_randoms(virtual_table.getTable(), virtual_table_size);
//     cout << virtual_table.getTable()[0] << endl;

//     create_matrix_and_vectors_from_table(virtual_table.getTable());
//     // resultA[999][0] = 88;
//     multiplication_handler(virtual_table);
//     // summation_handler(virtual_table);

//     cout << "Hello World!\n";
//     return 0;
// }
