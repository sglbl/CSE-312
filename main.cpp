#include <iostream>
#include <random>
#include <cstring>
#include <string>
#include "include/memory.h"
#include "include/info.h"
#include "include/disk.h"
#include "include/utils.h"
using namespace std;

// static PageTableEntry* physical_frame;
// static PageTableEntry* virtual_pages;
static PageTableEntry* page_table;
static VirtualMemory virtual_memory;
static PhysicalMemory physical_memory;
static PrintStatInfo print_stat_info[3];

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

    page_table = (PageTableEntry*)calloc(information.getSizeOfVirtualMemory(), sizeof(PageTableEntry));
    information.fillPageTableWith0s(&page_table, information.getSizeOfVirtualMemory());
    information.fillPrintStatWith0s(print_stat_info, 3);

    Disk disk;
    disk.setFileName(information.getDiskFileName());
    disk.createFile();

    // writing information in virtual memory to disk file
    disk.writeToFile(virtual_memory.getMemory(), information.getSizeOfVirtualMemory(), page_table);

    // reading information from disk file to physical memory
    // disk.readFromFile(information.getDiskFileName(), physical_memory.getMemory(), information.getSizeOfPhysicalMemory());

    // creating matrix and vectors from virtual memory
    create_matrix_and_vectors_from_memory(virtual_memory.getMemory(), information);

    // memory handling initialization with page faults, page replacements, etc.
    memory_handler(virtual_memory, physical_memory, page_table, disk, information, print_stat_info);

    // create threads and start them for each algorithm
    thread_handler();

    return 0;
}
