#ifndef INFO_H
#define INFO_H
#include "memory.h"
using namespace std;

class Info {
    public:
        void argument_handler(int argc, char *argv[]);
        void print_error_exit(string message);
        inline int getSizeOfFrame(){ return size_of_frame; }
        inline int getFramesOfPhysicalMemory(){ return frames_of_physical_memory; }
        inline int getFramesOfVirtualMemory(){ return frames_of_virtual_memory; }
        inline int getSizeOfPhysicalMemory(){ return size_of_physical_memory; }
        inline int getSizeOfVirtualMemory(){ return size_of_virtual_memory; }
    private:    
        int size_of_frame;
        int frames_of_physical_memory;
        int frames_of_virtual_memory;  // virtual page
        int page_table_print;
        string page_replacement;
        string table_type;
        string disk_file_name;
        int size_of_physical_memory;
        int size_of_virtual_memory;
};

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
} ThreadInfo;

typedef struct SearchNumberInfo{
    int numbers[5];
    bool is_found_linear[5];
    bool is_found_binary[5];
} SearchNumberInfo;

typedef struct PageTableEntry{
	int referenced_bit;
	int modified_bit;
	int present;
    int page_frame_number;
} PageTableEntry;

typedef struct PrintStatInfo{
    int num_of_reads;
    int num_of_writes;
    int num_of_page_faults;
    int num_of_page_replacements;
    int num_of_disk_page_writes;
    int num_of_disk_page_reads;
    int estimated_ws_func;
} PrintStatInfo;

#endif // INFO_H