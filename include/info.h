#ifndef INFO_H
#define INFO_H
#include "memory.h"
#include <string>
using namespace std;

struct PageTableEntry; // forward declaration
struct PrintStatInfo; // forward declaration

class Info {
    public:
        void argument_handler(int argc, char *argv[]);
        void fillPageTableWith0s(struct PageTableEntry** table, int table_size);
        void fillPrintStatWith0s(PrintStatInfo* print_stat_info, int table_size);
        inline int getSizeOfFrame(){ return size_of_frame; }
        inline int getFramesOfPhysicalMemory(){ return frames_of_physical_memory; }
        inline int getPagesOfVirtualMemory(){ return pages_of_virtual_memory; }
        inline int getSizeOfPhysicalMemory(){ return size_of_physical_memory; }
        inline int getSizeOfVirtualMemory(){ return size_of_virtual_memory; }
        inline std::string getPageReplacement(){ return page_replacement; }
        inline std::string getTableType(){ return table_type; }
        inline std::string getDiskFileName(){ return disk_file_name; }
    private:    
        int size_of_frame;
        int frames_of_physical_memory;
        int pages_of_virtual_memory;
        int page_table_print;
        std::string page_replacement;
        std::string table_type;
        std::string disk_file_name;
        int size_of_physical_memory;
        int size_of_virtual_memory;
};

typedef struct PageTableEntry{
	int page_frame_number;
    int present; // valid bit
    int referenced_bit; // used (access) bit
	int modified_bit; // dirty bit    
    int last_time_used; // for clock algos like second chance
    struct PageTableEntry *next;
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