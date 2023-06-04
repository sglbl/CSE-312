#include "../include/memory.h"
#include "../include/info.h"
#include "../include/utils.h"
#include "../include/externs.h"
#include <cstdlib>
#include <random>
using namespace std;


Memory::Memory()
{
    // The virtual memory memory stores virtual memory addresses.
    memorySize = 0;
    memory = nullptr;
}

void Memory::setElement(int index, int value)
{
    memory[index] = value;
}

int Memory::getElement(int index)
{
    return memory[index];
}

void Memory::setMemorySize(int size)
{
    memorySize = size;
    memory = (int*)malloc(size * sizeof(int));
}

int Memory::getMemorySize()
{
    return memorySize;
}

int* Memory::getMemory()
{
    return memory;
}

void Memory::setMemory(int* memory)
{
    this->memory = memory;
}

void Memory::fillMemoryWithRandoms(int* table, int table_size){
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

/****************
 * VIRTUAL MEMORY
 */
// make constructor is like base class
VirtualMemory::VirtualMemory(){
    setMemorySize(0);
}

VirtualMemory::VirtualMemory(int size){
    setMemorySize(size);
}

/****************
 * PHYSICAL MEMORY
 */
// make constructor is like base class
PhysicalMemory::PhysicalMemory(){
    setMemorySize(0);
}
PhysicalMemory::PhysicalMemory(int size){
    setMemorySize(size);
}

void PhysicalMemory::addPage(PageTableEntry page, int* page_elements){
    // add page to physical memory
    int page_frame_number = page.page_frame_number;
    int size_of_frame = information.getSizeOfFrame();
    for(int i = 0; i < size_of_frame; i++){
        setElement(page_frame_number * size_of_frame + i, page_elements[i]);
    }
    // // update page table
    // setPTE(page_table, page_frame_number, page_frame_number, 1, 0, 0);
    // cout << "Pfn" << page_frame_number << endl;
    setPTE_present(page_table, page.page_table_index, 1);
    setPTE_pfn(page_table, page.page_table_index, page_frame_number);
    cout << "[INFO: Page added to physical memory since it was not present]\n";
}

bool PhysicalMemory::isPagePresent(int page_frame_number){
    return getPTE(page_table, page_frame_number).present;
}

void PhysicalMemory::removePage(int page_frame_number){
    // check if page is present
    if (isPagePresent(page_frame_number)){
        // remove page from physical memory
        int size_of_frame = information.getSizeOfFrame();
        for(int i = 0; i < size_of_frame; i++){
            setElement(page_frame_number * size_of_frame + i, 0);
            // virtual_memory.setElement(page_frame_number * size_of_frame + i, 0);
        }
        // update page table
        setPTE(page_table, page_frame_number, page_frame_number, 0, 0, 1);
        cout << "[INFO: Page removed from physical memory]\n";
    }
    else{
        cout << "[INFO: Page not present in physical memory so it cannot be removed]\n";
    }

}

void PhysicalMemory::printPagesInPhysicalMemory(){
    int size_of_frame = information.getSizeOfFrame();
    int number_of_frames = information.getSizeOfPhysicalMemory() / size_of_frame;
    int* memory = getMemory();
    for(int i = 0; i < number_of_frames; i++){
        cout << "Frame " << i << " : "; 
        for(int j = 0; j < size_of_frame; j++){
            cout << memory[i * size_of_frame + j] << " ";
        }
        cout << endl;
    }
}

int PhysicalMemory::getEmptyPageFrame(){
    // check frame numbers of all pte's, return first empty one
    int size_of_frame = information.getSizeOfFrame();
    int number_of_frames = information.getSizeOfPhysicalMemory() / size_of_frame;
    for(int i = 0; i < number_of_frames; i++){
        int pte_frame_no = getPTE(page_table, i).page_frame_number;
        if (pte_frame_no == -1)
            return i;
    }
    
    return -1;
}

bool PhysicalMemory::checkIsFull(){
    int size_of_frame = information.getSizeOfFrame();
    int number_of_frames = information.getSizeOfPhysicalMemory() / size_of_frame;
    for(int i = 0; i < number_of_frames; i++){
        int pte_present = getPTE(page_table, i).present;
        if (!pte_present)
            return false;
    }
    return true;
}
