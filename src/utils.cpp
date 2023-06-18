#include <iostream>
#include <random>
#include <pthread.h>
#include <unistd.h>
#include <fstream>
#include <cstring>
#include "../include/externs.h"
using namespace std;

// external definitions are in externs.h
FATDirectoryEntry fat_directory;   // FAT directory
fstream file; // file stream to read and write to file
FileOperators file_oper; // file operator enum (mkdir, dir, dumpe2fs)
string internal_path_name; // path to the file_system.dat file that'll get by argv for part3
string fname; // path to the file_system.dat for part2 section
string fname2; // it will get by argv for write and read operations
float block_size; // block size in bytes
int num_of_blocks; // number of blocks in the file system

void print_error_exit(string message){
    cerr << "Error: " << message;
    exit(EXIT_FAILURE);
}

/* PART 1 FUNCTIONS */
void part1_argument_handler(int argc, char *argv[]){
    if(argc!=3)
        print_error_exit("Totally there should be 2 extra parameters!\nThe usage is:\n"
            "./bin/makeFileSystem blockSize mySystem.dat");
    
    // Example: argv[1] = first argument, argv[2] = second argument, etc.

    block_size = atof(argv[1]);
    if(block_size != 0.5 && block_size != 1 && block_size != 2 && block_size != 4){ 
        print_error_exit("Block size can be 0.5, 1, 2 or 4\n");
    }
    
    cout << "Block size: " << block_size << " KB\n";
    string disk_file(argv[2]);
    fname = disk_file;
    cout << "Disk file name: " << fname << endl;
}

void fat12_fs_creator(){
    block_size = block_size * 1024; // convert from KB to bytes by multiplying 2^10	
    num_of_blocks = pow(2, 12); // 2^12 = 4096 blocks can be represented since FAT12 is used

    if (!file.is_open()) {
        print_error_exit("Failed to open file.");
    }
    /* Creating a FAT12 file system */

    // Write block size
    write_to_file_int_as_byte(block_size, 2);

    // Write number of blocks
    write_to_file_int_as_byte(num_of_blocks, 2);
    
    // Write number of free blocks
    int free_blocks = num_of_blocks - 1; // 1 block is reserved for FAT
    write_to_file_int_as_byte(free_blocks, 2);

    // Write number of files
    int num_of_files = 0;
    write_to_file_int_as_byte(num_of_files, 2);

    // Write number of directories
    int num_of_dirs = 0;
    write_to_file_int_as_byte(num_of_dirs, 2);

    // Writing END_OF_FAT value to the first block's entry in the FAT
    // The first block is reserved to store metadata [so not part of the FAT entries.]
    write_to_file_int_as_byte(END_OF_FAT, 2); 

    // write free blocks to file by giving them FREE_BLOCK code (I choose -9)
    int i=0;
    while(i < num_of_blocks - 1){ // -1 because first block is reserved for metadata
        write_to_file_int_as_byte(FREE_BLOCK, 2); // give every block FREE_BLOCK code
        i++;
    }

    i = 0;
    int max_size = num_of_blocks * block_size;
    while(i < max_size){
        write_to_file_int_as_byte(0 , 1); // fill with 0s by 1 byte
        i++;
    }

    cout << "FAT12 Information is Created and Written to File\n\n";
}

bool write_to_file_int_as_byte(int num, int size){
    // cout << "Number to write: " << num << endl;
    short number = (short) num;
    // short = 2 bytes = 16 bits
    // so writing as 2 char bytes. first 8 bits and last 8 bits
    for (int i = 0; i < size; i++) {
        char byte;
        if (i == 0)
            byte = 0xFF & (number >> 8); // first 8 bits
        else if(i == 1)
            byte = 0xFF & number; // last 8 bits
        // cout << (int)byte << " ";
        file.write(&byte, sizeof(byte));
    }
    // cout << endl;
    // std::cout << "Integer written to file successfully." << std::endl;
    // exit(0);
    return true;
}

/* PART2 FUNCTIONS */

// Helper wrapper function to use strings in switch case
FileOperators enumerator(string const& str){
    if (str == "mkdir") return FileOperators::mkdir_e;
    else if (str == "dir") return FileOperators::dir_e;
    else if (str == "write") return FileOperators::write_e;
    else if (str == "del") return FileOperators::del_e;
    else if (str == "read") return FileOperators::read_e;
    else if (str == "dumpe2fs") return FileOperators::dumpe2fs_e;
    else if (str == "cmp") return FileOperators::cmp_e;
    else return FileOperators::unknown_e;
}

    /* "Usages:
    fileSystemOper fileSystem.data mkdir “\usr”
    fileSystemOper fileSystem.data mkdir “\usr\ysa”
    fileSystemOper fileSystem.data write “\usr\ysa\file1” linuxFile.data
    fileSystemOper fileSystem.data write “\usr\file2” linuxFile.data
    fileSystemOper fileSystem.data write “\file3” linuxFile.data
    fileSystemOper fileSystem.data dir “\”
    fileSystemOper fileSystem.data del “\usr\ysa\file1”
    fileSystemOper fileSystem.data dumpe2fs
    fileSystemOper fileSystem.data read “\usr\file2” linuxFile2.data
    */

void part2_argument_handler(int argc, char *argv[], string usage_message){
    if(argc < 3)
        print_error_exit(usage_message);
    // Example: argv[1] = first argument, argv[2] = second argument, etc.
    fname = argv[1];
    cout << "Disk file name: " << fname << endl;

    file_oper = enumerator(argv[2]);
    switch(file_oper){
        case mkdir_e:
            if(argc != 4)
                print_error_exit("mkdir command should have 3 parameters!\nThe usage is:\n"
                    "./bin/makeFileSystem sgFileSystem.dat mkdir dir1");
            internal_path_name = argv[3];
            cout << "Directory name: " << internal_path_name << endl;
            break;
        
        case dir_e:
            if(argc != 4)
                print_error_exit("dir command should have 3 parameters!\nThe usage is:\n"
                    "./bin/makeFileSystem sgFileSystem.dat dir dir1");
            internal_path_name = argv[3];
            cout << "Directory name: " << internal_path_name << endl;
            break;

        case write_e:
            if(argc != 5)
                print_error_exit("write command should have 4 parameters!\nThe usage is:\n"
                    "./bin/makeFileSystem sgFileSystem.dat write dir1 linuxFile.dat");
            internal_path_name = argv[3];
            cout << "Directory name: " << internal_path_name << endl;
            fname2 = argv[4];
            cout << "Linux file name: " << fname2 << endl;
            break;

        case del_e:
            if(argc != 4)
                print_error_exit("del command should have 3 parameters!\nThe usage is:\n"
                    "./bin/makeFileSystem sgFileSystem.dat del dir1");
            internal_path_name = argv[3];
            cout << "Directory name: " << internal_path_name << endl;
            break;

        case read_e:
            if(argc != 5)
                print_error_exit("read command should have 4 parameters!\nThe usage is:\n"
                    "./bin/makeFileSystem sgFileSystem.dat read dir1 linuxFile.dat");
            internal_path_name = argv[3];
            cout << "Directory name: " << internal_path_name << endl;
            fname2 = argv[4];
            cout << "Linux file name: " << fname2 << endl;
            break;

        case dumpe2fs_e:
            if(argc != 3)
                print_error_exit("dumpe2fs command should have 2 parameters!\nThe usage is:\n"
                    "./bin/makeFileSystem sgFileSystem.dat dumpe2fs");
            break;
        
        case cmp_e:
            if(argc != 5)
                print_error_exit("cmp command should have 4 parameters!\nThe usage is:\n"
                    "./bin/makeFileSystem sgFileSystem.dat cmp dir1 linuxFile.dat");
            internal_path_name = argv[3];
            cout << "Directory name: " << internal_path_name << endl;
            fname2 = argv[4];
            cout << "Linux file name: " << fname2 << endl;
            break;

        case unknown_e:
            print_error_exit("Unknown file operation!");
            break;
    }
    
}

int read_int_from_file(int size){
    int number = 0;
    char byte;
    for (int i = size-1; i >= 0; i--) {
        file.read(&byte, sizeof(byte));
        number |= ((byte & 0xFF) << (8 * i));
    }
    return number;
}

void fat12_fs_operator(){
    // Load the file system information
    block_size = read_int_from_file(2);
    num_of_blocks = read_int_from_file(2);
    // int free_blocks = read_int_from_file(2);
    // int num_of_files = read_int_from_file(2);
    // int num_of_dirs = read_int_from_file(2);
    
    switch(file_oper){
        case mkdir_e:
            mkdir_f();
            break;
        
        case dir_e:
            dir_f();
            break;

        case write_e:
            write_f();
            break;

        case del_e:
            del_f();
            break;

        case read_e:
            read_f();
            break;

        case dumpe2fs_e:
            dumpe2fs_f();
            break;

        case cmp_e:
            cmp_f();
            break;

        case unknown_e:
            print_error_exit("Unknown file operation!");
            break;
    }

}

bool read_from_file_to_dir_buffer(char *dir_buffer, int size){
    for (int i = 0; i < size; i++) 
        file.read(&dir_buffer[i], sizeof(char));
    return true;
}

ParsedPath path_parser(string path){
    // For example if /usr/sglbl/file1 is given, then
    // last_part_name: file1, parent_dir_path: usr/sglbl, full_path: /usr/sglbl/file1
    ParsedPath parsed_path;
    const string ROOT_PATH ="/";

    if(path == ROOT_PATH){
        set_parsed_path_info(&parsed_path, "", "", ROOT_PATH, ROOT_PATH);
        return parsed_path;
    }
    
    // check if last index is /, if it is then remove it
    if(path[path.length()-1] == ROOT_PATH[0]){
        path = path.substr(0, path.length()-1); // if path is /usr/, then it will be /usr
    }

    // if only one slash is given, then it is the root directory
    int first_slash_index = path.find_first_of(ROOT_PATH);
    if(first_slash_index == -1){  // since no match is found, no parsing is needed
        set_parsed_path_info(&parsed_path, path, "", "", path);
        return parsed_path;
    }   
    int second_slash_index = path.substr(1).find_first_of(ROOT_PATH); // for /bin/, it should be 4
    int last_slash_index = path.find_last_of(ROOT_PATH);
    
    
    if(first_slash_index == last_slash_index){ // only one slash is given, so make last empty
        if(path.size() > 1){
            set_parsed_path_info(&parsed_path, path.substr(1, path.length()-1), 
                                               path.substr(1, path.length()-1), 
                                               ROOT_PATH, 
                                               path);
            return parsed_path;
        }

        set_parsed_path_info(&parsed_path, ROOT_PATH, ROOT_PATH, ROOT_PATH, ROOT_PATH);
        return parsed_path;
    }

    // if there are more than one slash, then it is not the root directory
    set_parsed_path_info(&parsed_path,
        path.substr(1, second_slash_index),
        path.substr(last_slash_index+1, path.length()-1),
        path.substr(first_slash_index+1, last_slash_index-first_slash_index-1),
        path);

    return parsed_path;
}

void set_parsed_path_info(ParsedPath *parsed_path, string first_part_name, string last_part_name, string parent_dir_path, string full_path){
    parsed_path->first_part_name = first_part_name;
    parsed_path->last_part_name = last_part_name;
    parsed_path->parent_dir_path = parent_dir_path;
    parsed_path->full_path = full_path;
}

static short int16_from_str(char *str){
    short result = 0;
    int index = 0;
    for(int i = 1; i >= 0; i--){ // for every 8 bits (2 bytes=16 bits)
        result |= ((str[index] & 0x000000FF) << (i*8));
        index++;
    }
    return result;
}

static int int32_from_str(char *str){
    int result = 0;
    int index = 0;
    for(int i = 3; i >= 0; i--){ // for every 8 bits (4 bytes=32 bits)
        result |= ((str[index] & 0x000000FF) << (i*8));
        index++;
    }
    return result;
}

static char *int16_to_str(short num, char *str){
    int str_index = 0;
    for(int i = 1; i >= 0; i--){ // for every 8 bits (2 bytes=16 bits)
        str[str_index] = (num >> (i*8)) & 0x000000FF;
        str_index++;
    }
    return str;
}

static char *int32_to_str(int num, char *str){
    int str_index = 0;
    for(int i = 3; i >= 0; i--){ // for every 8 bits (4 bytes=32 bits)
        str[str_index] = (num >> (i*8)) & 0x000000FF;
        str_index++;
    }
    // string is computed from the most significant byte 
    // to the least significant byte (right to left)
    return str;
}

short get_fat_val_from_file(short fat_idx) { // Get fat_val from the file at the given index FAT entry
    char buff[2]; // Buffer to store the FAT entry
    std::streampos offset = file.tellg();  // Store the current file position
    file.seekg(10 + fat_idx * 2);  // Seek to the position of the FAT entry  (mult by 2 because each FAT entry is 2 bytes, each dir entry 32 bytes)
    file.read(buff, sizeof(buff)); // Read the data into the buffer
    file.seekg(offset);            // Restore the original file position
    return int16_from_str(buff);   // Convert and return the short value
}

void set_fat_val_to_file(short fat_idx, short fat_val){ // Convert fat_val to string and write it to the file at the given index FAT entry
    char buff[2];
    long offset = file.tellp(); // Store the current file position
    file.seekp(10 + fat_idx * 2, std::ios_base::beg); // Seek to the position of the FAT entry
    int16_to_str(fat_val, buff);
    write_to_file_byte(buff, sizeof(buff));
    file.seekp(offset); // Restore the original file position
}

bool write_to_file_byte(char *buffer, int size){
    for (int i = 0; i < size; i++) {
        if (!file.put(buffer[i]))
            return false;
    }
    return true;
}

// bool read_from_file(char *buffer, int size){
//     for (int i = 0; i < size; i++) {
//         if (!file.get(buffer[i]))
//             return false;
//     }// erorrrrws
//     return true;
// }

// This function will be called when a new file/dir is created
short get_free_block_index(){
    short result = -1;
    std::streampos offset = file.tellg();  // Store the current file position
    file.seekg(10, ios::beg); // Seek to the position of the FAT entry
    // file.read(buff, sizeof(buff));         // Read the data into the buffer
    for(int i = 0; i < num_of_blocks; i++){
        if((short)read_int_from_file(2) == FREE_BLOCK){ // for ex convert from 65535 to -1
            result = i;
            break;
        }
    }
    file.seekg(offset);                     // Restore the original file position
    // return int16_from_str(buff);               // Convert and return the short value
    return result;
}


bool mkdir_f(){
    ParsedPath parsed_path = path_parser(internal_path_name);
    if (parsed_path.last_part_name.length() > 8){
        cout << "Error: Directory name is too long" << endl;
        return false;
    }

    char dir_entry_buffer[32];
    short prev_block_index_of_dir = get_block_index_of_dir(parsed_path.parent_dir_path);
    if(DEBUG) cout << "prev_block_index_of_dir: " << prev_block_index_of_dir << endl;
    bool is_block_created = 0; // for 4 kb every block can have 128 dir entries

    short block_index_of_dir = prev_block_index_of_dir;
    
    while (block_index_of_dir != END_OF_FAT){ // while not end of fat so that block exists
        file.seekg(START_BYTE + block_index_of_dir*block_size, std::ios::beg);
        int number_of_dir_entries_in_block = block_size / SIZE_DE;
        for(int i = 0; i < number_of_dir_entries_in_block; i++){
            file.read(dir_entry_buffer, SIZE_DE);
            if(DEBUG)  cout << "dir_entry_buffer: " << dir_entry_buffer << endl;
            directory_entry_reader_to_struct(dir_entry_buffer);
            if(fat_directory.file_name == parsed_path.last_part_name && parsed_path.last_part_name != ""){
                cout << "Directory " << parsed_path.last_part_name << " already exists" << endl;
                return false;
            }
            if(fat_directory.file_attributes == IS_EMPTY_DE){
                file.seekp(START_BYTE + block_index_of_dir * block_size + i * SIZE_DE, std::ios::beg);
                if(DEBUG) cout << "Inside loop and variables are " << block_index_of_dir << " " << i <<
                " seek position is " << START_BYTE + block_index_of_dir * block_size + i * SIZE_DE << endl;
                strncpy(fat_directory.file_name, parsed_path.last_part_name.c_str(), 8);
                fat_directory.file_attributes = IS_DIR_DE;
                fat_directory.date = 0;
                fat_directory.time = 0;
                fat_directory.first_block_no = get_free_block_index();
                fat_directory.file_size = 0;        
                set_fat_val_to_file(fat_directory.first_block_no, END_OF_FAT);        
                set_fat_val_to_file(-1, fat_directory.first_block_no); // -1 means sets as last block in the chain |al

                directory_entry_writer_to_buffer(dir_entry_buffer);
                write_to_file_byte(dir_entry_buffer, SIZE_DE);  // buffer to file
                is_block_created = 1;
                break;
            }
        }
        if(is_block_created){
            break;
        }
        prev_block_index_of_dir = block_index_of_dir;
        block_index_of_dir = get_fat_val_from_file(block_index_of_dir);
    }

    if(is_block_created == 0){
        // create new block
        short new_block_index = get_free_block_index();
        file.seekp(START_BYTE + new_block_index * block_size, std::ios::beg);
        if(DEBUG) cout << "Outside loop and variables are " << block_index_of_dir << " " <<
            " seek position is " << START_BYTE + new_block_index * block_size << endl;

        set_fat_val_to_file(new_block_index, END_OF_FAT); // set fat value of new block to END_OF_FAT
        set_fat_val_to_file(prev_block_index_of_dir, new_block_index); // set fat value of prev block to new block index
        if(DEBUG) cout << "New block index: " << new_block_index << " prev_block_index_of_dir: " << prev_block_index_of_dir << endl;

        file.seekp(START_BYTE + new_block_index * block_size, std::ios::beg);
        strncpy(fat_directory.file_name, parsed_path.last_part_name.c_str(), 8);
        fat_directory.file_attributes = IS_DIR_DE;
        fat_directory.date = 0;
        fat_directory.time = 0;
        fat_directory.first_block_no = get_free_block_index();
        fat_directory.file_size = 0;                


        directory_entry_writer_to_buffer(dir_entry_buffer);
        write_to_file_byte(dir_entry_buffer, SIZE_DE);  // buffer to file
    }

    cout << "Directory " << parsed_path.last_part_name << " created" << endl;
    return true;
}

bool dir_f(){
    cout << "Directory listing of " << internal_path_name << endl;
    // Get the block index of the directory based on the internal path name
    short block_index_of_dir = get_block_index_of_dir(internal_path_name);

    // Buffer to hold the directory entry data
    char dir_entry_buffer[SIZE_DE];

    // Loop through all the blocks of the directory
    while (block_index_of_dir != END_OF_FAT) {
        // Setting file position to the beginning of the current dir_block
        int point_to_seek = START_BYTE + block_index_of_dir * block_size;
        if(DEBUG) cout << "Block index of dir " << block_index_of_dir << ", point_to_seek: " << point_to_seek << endl;
        file.seekg(point_to_seek, ios::beg);

        // Calculate the num_of_dir_entries in the block by dividing to 32
        int number_of_dirs_in_block = block_size / SIZE_DE;
 
        // Loop through each directory entry in the block
        for (int i = 0; i < number_of_dirs_in_block; i++) {
            // Read the dir_entry data into the buffer
            file.read(dir_entry_buffer, SIZE_DE); 
            // Process the directory entry using the directory_entry_reader_to_struct function
            directory_entry_reader_to_struct(dir_entry_buffer); // read to fat_directory

            // Check if the directory entry is not empty (has valid attributes)
            if (fat_directory.file_attributes != IS_EMPTY_DE) {
                // Output the name of the directory entry (could be file name or directory name)
                cout << "[" << fat_directory.file_name << "]\n";
            }
        }
        // Get the FAT value for the next block of the directory
        block_index_of_dir = get_fat_val_from_file(block_index_of_dir);
    }

    return true;
}

bool write_f(){

    return true;
}

bool del_f(){

    return true;
}

bool read_f(){

    return true;
}

bool dumpe2fs_f(){ 
    file.seekg(0, std::ios::beg);
    printf("Blocks: %d\n", read_int_from_file(2));
    printf("Block Size: %d\n", read_int_from_file(2));
    printf("Free Blocks: %d\n", read_int_from_file(2));
    printf("Files: %d\n", read_int_from_file(2));
    printf("Directories: %d\n", read_int_from_file(2));
    return true;
}

bool cmp_f(){

    return true;
}

// Directory functions

void reset_fat_dir_entry(){
    for(int i=0; i < FILE_NAME_SIZE; i++)
        fat_directory.file_name[i] = 0;
    fat_directory.file_attributes = 0;
    fat_directory.date = 0;
    fat_directory.time = 0;
    fat_directory.first_block_no = 0;
    fat_directory.file_size = 0;
}

// Get the block index of the directory based on the internal path name
int get_block_index_of_dir(string parent_directory_path){
    short int block_index_of_dir = 0;
    // get first and rest
    ParsedPath parsed_path = path_parser(parent_directory_path);
    while(parsed_path.first_part_name.length() > 0){
        int find_flag = 0;
        short block_index = 0;
        block_index = block_index_of_dir;
         // check all blocks of the directory
        while(block_index != END_OF_FAT){
            file.seekg(START_BYTE + block_index*block_size, std::ios::beg);
            int dir_entry_index = block_size / SIZE_DE;
            // check all directory entries in the block if find the directory break and return the block index
            for(int i = 0; i < dir_entry_index; i++){   
                char dir_buffer[SIZE_DE];
                read_from_file_to_dir_buffer(dir_buffer, SIZE_DE/*32*/);
                directory_entry_reader_to_struct(dir_buffer);
                if(parsed_path.first_part_name == fat_directory.file_name 
                   && fat_directory.file_attributes == IS_DIR_DE){ // if find the directory break and return the block index
                    if(DEBUG) cout << "Dir name: " << fat_directory.file_name << endl;
                    find_flag = 1;
                    block_index_of_dir = fat_directory.first_block_no;
                    break;
                }
            }
            if (find_flag == 1) {
                break;
            }
            block_index = (short)get_fat_val_from_file(block_index);
        }
        if (find_flag == 0) {
            reset_fat_dir_entry();
            return -1;
        }
        if(parsed_path.first_part_name == parsed_path.last_part_name){
            parsed_path.last_part_name = "";
        }
        parsed_path = path_parser(parsed_path.last_part_name);
    }
    reset_fat_dir_entry();
    return block_index_of_dir;
}


int get_value_from_fat12(int index){
    // get current offset from ifstream file
    long offset = file.tellg(); // temporary save the offset
    int position_to_seek = 2*index + 10;
    file.seekg(position_to_seek, std::ios::beg);
    int number = 0;
    char byte;
    // file.read(&byte, sizeof(byte));
    for (int i = 0; i < 2; i++) {
        file.read(&byte, sizeof(byte));
        // cout << (int)byte << " ";
        number |= (byte & 0xFF) << (8 * i);
    }
    // go back to the original offset
    file.seekg(offset, std::ios::beg);
    return number;
}

void directory_entry_reader_to_struct(char *dir_buffer_32){
    // directory_table 
    // [fileName(8) fileExt(3) fileAttr(1) reserved(10) time(2) date(2) firstBlock(2) fileSize(4)] 
    // read 32 bytes from dir_buffer_32 and write to fat_directory
    int index_iter = 0;
    int i = 0;
    while(i < FILE_NAME_SIZE + EXTENSION_SIZE){
        fat_directory.file_name[i] = dir_buffer_32[index_iter];
        i++;
        index_iter++;
    }

    fat_directory.file_name[i] = '\0';
    fat_directory.file_attributes = dir_buffer_32[index_iter++];
    // cout << "File attr in reader to struct " << (int)fat_directory.file_attributes << endl;

    i = 0;
    while (i < 10) {
        // pass reserved
        i++;
        index_iter++;
    }

    i = 0;
    char temp_convert_to_int[2];
    while (i < 2) {
        temp_convert_to_int[i] = dir_buffer_32[index_iter];
        i++;
        index_iter++;
    }
    fat_directory.time = int16_from_str(temp_convert_to_int);

    i = 0;
    while (i < 2) {
        temp_convert_to_int[i] = dir_buffer_32[index_iter];
        i++;
        index_iter++;
    }
    fat_directory.date = int16_from_str(temp_convert_to_int);

    i = 0;
    while (i < 2) {
        temp_convert_to_int[i] = dir_buffer_32[index_iter];
        i++;
        index_iter++;
    }

    fat_directory.first_block_no = int16_from_str(temp_convert_to_int);

    i = 0;
    char temp_convert_to_int2[4];
    while (i < 4) {
        temp_convert_to_int2[i] = dir_buffer_32[index_iter];
        i++;
        index_iter++;
    }

    fat_directory.file_size = int32_from_str(temp_convert_to_int2); // since 4 bytes instead of 2
    // cout << "File size: " << fat_directory.file_size << endl;
    // cout << "File attr:" << (int)fat_directory.file_attributes << endl;
    // cout << "File name: " << fat_directory.file_name << endl;
    // cout << "First block no: " << fat_directory.first_block_no << endl;
}


void directory_entry_writer_to_buffer(char *dir_buffer_32){
    // directory_table [fileName(8) fileExt(3) fileAttr(1) reserved(10)
                    // time(2) date(2) firstBlock(2) fileSize(4)] 
    // write 32 bytes from directory_table_fat and write to dir_buffer_32
    int index_iter = 0;
    int i = 0;
    while(i < FILE_NAME_SIZE + EXTENSION_SIZE){ // get file name and extension and write to buffer
        dir_buffer_32[index_iter] = fat_directory.file_name[i];
        i++;
        index_iter++;
    }

    dir_buffer_32[index_iter++] = fat_directory.file_attributes; // get file attributes and write to buffer

    i = 0;
    while (i < 10) {
        // pass reserved
        i++;
        index_iter++;
    }

    i = 0;
    char temp_convert_to_int[2];
    int16_to_str(fat_directory.time, temp_convert_to_int);
    while (i < 2) {
        dir_buffer_32[index_iter] = temp_convert_to_int[i];
        i++;
        index_iter++;
    }

    i = 0;
    int16_to_str(fat_directory.date, temp_convert_to_int);
    while (i < 2) {
        dir_buffer_32[index_iter] = temp_convert_to_int[i];
        i++;
        index_iter++;
    }

    i = 0;
    int16_to_str(fat_directory.first_block_no, temp_convert_to_int);
    while (i < 2) {
        dir_buffer_32[index_iter] = temp_convert_to_int[i];
        i++;
        index_iter++;
    }

    i = 0;
    char temp_convert_to_int4[4];
    int32_to_str(fat_directory.file_size, temp_convert_to_int4);
    while (i < 4) {
        dir_buffer_32[index_iter] = temp_convert_to_int4[i];
        i++;
        index_iter++;
    }

}
