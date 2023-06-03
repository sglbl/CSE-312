#ifndef UTILS_H
#define UTILS_H
#include "memory.h"
#include "disk.h"

enum MultiplicationAlgo{
    MATRIX_BY_VECTOR = 0,
    VECTOR_BY_T = 1
};

enum MultiplicationType{
    ROW_BY_COLUMN = 0,
    COLUMN_BY_COLUMN = 1
};

enum SummationType{
    ROW_SUMM = 0,
    COLUMN_SUMM = 1
};

typedef struct ThreadInfo{
    int thread_id;
    int page_frame;
    MultiplicationAlgo multiplication_algo;
    MultiplicationType multiplication_type;
    SummationType summation_type;
    int num_of_columns_to_calculate;
    // bool is_found[5];
} ThreadInfo;

typedef struct SearchNumberInfo{
    int numbers[5];
    bool is_found_linear_t1[5];
    bool is_found_binary_t2[5];
} SearchNumberInfo;

void print_error_exit(string error_message);

void handle_page_fault(int page_table_index, string algorithm);

void thread_handler();

void create_matrix_and_vectors_from_memory(int *memory);

void* thread_job(void *arg);

void* main_thread_job(void *arg);

void barrier();

// void memory_handler(VirtualMemory virtualMemory, PhysicalMemory physicalMemory, PageTableEntry* pageTable, Disk disk, Info information, PrintStatInfo printStatInfo[3]);

int get(int index, string algorithm);

void set(int index, int value, string algorithm);

#endif // UTILS_H