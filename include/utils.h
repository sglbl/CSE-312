#ifndef UTILS_H
#define UTILS_H
#include "memory.h"
#include "disk.h"

// typedef struct LinkedList{
//     int page_frame;
//     int reference_bit;
//     struct LinkedList *next;
// } LinkedList;

typedef struct ThreadInfo{
    int thread_id;
    int page_frame;
    int num_of_columns_to_calculate;
} ThreadInfo;

typedef struct SearchNumberInfo{
    int numbers[5];
    bool is_found_linear_t1[5];
    bool is_found_binary_t2[5];
} SearchNumberInfo;

void print_error_exit(string error_message);

void handle_page_fault(int page_table_index, string algorithm);

void thread_handler();

PageTableEntry getPTE(PageTableEntry *head, int index);

PageTableEntry setPTE(PageTableEntry *head, int index, int pfn, int present, int modified, int referenced);

PageTableEntry setPTE_pfn(PageTableEntry *head, int index, int bit);

PageTableEntry setPTE_present(PageTableEntry *head, int index, int bit);

PageTableEntry setPTE_referenced(PageTableEntry *head, int index, int bit);

PageTableEntry setPTE_modified(PageTableEntry *head, int index, int bit);

void create_matrix_and_vectors_from_memory(int *memory);

void* thread_job(void *arg);

void* main_thread_job(void *arg);

void barrier();

// void memory_handler(VirtualMemory virtualMemory, PhysicalMemory physicalMemory, PageTableEntry* pageTable, Disk disk, Info information, PrintStatInfo printStatInfo[3]);

int get(int index, string algorithm);

void set(int index, int value, string algorithm);

#endif // UTILS_H