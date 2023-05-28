#include <iostream>
#include <random>
#include <pthread.h>
#include <unistd.h>
#include "../include/info.h"
#include "../include/memory.h"
using namespace std;

#define FRAME_SIZE 4096 // 4KB
#define VIRTUAL_PAGE_FRAMES 16
#define NUM_THREADS 16
#define MATRIX_ROW 1000
#define MATRIX_COL 3
#define VECTOR_COL 1000
#define VECTOR_ROW 3
#define RESULT_COL 1

/* matrixA * vectorA = resultA */
static int matrixA[MATRIX_ROW][MATRIX_COL] = {0};
static int vectorA[VECTOR_ROW][RESULT_COL] = {0};
static int resultA[MATRIX_ROW][RESULT_COL] = {0};

/* vectorB * vectorB^T = resultB  */
static int vectorB[1][VECTOR_COL] = {0};
static int vectorBT[VECTOR_COL][1] = {0};
static int resultB[1][1] = {0};

static int result_sum_array[MATRIX_ROW][RESULT_COL] = {0};
static int result_sum_vector[MATRIX_ROW * RESULT_COL + 1] = {0};

static int found_binary_search[5] = {0};
static int found_linear_search[5] = {0};
static SearchNumberInfo search_info = {{ -2, 59, 161, 376, 1000 },
                                { false, false, false, false, false},
                                { false, false, false, false, false}};

static int part1FinishedThreads;
static pthread_mutex_t csMutex;
static pthread_mutex_t barrierMutex;
static pthread_cond_t barrierCond;

void Info::print_error_exit(string message){
    cerr << message;
    exit(EXIT_FAILURE);
}

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
    int size_of_frame = 1 << frameSizeExp; // 2^frameSizeExp -> Ex: 2^12 = 4096
    int frames_of_physical_memory = 1 << physicalFrameExp; // 2^physicalFrameExp -> Ex: 2^5 = 32
    int frames_of_virtual_memory = 1 << virtualFrameExp; // 2^virtualFrameExp -> Ex: 2^10 = 1024

    int size_of_physical_memory = size_of_frame * frames_of_physical_memory; // 4096 frame size * 32 physical frame = 128KB int
    int size_of_virtual_memory = size_of_frame * frames_of_virtual_memory; // 4096 frame size * 1024 virtual frame = 4MB int

}

