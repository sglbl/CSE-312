#ifndef UTILS_H
#define UTILS_H
using namespace std;

#define END_OF_FAT -1
#define FREE_BLOCK -9
#define FILE_NAME_SIZE 8
#define EXTENSION_SIZE 3
#define START_BYTE 8202 // 4096*2 (2 blocks for system) + 10 reserved
#define SIZE_DE 32

// Attirbutes for Directory Entry (DE)
#define IS_FILE_DE 2
#define IS_EMPTY_DE 0
#define IS_DIR_DE 1

typedef struct FATDirectoryEntry{
    char file_name[FILE_NAME_SIZE];
    char file_extension[EXTENSION_SIZE];
    char file_attributes;
    char reserved[10];
    unsigned short time;
    unsigned short date;
    unsigned short first_block_no; 
    unsigned int file_size; // file size as 4 bytes
}FATDirectoryEntry;  // Fixed size of 32 bytes 

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

// Error and Argument Handling Functions
void print_error_exit(string error_message);
void part1_argument_handler(int argc, char *argv[]);
void part2_argument_handler(int argc, char *argv[], string usage);
// File System Creation and Operation Functions
void fat12_fs_creator();
void fat12_fs_operator();

// Operator Functions (f=function)
bool mkdir_f();
bool write_f();
bool dir_f();
bool del_f();
bool dumpe2fs_f();
bool read_f();
bool cmp_f();

// File writing and reading helper functions
bool write_to_file_int_as_byte(int num, int size=1);
bool write_to_file_byte(char *buffer, int size);
int read_int_from_file(int size=1);
// bool read_from_file(int size=1);

// Path functions
ParsedPath path_parser(string path);
void set_parsed_path_info(ParsedPath *parsed_path, string first_part_name, string last_part_name, string parent_dir_path, string full_path);

// Directory functions
int get_block_index_of_dir(string parent_directory_path);
int get_value_from_fat12(int index);
void directory_entry_reader_to_struct(char *dir_buffer_32);
void directory_entry_writer_to_buffer(char *dir_buffer_32);


#endif // UTILS_H