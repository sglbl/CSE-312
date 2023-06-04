#include <iostream>
#include <random>
#include <pthread.h>
#include <unistd.h>
#include "../include/utils.h"
#include "../include/info.h"
#include "../include/memory.h"
using namespace std;

void Info::argument_handler(int argc, char *argv[]){
    if(argc!=8)
        print_error_exit("Totally there should be 7 extra parameters!\nThe usage is(exp=2 to the exponent):\n"
            "./operateArrays frameSizeExp physicalFrameExp virtualFrameExp pageReplacement tableType printFreq diskFileName.dat");
    
    // Example: argv[1] = first argument, argv[2] = second argument, etc.

    int frameSizeExp = atoi(argv[1]);
    if(frameSizeExp < 0 ) print_error_exit("frameSizeExp. should be > 0\n");

    int physicalFrameExp = atoi(argv[2]);
    if(physicalFrameExp < 0) print_error_exit("physicalFrameExp. should be > 0\n");
    
    int virtualFrameExp = atoi(argv[3]);
    if(virtualFrameExp <0 ) print_error_exit("numVirtual should be > 0\n");

    string page_replacement(argv[4]);
    this->page_replacement = page_replacement;

    string table_type(argv[5]);
    this->table_type = table_type;

    int page_table_print = atoi(argv[6]);
    if(page_table_print<0) print_error_exit("pageTablePrint should be > 0\n");
    
    string disk_file_name(argv[7]);
    this->disk_file_name = disk_file_name;

    if(physicalFrameExp > virtualFrameExp) 
        print_error_exit("virtualFrameExp should be bigger than physicalFrameExp ! "); 

    // 1 << x = 2^x
    size_of_frame = 1 << frameSizeExp; // 2^frameSizeExp -> Ex: 2^12 = 4096
    frames_of_physical_memory = 1 << physicalFrameExp; // 2^physicalFrameExp -> Ex: 2^5 = 32
    pages_of_virtual_memory = 1 << virtualFrameExp; // 2^virtualFrameExp -> Ex: 2^10 = 1024

    size_of_physical_memory = size_of_frame * frames_of_physical_memory; // 4096 frame size * 32 physical frame = 128KB int
    size_of_virtual_memory = size_of_frame * pages_of_virtual_memory; // 4096 frame size * 1024 virtual frame = 4MB int

    // printing information
    cout << "\n[INFO: Size of a frame: " << size_of_frame << " integers]\n";
    cout << "[INFO: Number of frames in physical memory: " << frames_of_physical_memory << "]\n";
    cout << "[INFO: Number of pages in virtual memory: " << pages_of_virtual_memory << "]\n";
    cout << "[INFO: Size of physical memory: " << size_of_physical_memory << " integers]\n";
    cout << "[INFO: Size of virtual memory: " << size_of_virtual_memory << " integers]\n\n";
}

void Info::fillPageTableWith0s(PageTableEntry** page_table, int table_size){
    // NOTE: When incorrect, it is attempting to access a member of a pointer without dereferencing it properly.
    // incorrect dereference: page_table[1]->present 
    // corrrect dereference: (*page_table)[1].present or (*page_table + 1)->present    

    (*page_table)->next = NULL; 
    PageTableEntry *temp = *page_table;
    // initialize page table entries for physical and virtual memory
    for (int i = 0; i < table_size; i++){
        (*page_table)->present = 0;
        (*page_table)->modified_bit = 0;
        (*page_table)->referenced_bit = 0;
        (*page_table)->page_frame_number = i;
        (*page_table)->last_time_used = -1;
        (*page_table)->next = (PageTableEntry*)malloc(sizeof(PageTableEntry));
        (*page_table) = (*page_table)->next;
    }
    (*page_table) = temp;
}

void Info::fillPrintStatWith0s(PrintStatInfo* print_stat_info, int num_of_threads){
    for (int i = 0; i < num_of_threads; i++){
        print_stat_info[i].num_of_reads = 0;
        print_stat_info[i].num_of_writes = 0;
        print_stat_info[i].num_of_page_faults = 0;
        print_stat_info[i].num_of_page_replacements = 0;
        print_stat_info[i].num_of_disk_page_writes = 0;
        print_stat_info[i].num_of_disk_page_reads = 0;
        print_stat_info[i].estimated_ws_func = 0;
    }
}