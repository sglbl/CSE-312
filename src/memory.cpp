#include "../include/memory.h"
#include "../include/info.h"
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
        virtual_memory.setElement(page_frame_number * size_of_frame + i, page_elements[i]);
    }
    // update page table
    page_table[page_frame_number].present = 1;
    page_table[page_frame_number].referenced_bit = 1;
    page_table[page_frame_number].modified_bit = 0;
    page_table[page_frame_number].page_frame_number = page_frame_number;
    // update file
    disk.writePage(page_frame_number, page_elements);
}
