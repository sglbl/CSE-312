#include <iostream>
#include <random>
#include <pthread.h>
#include <unistd.h>
#include "../include/utils.h"
#include "../include/memory.h"
#include "../include/info.h"
#include "../include/externs.h"
using namespace std;

#define MATRIX_ROW 1000
#define MATRIX_COL 3
#define VECTOR_COL 1000
#define VECTOR_ROW 3
#define RESULT_COL 1

/* matrixA * vectorA = resultA */
static int matrixA[MATRIX_ROW][MATRIX_COL] = {{0}};
static int vectorA[VECTOR_ROW][RESULT_COL] = {{0}};
static int resultA[MATRIX_ROW][RESULT_COL] = {{0}};

/* vectorB * vectorB^T = resultB  */
static int vectorB[1][VECTOR_COL] = {{0}};
static int vectorBT[VECTOR_COL][1] = {{0}}; 
static int resultB[1][1] = {0};

static int result_sum_array[MATRIX_ROW][RESULT_COL] = {{0}};
static int result_sum_vector[MATRIX_ROW * RESULT_COL + 1] = {0};

static SearchNumberInfo search_info = {
    { -2, 59, 161, 376, 1000 }, // numbers
    { false, false, false, false, false},  // is_found_linear_t1
    { false, false, false, false, false},  // is_found_binary_t2
};

static int part1FinishedThreads;
static pthread_mutex_t csMutex;
static pthread_mutex_t barrierMutex;
static pthread_cond_t barrierCond;

void print_error_exit(string message){
    cerr << message;
    exit(EXIT_FAILURE);
}

// get for virtual memory
int get(int index, string arg){
    int element = 0;
    pthread_mutex_lock(&csMutex); // enter cs

    if(arg == "clock"){
        int frame_size = information.getSizeOfFrame();
        int page_table_index = index / frame_size; // for ex 2/4096 = 0 so it is in the first page
        int page_frame_number = getPTE(page_table, page_table_index).page_frame_number;
        cout << "\nPage table index to look (index / frame size): " << index << "/" << frame_size << " = " << page_table_index << endl;
        // for ex. if we are looking for index 2, it is in the page_table index 0
        if(getPTE(page_table, page_table_index).present == 1){
            // if it is in the physical memory no need to go to disk
            // just use number in the virtual memory
            element = virtual_memory.getElement(index);
        }
        else{   
            // if it is not in the physical memory, we need to go to disk
            handle_page_fault(page_table_index, "clock"); // tell main physical memory to get the page from disk
            element = virtual_memory.getElement(index);
        }
        // int offset = index % frame_size; // for ex 2%4096 = 2 so it is in the second offset
    }

    pthread_mutex_unlock(&csMutex); // enter cs
    return element;
}

bool second_chance_algorithm(){
    bool write_to_file_flag = false;

    // checking how frequently page faults are and depending on 
    // the page faults how many times I moved from front of queue to back of queue
    
    /** INFO: second_chance_algorithm algorithm
        loaded pages are ordered in a fifo list, oldest page is at the front
        when oldest page is selected for replacement, (m is unimportant)
        if r==1 get second chance, put it end of queue and set r bit to 0 (make youngest)
        if r==0, page is replaced; set r=0; move page tail of fifo list; check next-oldest
    */
    cout << "[INFO: Applying Second Chance (SC) page replacement algorithm]\n";
    // print page table order
    cout << "Linked list order before SC: \n";
    PageTableEntry *print_iter = page_table;
    while(print_iter != NULL){
        cout << print_iter->page_table_index << " -> ";
        print_iter = print_iter->next;
    } cout << "\n";

    string table_type = information.getTableType();
    if(table_type == "regular" || table_type == "normal"){
        PageTableEntry *iter = page_table;
        PageTableEntry* prev = NULL;
        bool current_found = false;
        while(true){
            if(iter->referenced_bit == 1){ // means it is not the oldest page
                write_to_file_flag = true;
                iter->referenced_bit = 0;
            }
            else{ // means it is (one of) the oldest page
                if(prev == NULL) // if replaced page is the first page in the queue
                    page_table = iter->next;
                else 
                    prev->next = iter->next;
                break;
            }
            prev = iter;
            iter = iter->next;
            if(iter == NULL) // if not last page
                iter = page_table; 
            if (iter == page_table)
                break;
        }
        // page to be removed print
        cout << "[INFO: Page to be removed: " << iter->page_table_index << "]\n";
        
        // now we have the oldest page.         
        // write the page to be replaced to disk
        // first get page from virtual memory
        int page_table_index = iter->page_table_index;

        // int *page_elements = virtual_memory.getPage(page_table_index);
        // // then write it to disk
        // disk.writePage(page_table_index, page_elements);

        // update page table
        // int page_frame_number = iter->page_frame_number;
        // setPTE(page_table, page_table_index, page_frame_number, 0, 0, 0);


    }
    else if(table_type == "inverted"){
        cout << "todo\n";
    }


    // print page table order
    print_iter = page_table;
    cout << "Linked list order after SC: \n";
    while(print_iter != NULL){
        cout << print_iter->page_table_index << " -> ";
        print_iter = print_iter->next;
    } cout << endl;
    return write_to_file_flag;
}

void handle_page_fault(int page_table_index, string algorithm){
    cout << "\n[INFO: Page fault occured for page table index " 
         << page_table_index << ", reading from disk]\n";
    // tell main memory to get the page from disk
    PageTableEntry page_table_entry = disk.getPage(page_table_index);
    int *page_elements = disk.getPageFrameElements(page_table_index);
    
    // if(page_table_entry.page_frame_number > information.getSizeOfFrame()){
    if(physical_memory.checkIsFull()){
        cout << "[INFO: Replacement is needed since physical frames are full. "
             "Frame size " << information.getSizeOfFrame() << "]\n";
        // exit(EXIT_SUCCESS);
        if(algorithm == "clock"){
            int write_to_file_flag_if_theres_modified = second_chance_algorithm();
            if (write_to_file_flag_if_theres_modified)
                disk.writeToFile(information.getSizeOfVirtualMemory(), information.getSizeOfFrame());
            else
                cout << "[INFO: No need to write to file since there is no modified page]\n";
            physical_memory.printPagesInPhysicalMemory();
        }
    }

    // update page table
    // setPTE_present(page_table, page_table_index, 1);
    // setPTE_pfn(page_table, page_table_index, page_table_entry.page_frame_number);
    // adding that page info and memory values to physical memory
    
    int frame_no = physical_memory.getEmptyPageFrame();
    page_table_entry.page_frame_number = frame_no;
    cout << "fRAME NO: " << frame_no << "\n";
    physical_memory.addPage(page_table_entry, page_elements);        
}

void set(int index, int value, string arg){
    pthread_mutex_lock(&csMutex); // enter cs

    if(arg == "clock"){
        int frame_size = information.getSizeOfFrame();
        int page_table_index = index / frame_size; // for ex 2/4096 = 0 so it is in the first page
        virtual_memory.setElement(index, value);
        // since it is set, we need to update the dirty bit
        setPTE_modified(page_table, page_table_index, 1);
        // if(page_table[page_table_index].modified_bit/*dirty bit*/ == 1)
        disk.writeToFile(information.getSizeOfVirtualMemory(), information.getSizeOfFrame());

        // remove the page that changed value is in
        cout << "[INFO: Removing page table index " << page_table_index << " from physical memory]\n";
        physical_memory.removePage(page_table_index);
        // update the page table

        
        // add the page that changed value is in
        // PageTableEntry page_table_entry = disk.getPage(page_table_index);
        // int *page_elements = disk.getPageFrameElements(page_table_index);
        // physical_memory.addPage(page_table_entry, page_elements);
    }

    pthread_mutex_unlock(&csMutex); // enter cs
}

void fill_physical_memory(){
    // fill physical memory with pages from disk
    for(int i = 0; i < information.getSizeOfPhysicalMemory(); ++i){
        PageTableEntry page_table_entry = disk.getPage(i);
        int *page_elements = disk.getPageFrameElements(i);
        physical_memory.addPage(page_table_entry, page_elements);
    }
}

void create_matrix_and_vectors_from_memory(int *memory){
    int index = 0;
    for(int i = 0; i < MATRIX_ROW; ++i){
        for(int j = 0; j < MATRIX_COL; ++j){
            // matrixA[i][j] = memory[index++];
            matrixA[i][j] = get(index++, "clock");
        }
    }
    for(int i = 0; i < VECTOR_ROW; ++i){
        for(int j = 0; j < RESULT_COL; ++j){
            // vectorA[i][j] = memory[index++];
            vectorA[i][j] = get(index++, "clock");
        }
    }
    for(int i = 0; i < 1; ++i){
        for(int j = 0; j < VECTOR_COL; ++j){
            // vectorB[i][j] = memory[index++];
            vectorB[i][j] = get(index++, "clock");
        }
    }

    // transpose vectorB
    for(int i = 0; i < VECTOR_COL; ++i){
        for(int j = 0; j < RESULT_COL; ++j){
            vectorBT[i][j] = vectorB[j][i];
        }
    }

}

PageTableEntry getPTE(PageTableEntry *head, int index){
    PageTableEntry *current = head;
    for(int i = 0; current != NULL; i++){
        if(i == index)
            break;
        current = current->next;
    }
    return *current;
}

PageTableEntry setPTE(PageTableEntry *head, int index, int pfn, int present, int modified, int referenced){
    PageTableEntry *current = head;
    for(int i = 0; current != NULL; i++){
        if(i == index)
            break;
        current = current->next;
    }
    current->page_frame_number = pfn;
    current->present = present;
    current->modified_bit = modified;
    current->referenced_bit = referenced;
    return *current;
}

PageTableEntry setPTE_pfn(PageTableEntry *head, int index, int bit){
    PageTableEntry *current = head;
    for(int i = 0; current != NULL; i++){
        if(i == index)
            break;
        current = current->next;
    }
    current->page_frame_number = bit;
    return *current;
}

PageTableEntry setPTE_present(PageTableEntry *head, int index, int bit){
    PageTableEntry *current = head;
    for(int i = 0; current != NULL; i++){
        if(i == index)
            break;
        current = current->next;
    }
    current->present = bit;
    return *current;
}

PageTableEntry setPTE_modified(PageTableEntry *head, int index, int bit){
    PageTableEntry *current = head;
    for(int i = 0; current != NULL; i++){
        if(i == index)
            break;
        current = current->next;
    }
    current->modified_bit = bit;
    return *current;
}

PageTableEntry setPTE_referenced(PageTableEntry *head, int index, int bit){
    PageTableEntry *current = head;
    for(int i = 0; current != NULL; i++){
        if(i == index)
            break;
        current = current->next;
    }
    current->referenced_bit = bit;
    return *current;
}

void thread_handler(){
    pthread_t threads[2];
    pthread_t main_thread; // for summation of both results

    int rc; // return code from pthread_create()
    ThreadInfo *thread_info = (ThreadInfo*)calloc(2, sizeof(ThreadInfo));

    pthread_mutex_init(&csMutex, NULL); 
    pthread_mutex_init(&barrierMutex, NULL); 
    pthread_cond_init(&barrierCond, NULL);

    // Thread info initialization
    for (int i = 0; i < 2; i++){  
        thread_info[i].thread_id =  i;
        thread_info[i].page_frame = i;
        thread_info[i].num_of_columns_to_calculate = MATRIX_ROW / 2; // 1000/2 = 500
    }

    // Main thread creation. It will calculate the summation.
    if( pthread_create(&main_thread, NULL, main_thread_job, (void*)0) != 0 ){ //if returns 0 it's okay.
        perror("pthread_create()");
        exit(EXIT_FAILURE);
    }

    // Thread creation (start threads)
    for(int t=0; t<2; t++){
        rc = pthread_create(&threads[t], NULL, thread_job,  (void*)&thread_info[t]);
        if (rc){
            cerr << "ERROR; return code from pthread_create() is " << rc << endl;
            exit(EXIT_FAILURE);
        }
    }

    // Thread join (wait for all threads to finish)
    for(int i = 0; i < 2; i++){
        if( pthread_join(threads[i], NULL) != 0 ){
            perror("pthread_join()");
            exit(EXIT_FAILURE);
        }
        else{
            cout << "Joined thread " << i << "\n";
        }
    }

    // Main thread
    if( pthread_join(main_thread, NULL) != 0 ){ // main thread is detached
        perror("pthread exiting error for main thread");
        exit(EXIT_FAILURE);
    }else{
        cout << "Detached main thread\n";
    }

    pthread_exit(NULL);
}

void* main_thread_job(void *arg){
    // remove buffering from cout
    std::cout.setf(std::ios::unitbuf);
    // cout << "Printing ResultA\n[";
    // for(int i = 0; i < MATRIX_ROW; ++i){
    //     for(int j = 0; j < RESULT_COL; ++j){
    //         cout << resultA[i][j] << " ";
    //     }
    //     cout << "]\n";
    // }

    // cout << "Printing ResultB\n[";
    // for(int i = 0; i < 1; ++i){
    //     for(int j = 0; j < 1; ++j){
    //         cout << resultB[i][j] << " ";
    //     }
    //     cout << "]\n";
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

    
    // Printing numbers to search
    cout << unitbuf << "Printing numbers to search\n";
    for(int i = 0; i < 5; ++i){
        cout << unitbuf << search_info.numbers[i] << " ";
    } cout << unitbuf << "\n";
    pthread_mutex_unlock(&csMutex);

    // cout << "Printing Result Summation Vector (1x1)\n";
    // for(int i = 0; i < 1; ++i){
    //     for(int j = 0; j < 1; ++j){
    //         cout << result_sum_vector[i] << " ";
    //     }
    //     cout << endl;
    // }


    for (int i = 0; i < 5; ++i) {
        // check for 2 thread's if found for linear search
        if(search_info.is_found_linear_t1[i]){
            cout << "Linear search for numbers[" << i << "] = " << search_info.numbers[i] << ": Found the element in the matrix\n";
        }else{
            // handle_page_fault();
            cout << "Linear search for numbers[" << i << "] = " << search_info.numbers[i] << ": Element not found in the matrix\n";
        }

        // // check for 2 thread's if found for binary search
        // if(search_info.is_found_binary_t2[i]){
        //     cout << "Binary search for numbers[" << i << "] = " << search_info.numbers[i] << ": Found the element in the matrix\n";
        // }else{
        //     cout << "Binary search for numbers[" << i << "] = " << search_info.numbers[i] << ": Element not found in the matrix\n";
        // }
    }

    pthread_exit(NULL);
}

void* thread_job(void *arg){
    // Allowing instant printing with cout without buffer
    std::cout.setf(std::ios::unitbuf);
    setvbuf(stdout, NULL, _IONBF, 0);

    ThreadInfo* thread_info = (ThreadInfo*)arg;
    cout << "Hello, My Thread ID: " << thread_info->thread_id << "\n";

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

        pthread_mutex_lock(&csMutex);
        // since 1x1000 * 1000x1 = 1x1, we can use resultB[0][0] for all threads
        resultB[0][0] += vectorB[0][rowColumnIndex] * vectorBT[rowColumnIndex][0];
        pthread_mutex_unlock(&csMutex);
    }
    barrier();
    cout << "Thread passed the barrier (ID: " << thread_info->thread_id << ")\n";

    int search_array[5];
    for(int i=0; i<5; i++)
        search_array[i] = search_info.numbers[i];
    search_array[0] = result_sum_array[0][0];
    search_array[1] = result_sum_array[20][0];

    // searching 5 numbers 2 of them not in the result_sum_array 
    // both linear and binary search

    // Linear Search (THREAD 1 JOB)
    if(thread_info->thread_id == 0){
        for (int i = 0; i < 5; ++i) {
            int foundIndex_t1 = -1;
            for (int j = 0; j < MATRIX_ROW; ++j) {
                // if element is found, break the loop
                if (result_sum_array[j][0] == search_array[i]) {
                    foundIndex_t1 = j;
                    break;
                }
            }
            if (foundIndex_t1 == -1){ 
                cout << "Linear Search: " << search_array[i] << " not found (ID: " << thread_info->thread_id << ")\n";
            }
            else {
                search_info.is_found_linear_t1[i] = true;
                cout << "Linear Search: " << search_array[i] << " found at index " << foundIndex_t1 << " (ID: " << thread_info->thread_id << ")\n";
            }
        }
    }

    // Binary Search (THREAD 2 JOB)
    else if (thread_info->thread_id == 1){
        for (int i = 0; i < 5; ++i) {
            int foundIndex_t2 = -1;
            int left = 0, right = MATRIX_ROW - 1;
            while (left <= right) {
                int mid = (left + right) / 2;
                if (result_sum_array[mid][0] == search_array[i]) {
                    foundIndex_t2 = mid;
                    break;
                }
                else if (result_sum_array[mid][0] < search_array[i]) {
                    left = mid + 1;
                }
                else {
                    right = mid - 1;
                }
            }
            if (foundIndex_t2 == -1) {
                cout << "Binary Search: " << search_array[i] << " not found (ID: " << thread_info->thread_id << ")\n";
            }
            else {
                search_info.is_found_binary_t2[i] = true;
                cout << "Binary Search: " << search_array[i] << " found at index " << foundIndex_t2 << " (ID: " << thread_info->thread_id << ")\n";
            }
        }
    }

    pthread_exit(NULL);
}

void barrier(){
    pthread_mutex_lock(&barrierMutex);
    ++part1FinishedThreads;
    while(true){ // Not busy waiting. Using conditional variable + mutex for monitoring
        if(part1FinishedThreads == 2 + 1 /* 2 threads for different part of matrix + summation thread */){
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

// void memory_handler(VirtualMemory virtual_memory, PhysicalMemory physicalMemory, PageTableEntry* pageTable, Disk disk, Info information, PrintStatInfo printStatInfo[3]){
//     // memory handling to handle page faults, page replacements, disk etc.
//     int pageFaultCount = 0;
//     int diskReadCount = 0;
//     int diskWriteCount = 0;
//     int pageReplacementCount = 0;

//     // // initializing the page table
//     // for(int i=0; i<information.N; i++){
//     //     pageTable[i].virtualPageNumber = i;
//     //     pageTable[i].valid = 0;
//     //     pageTable[i].dirty = 0;
//     //     pageTable[i].page_frame_number = -1;
//     // }
// }