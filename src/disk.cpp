#include <iostream>
#include <cstring>
#include <random>
#include <pthread.h>
#include <unistd.h>
#include "../include/utils.h"
#include "../include/info.h"
#include "../include/disk.h"
#include "../include/externs.h"
using namespace std;

void Disk::createFile()
{
    cout << "Creating file " << file_name << endl;
    // initialize disk file
    disk_file = fopen(file_name.c_str(), "w");
    if (disk_file == NULL){
        print_error_exit("Error: Disk file could not be created.\n");
    }
    fclose(disk_file);
}

void Disk::writeToFile(int* memory, int memory_size, PageTableEntry* page_table, int page_table_size){
    cout << "Writing to file " << file_name << endl;
    // open disk file
    disk_file = fopen(file_name.c_str(), "w");
    if (disk_file == NULL){
        print_error_exit("Error: Disk file could not be opened.\n");
    }
    // write to disk file
    int page_counter = 0;
    for(int i=0; i<memory_size; i++){
        // write page table entry
        if(i % page_table_size == 0){
            if(i != 0)  fprintf(disk_file, "\n");
            // fprintf(disk_file, "%s:%d\n", "Pte", page_table[page_counter].page_frame_number);
            fprintf(disk_file, "%d,%d,%d,%d,%d\n", page_table[page_counter].page_frame_number, 
                page_table[page_counter].referenced_bit, page_table[page_counter].modified_bit,
                page_table[page_counter].present, page_table[page_counter].last_time_used); // page_table[i].page_number);
            page_counter++;
        }
        // write virtual memory content to disk file 
        fprintf(disk_file, "%d,", memory[i]);
    }
    
    fclose(disk_file);
}

PageTableEntry Disk::getPage(int page_table_index){
    // read from disk page_table_index*2 + 1th line //çç
    // open disk file
    disk_file = fopen(file_name.c_str(), "r");
    if (disk_file == NULL){
        print_error_exit("Error: Disk file could not be opened.\n");
    }
    // read from disk file
    char line[10000];
    int line_counter = 0;
    while (fgets(line, sizeof(line), disk_file) != NULL){
        if(line_counter == page_table_index*2+1){
            break;
        }
        line_counter++;
    }
    cout << "Get page table: " << line << endl;
    // close disk file
    fclose(disk_file);
    
    // fprintf(disk_file, "%d,%d,%d,%d,%d\n", page_table[page_counter].page_frame_number, 
    //             page_table[page_counter].referenced_bit, page_table[page_counter].modified_bit,
    //             page_table[page_counter].present, page_table[page_counter].last_time_used);
    
    // parse line using sscanf
    PageTableEntry page_table_entry;
    sscanf(line, "%d,%d,%d,%d,%d", &page_table_entry.page_frame_number, &page_table_entry.referenced_bit, 
        &page_table_entry.modified_bit, &page_table_entry.present, &page_table_entry.last_time_used);
    return page_table_entry;
}

int* Disk::getPageFrameElements(int page_table_index){
    // read from disk page_table_index*2 + 1 th line
    // open disk file
    disk_file = fopen(file_name.c_str(), "r");
    if (disk_file == NULL){
        print_error_exit("Error: Disk file could not be opened.\n");
    }

    // read from disk file
    char line[10000];
    int line_counter = 0;
    while (fgets(line, sizeof(line), disk_file) != NULL){
        if(line_counter == page_table_index*2){
            break;
        }
        line_counter++;
    }
    // close disk file
    fclose(disk_file);

    // parse line using sscanf
    int* page_frame_elements = (int*)calloc(information.getPagesOfVirtualMemory(), sizeof(int));
    char* token = strtok(line, ",");
    int i = 0;
    while(token != NULL){
        page_frame_elements[i] = atoi(token);
        token = strtok(NULL, ",");
        i++;
    }
    return page_frame_elements;
}