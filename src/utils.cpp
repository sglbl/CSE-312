#include <iostream>
#include <random>
#include <pthread.h>
#include <unistd.h>
#include "../include/utils.h"
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

void handle_page_fault(){
    cout << "Page Fault!\n";
}

void fill_table_with_randoms(int* table, int table_size){
    // Mersenne Twister random number generator
    int rand_number;
    random_device rand_device;
    mt19937 rand(rand_device());
    uniform_int_distribution<mt19937::result_type> rand_func(1, 50);

    for (int i = 0; i < table_size; i++){
        rand_number = rand_func(rand_device);    
        table[i] = rand_number;
    }
}

void create_matrix_and_vectors_from_table(int *table){
    int line = MATRIX_ROW / VIRTUAL_PAGE_FRAMES; // 1000 / 16 = 62
    int index = 0;
    for(int i = 0; i < MATRIX_ROW; ++i){
        for(int j = 0; j < MATRIX_COL; ++j){
            int frame_no = i / line; // 0, 1, 2, 3, 4, 5,..., 15
            matrixA[i][j] = table[4*1024*frame_no + index++];
        }
    }
    for(int i = 0; i < VECTOR_ROW; ++i){
        for(int j = 0; j < RESULT_COL; ++j){
            int frame_no = i / line; // 0, 1, 2, 3, 4, 5,..., 15
            vectorA[i][j] = table[4*1024*frame_no + index++];
        }
    }
    for(int i = 0; i < 1; ++i){
        for(int j = 0; j < VECTOR_COL; ++j){
            int frame_no = i / line; // 0, 1, 2, 3, 4, 5,..., 15
            vectorB[i][j] = table[4*1024*frame_no + index++];
        }
    }

    // transpose vectorB
    for(int i = 0; i < VECTOR_COL; ++i){
        for(int j = 0; j < RESULT_COL; ++j){
            vectorBT[i][j] = vectorB[j][i];
        }
    }

}

void multiplication_handler(VirtualTable table){
    pthread_t threads[NUM_THREADS];
    pthread_t main_thread;

    int rc;
    ThreadInfo *thread_info = (ThreadInfo*)calloc(sizeof(ThreadInfo), NUM_THREADS);

    pthread_mutex_init(&csMutex, NULL); 
    pthread_mutex_init(&barrierMutex, NULL); 
    pthread_cond_init(&barrierCond, NULL);

    // Thread info initialization
    for (int i = 0; i < NUM_THREADS; i++){  
        thread_info[i].thread_id =  i;
        thread_info[i].page_frame = i;
        thread_info[i].num_of_columns_to_calculate = MATRIX_ROW / NUM_THREADS; // 1000/16=62
        if(i == NUM_THREADS - 1){
            thread_info[i].num_of_columns_to_calculate += MATRIX_ROW % NUM_THREADS; // 1000%16=8
        }
    }

    // Main thread creation. It will calculate the summation.
    if( pthread_create(&main_thread, NULL, main_thread_job, (void*)0) != 0 ){ //if returns 0 it's okay.
            perror("pthread_create()");
            exit(EXIT_FAILURE);
    }

    // Thread creation (start threads)
    for(int t=0; t<NUM_THREADS; t++){
        rc = pthread_create(&threads[t], NULL, thread_job,  (void*)&thread_info[t]);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    // Thread join (wait for all threads to finish)
    for(int i = 0; i < NUM_THREADS; i++){
        if( pthread_join(threads[i], NULL) != 0 ){
            perror("pthread_join()");
            exit(EXIT_FAILURE);
        }
        else{
            printf("Joined thread %d\n", i);
        }
    }

    // Main thread
    if( pthread_join(main_thread, NULL) != 0 ){ // main thread is detached
        perror("pthread exiting error for main thread");
        exit(EXIT_FAILURE);
    }else{
        printf("Detached main thread\n");
    }

    pthread_exit(NULL);
}

void* main_thread_job(void *arg){
    // cout << "Printing ResultA\n";
    // for(int i = 0; i < MATRIX_ROW; ++i){
    //     for(int j = 0; j < RESULT_COL; ++j){
    //         cout << resultA[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    // cout << "Printing ResultB\n";
    // for(int i = 0; i < 1; ++i){
    //     for(int j = 0; j < 1; ++j){
    //         cout << resultB[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    barrier(); // wait for all threads to finish their part1 job

    pthread_mutex_lock(&csMutex);
    // Calculate resultA + resultB in critical section
    // array summation    
    // Result of resultA[MATRIX_ROW][RESULT_COL] + resultB[1][1] will be result_sum_array
    for(int i = 0; i < MATRIX_ROW; ++i){
        for(int j = 0; j < RESULT_COL; ++j){
            result_sum_array[i][j] = resultA[i][j] + resultB[0][0];
        }
    }
    
    // Copy the elements of resultA to resultC_vector
    int index = 0;
    for (int i = 0; i < MATRIX_ROW; ++i) {
        for (int j = 0; j < RESULT_COL; ++j) {
            result_sum_vector[index++] = resultA[i][j];
        }
    }
    // Copy the element from resultB to resultC_vector
    result_sum_vector[index] = resultB[0][0];    
    pthread_mutex_unlock(&csMutex);

    cout << "Printing ResultC\n";
    for(int i = 0; i < 1; ++i){
        for(int j = 0; j < 1; ++j){
            cout << result_sum_vector[i] << " ";
        }
        cout << endl;
    }

    pthread_exit(NULL);
}

void* thread_job(void *arg){
    // Allowing instant printing with cout without buffer
    setvbuf(stdout, NULL, _IONBF, 0);

    ThreadInfo* thread_info = (ThreadInfo*)arg;
    cout << "Hello, My Thread ID: " << thread_info->thread_id << endl;

    /*  Every thread will calculate the some column of matrix C = matrixA * matrixB
            A           B           C
        [ a b c ]   [ k - - ]   [ x - -]
        [ d e f ] * [ l - - ] = [ y - -]
        [ g h j ]   [ m - - ]   [ z - -]
        For example 1st thread will calculate the 1st column (index=0) of matrix C    
        x = a * k + b * l + c * m
        y = d * k + e * l + f * m
        z = g * k + h * l + j * m
    */

    // Every thread will calculate a part of the resultA = matrixA * vectorA
    for(int i = 0; i < thread_info->num_of_columns_to_calculate; ++i){
        // thread_info->i will calculate (info->num_of_columns_to_calculate) columns
        int rowIndex = thread_info->thread_id * thread_info->num_of_columns_to_calculate + i;

        if(thread_info->thread_id == NUM_THREADS-1){  // For the remaining columns
            // use previous ones number of columns; 15x62 + 0...70 = 1000
            rowIndex = thread_info->thread_id * (thread_info-1)->num_of_columns_to_calculate + i; 
        }
        for(int j = 0; j < MATRIX_COL; ++j){ 
            // Entering critical section.
            pthread_mutex_lock(&csMutex);
            resultA[rowIndex][0] += matrixA[i][j] * vectorA[j][0];
            // Leaving critical section.
            pthread_mutex_unlock(&csMutex);
        }
    }

    // Every thread will calculate a part of the resultB = vectorB * vectorB_transpose 
    for(int i = 0; i < thread_info->num_of_columns_to_calculate; ++i){
        // Thread_(info->index) will calculate (info->num_of_columns_to_calculate) columns
        int rowColumnIndex = thread_info->thread_id * thread_info->num_of_columns_to_calculate + i;

        if(thread_info->thread_id == NUM_THREADS-1){  // For the remaining columns
            // use previous ones number of columns; 15x62 + 0...70 = 1000
            rowColumnIndex = thread_info->thread_id * (thread_info-1)->num_of_columns_to_calculate + i; 
            // cout << "Thread id " << thread_info->thread_id << " rowColumnIndex " << rowColumnIndex << endl;
        }
        pthread_mutex_lock(&csMutex);
        // since 1x1000 * 1000x1 = 1x1, we can use resultB[0][0] for all threads
        resultB[0][0] += vectorB[0][rowColumnIndex] * vectorBT[rowColumnIndex][0];
        pthread_mutex_unlock(&csMutex);
    }
    barrier();
    cout << "Thread passed the barrier (ID: " << thread_info->thread_id << ")\n";
    cout << search_info.numbers[0] << " numbers to search\n";

    int search_array[5];
    for(int i=0; i<5; i++)
        search_array[i] = search_info.numbers[i];
    search_array[1] = result_sum_array[65][0];
    
    // searching 5 numbers 2 of them not in the result_sum_array both linear and binary search
    for (int i = 0; i < 5; ++i) {
        int foundIndex = -1;
        // since matrix is 1000x1, num_of_columns_to_calculate is actually num of rows
        for (int j = 0; j < thread_info->num_of_columns_to_calculate; ++j) {
            int rowIndex = thread_info->thread_id * thread_info->num_of_columns_to_calculate + j;    
            if(thread_info->thread_id == NUM_THREADS-1){  // For the remaining rows
                // use previous ones number of columns; 15x62 + 0...70 = 1000
                rowIndex = thread_info->thread_id * (thread_info-1)->num_of_columns_to_calculate + i; 
            }
            // if element is found, break the loop
            if (result_sum_array[rowIndex][0] == search_array[i]) {
                foundIndex = j;
                break;
            }
        }
        if (foundIndex == -1){ 
            cout << "Linear Search: " << search_array[i] << " not found\n";
        }
        else {
            cout << "Linear Search: " << search_array[i] << " found at index " << foundIndex << endl;
        }
    } 

    pthread_exit(NULL);
}

void barrier(){
    pthread_mutex_lock(&barrierMutex);
    ++part1FinishedThreads;
    while(true){ // Not busy waiting. Using conditional variable + mutex for monitoring
        if(part1FinishedThreads == NUM_THREADS + 1 /* Number of threads */){
            part1FinishedThreads = 0;
            pthread_cond_broadcast(&barrierCond); // signal to all threads to wake them up
            break;
        }
        else{
            pthread_cond_wait(&barrierCond, &barrierMutex);
            break;
        }
    }
    pthread_mutex_unlock(&barrierMutex);
}
