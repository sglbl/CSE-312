#include <iostream>
#include <cstring>
#include <random>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h> 
#include "../include/utils.h"
#include "../include/info.h"
#include "../include/disk.h"
#include "../include/externs.h"
using namespace std;

void Disk::createFile(){
    cout << "[INFO: Creating file " << file_name << "]\n";
    // initialize disk file
    disk_file = fopen(file_name.c_str(), "w");
    if (disk_file == NULL){
        print_error_exit("Error: Disk file could not be created.\n");
    }
    fclose(disk_file);
}

void Disk::writeToFile(int memory_size, int frame_size){
    cout << "[INFO: Writing page table and virtual memory to disk file " << file_name << "]\n";
    // open disk file
    disk_file = fopen(file_name.c_str(), "w");
    if (disk_file == NULL){
        print_error_exit("Error: Disk file could not be opened.\n");
    }
    // write to disk file
    int page_counter = 0;
    number_of_lines = 1;
    for(int i=0; i<memory_size; i++){
        // write page table entry
        if(i % frame_size == 0){
            if(i != 0){  
                fprintf(disk_file, "\n");
                number_of_lines++;
            }
            // cout << "Print 3030" << endl;
            // cout << ".present is " << page_table[3030].present << endl;
            // cout << "Writing page table entry " << page_counter << endl;
            // cout << i << " is i\n";
            // cout << "memory size is " << memory_size << endl;
            fprintf(disk_file, "%d,%d,%d,%d,%d\n", 
                    page_table[page_counter].page_frame_number, 
                    page_table[page_counter].present, // valid bit
                    page_table[page_counter].referenced_bit, // used bit
                    page_table[page_counter].modified_bit, // dirty bit            
                    page_table[page_counter].last_time_used);
            page_counter++;
            number_of_lines++;
        }
        // write virtual memory content to disk file 
        fprintf(disk_file, "%d,", virtual_memory.getElement(i));
    }
    
    fclose(disk_file);
}

PageTableEntry Disk::getPage(int page_table_index){
    // read from disk page_table_index*2 th line 
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
    // cout << "Get page table: " << line << endl;
    // close disk file
    fclose(disk_file);
    
    // parse line using sscanf
    PageTableEntry page_table_entry;
    sscanf(line, "%d,%d,%d,%d,%d",
            &page_table_entry.page_frame_number, 
            &page_table_entry.present, // valid bit
            &page_table_entry.referenced_bit, // used bit
            &page_table_entry.modified_bit, // dirty bit            
            &page_table_entry.last_time_used);
    return page_table_entry;
}

int* Disk::getPageFrameElements(int page_table_index){
    // read from disk page_table_index*2 + 1 th line
    // open disk file
    disk_file = fopen(file_name.c_str(), "r");
    if (disk_file == NULL){
        print_error_exit("Error: Disk file could not be opened in getPageFrameElements().\n");
    }

    // read from disk file
    char line[10000];
    int line_counter = 0;
    while (fgets(line, sizeof(line), disk_file) != NULL){
        // cout << page_table_index << " page_table_index" << endl;
        // cout << line_counter << " line_counter" << endl;
        if(line_counter == page_table_index*2+1){
            break;
        }
        line_counter++;
    }
    // number_of_lines = line_counter;
    // cout << "Number of lines: " << number_of_lines << endl;

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

// void Disk::writePageToFile(int page_frame_number, int *page_elements){
//     // use to change the page_frame_number*2 + 1 th line
//     disk_file = fopen(file_name.c_str(), "r");
//     if (disk_file == NULL){
//         print_error_exit("Error: Disk file could not be opened in writePageToFile.\n");
//     }

//     // read from disk file
//     char **lines = (char**)calloc(number_of_lines, sizeof(char*));
//     cout << "Num of lines " << number_of_lines << endl;

//     char *line_buf = NULL;
//     size_t line_buf_size = 0;

//     int line_counter = 0;
//     size_t line_size = getline(&line_buf, &line_buf_size, disk_file);
//     cout << "line_size: " << line_size << endl;
//     while (line_size >= 0){
//         line_counter++;
//         // cout << "line_counter: " << line_counter << endl;
//         // printf("line[%06d]: chars=%06zd, buf size=%06zu, contents: %s", line_counter,
//         //     line_size, line_buf_size, line_buf);
//         line_size = getline(&line_buf, &line_buf_size, disk_file);
//         // cout << line_counter << " |";
//         lines[line_counter-1] = line_buf;
//     }


//     // int line_counter = 0;
//     // while (fgets(line, sizeof(line), disk_file) != NULL){
//     //     if(line_counter == page_frame_number*2+1){
//     //         break;
//     //     }
//     //     line_counter++;
//     // }

//     // cout << "Write page to file: " << line << endl;

//     // // parse line using sscanf
//     // int* page_frame_elements = (int*)calloc(information.getPagesOfVirtualMemory(), sizeof(int));
//     // char* token = strtok(line, ",");
//     // int i = 0;
//     // while(token != NULL){
//     //     page_frame_elements[i] = atoi(token);
//     //     token = strtok(NULL, ",");
//     //     i++;
//     // }
    

// }

