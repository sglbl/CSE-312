#include "../include/memory.h"
#include "../include/info.h"
#include <cstdlib>
#include <random>
using namespace std;

/**
 * PAGE TABLE
 */
Memory::Memory()
{
    // The virtual memory memory stores virtual memory addresses.
    memorySize = 0;
    memory = nullptr;
}

void Memory::setEntry(int entry, int value)
{
    memory[entry] = value;
}

int Memory::getEntry(int entry)
{
    return memory[entry];
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
