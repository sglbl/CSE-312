#ifndef UTILS_H
#define UTILS_H
using namespace std;

// typedef struct LinkedList{
//     int page_frame;
//     int reference_bit;
//     struct LinkedList *next;
// } LinkedList;

#define EMPTY_BLOCK -1
#define FILE_NAME_SIZE 8
#define EXTENSION_SIZE 3
#define DIRECTORY_ENTRY_SIZE 32

#define IS_EMPTY 7
#define IS_DIR 8
#define IS_FILE 9

typedef struct FATDirectoryEntry{
    char file_name[FILE_NAME_SIZE];
    char file_extension[EXTENSION_SIZE];
    char file_attributes;
    char reserved[10];
    unsigned short time;
    unsigned short date;
    unsigned short first_block_no; 
    unsigned int file_size; // file size in bytes
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

typedef struct ParsedPath{
    string first_part_name;
    string last_part_name;
    string parent_dir_path;
    string full_path;
}ParsedPath;

enum FileOperators{
    mkdir_e,
    write_e,
    dir_e,
    del_e,
    dumpe2fs_e,
    read_e,
    cmp_e,
    unknown_e
}; // e = enum

void print_error_exit(string error_message);
void part1_argument_handler(int argc, char *argv[]);
void part2_argument_handler(int argc, char *argv[], string usage);
void fat12_fs_creator();
void fat12_fs_operator();
bool write_to_file(int num, int size=1);
bool read_from_file(int size=1);
// Operator Functions (f=function)
bool mkdir_f();
bool write_f();
bool dir_f();
bool del_f();
bool dumpe2fs_f();
bool read_f();
bool cmp_f();

// Path functions
ParsedPath path_parser(string path);
void set_parsed_path_info(ParsedPath *parsed_path, string first_part_name, string last_part_name, string parent_dir_path, string full_path);

// Directory functions
int get_block_index_of_dir(string parent_directory_path);
int get_value_from_fat12(int index);


#endif // UTILS_H