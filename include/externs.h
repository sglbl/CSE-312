#ifdef UTILS_H_
  #define EXTERN
#else
  #define EXTERN extern
#endif

#include "utils.h"
#define DEBUG 1

// not making static because of: multiple definition for each include
EXTERN FATDirectoryEntry fat_directory;  
EXTERN int block_size;
EXTERN int num_of_blocks;
EXTERN string fname;
EXTERN FileOperators file_oper;
EXTERN fstream file;
EXTERN string internal_path_name;
EXTERN string fname2;
