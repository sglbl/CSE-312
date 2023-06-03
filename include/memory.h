#ifndef MEMORY_H
#define MEMORY_H
#include "info.h"
#include "disk.h"

class Memory {
    public:
        Memory();
        void setElement(int index, int value);
        int getElement(int index);
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
};

class PhysicalMemory : public Memory{
    public:
        PhysicalMemory();
        PhysicalMemory(int size);
        void addPage(PageTableEntry page, int* page_elements);
        void removePage(int page_frame_number);
        bool isPagePresent(int page_frame_number);
};

#endif