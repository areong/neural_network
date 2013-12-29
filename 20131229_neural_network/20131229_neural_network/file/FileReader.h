#ifndef FILE_FILEREADER_H_
#define FILE_FILEREADER_H_

class Data;

class FileReader {
public:
    FileReader();
    ~FileReader();
    int storeFileIn(char *fileName, Data *data);
};

#endif