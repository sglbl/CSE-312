clear &&
make &&
./bin/makeFileSystem 4 sgFileSystem.dat
./bin/fileSystemOper sgFileSystem.dat mkdir "/folder1"
./bin/fileSystemOper sgFileSystem.dat mkdir "/folder2"
./bin/fileSystemOper sgFileSystem.dat mkdir "/folder3"
./bin/fileSystemOper sgFileSystem.dat mkdir "/folder4"
./bin/fileSystemOper sgFileSystem.dat mkdir "/folder5"
./bin/fileSystemOper sgFileSystem.dat mkdir "/folder4/suleyman"
./bin/fileSystemOper sgFileSystem.dat mkdir "/folder4/ahmet"
./bin/fileSystemOper sgFileSystem.dat dir '/'
./bin/fileSystemOper sgFileSystem.dat dir '/folder4'
./bin/fileSystemOper sgFileSystem.dat dumpe2fs
