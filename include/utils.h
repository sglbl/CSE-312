#ifndef UTILS_H
#define UTILS_H
using namespace std;

// typedef struct LinkedList{
//     int page_frame;
//     int reference_bit;
//     struct LinkedList *next;
// } LinkedList;

#define EMPTY_BLOCK -1

typedef struct FATDirectoryEntry{
    char file_name[8];
    char file_extension[3];
    char file_attributes;
    char reserved[10];
    unsigned short time;
    unsigned short date;
    unsigned short first_block_no; 
    unsigned int size; // file size in bytes
}FATDirectoryEntry;  // Fixed size of 32 bytes 

// File Allocation Table (FAT) structure
typedef struct FAT12{
    // FILE *filep;
    // string fname;
    int block_count;
    int block_size;
} FAT12;    
// Each entry in the FAT holds information about the allocation status of the
// corresponding data block, including whether it is free or allocated and 
// pointers to the next block in a file's chain.


void print_error_exit(string error_message);
void part1_argument_handler(int argc, char *argv[]);
void part2_argument_handler(int argc, char *argv[], string usage);
void fat12_fs_creator();
void fat12_fs_operator();
bool write_to_file(int num, int size=1);
bool read_from_file(string buffer, int size=1);

#endif // UTILS_H