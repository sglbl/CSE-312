#ifdef UTILS_H_
  #define EXTERN
#else
  #define EXTERN extern
#endif

#include "info.h"

// not making static because of: multiple definition for each include
EXTERN PageTableEntry* page_table;
EXTERN VirtualMemory virtual_memory;
EXTERN PhysicalMemory physical_memory;
EXTERN PrintStatInfo print_stat_info[3];
EXTERN Info information;
EXTERN Disk disk;
