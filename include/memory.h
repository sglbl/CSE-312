#ifndef MEMORY_H
#define MEMORY_H
#include "info.h"

class Memory {
    public:
        Memory();
        void setEntry(int entry, int value);
        int getEntry(int entry);
        void setMemorySize(int size);
        int getMemorySize();
        int* getMemory();
        void setMemory(int* memory);
        void fillMemoryWithRandoms(int* table, int table_size);
    private:
        int memorySize;
        int *memory;
};

class VirtualMemory : public Memory{
    public:
        VirtualMemory();
        VirtualMemory(int size);
    private:
        int var;
};

class PhysicalMemory : public Memory{
    public:
        PhysicalMemory();
        PhysicalMemory(int size);
    private:
        int var;
};

#endif