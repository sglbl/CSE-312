// #ifndef UTILS_H
// #define UTILS_H
// #include "memory.h"

// enum MultiplicationAlgo{
//     MATRIX_BY_VECTOR = 0,
//     VECTOR_BY_T = 1
// };

// enum MultiplicationType{
//     ROW_BY_COLUMN = 0,
//     COLUMN_BY_COLUMN = 1
// };

// enum SummationType{
//     ROW_SUMM = 0,
//     COLUMN_SUMM = 1
// };

// typedef struct ThreadInfo{
//     int thread_id;
//     int page_frame;
//     MultiplicationAlgo multiplication_algo;
//     MultiplicationType multiplication_type;
//     SummationType summation_type;
//     int num_of_columns_to_calculate;
// } ThreadInfo;

// typedef struct SearchNumberInfo{
//     int numbers[5];
//     bool is_found_linear[5];
//     bool is_found_binary[5];
// } SearchNumberInfo;

// typedef struct PageTableEntry{
// 	int referenced_bit;
// 	int modified_bit;
// 	int present;
//     int page_frame_number;
// } PageTableEntry;


// void handle_page_fault();

// void fill_table_with_randoms(int* table, int table_size);

// void multiplication_handler(VirtualTable table);

// void create_matrix_and_vectors_from_table(int *table);

// void* thread_job(void *arg);

// void* main_thread_job(void *arg);

// void barrier();

// #endif // UTILS_H