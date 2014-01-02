#ifndef FILE_FILEREADER_H_
#define FILE_FILEREADER_H_

#include <string>
using namespace std;

class Data;

class FileReader {
public:
    FileReader();
    ~FileReader();
    int read(char *fileName, Data *data);

    /*
        Set number of lines to be read in file.
     */
    void setNumLineToRead(int numLine);

private:
    void readAtEachLineFor102MLFinal(string *line, Data *data);

    /*
        Number of lines to be read in file.
     */
    int numLineToRead;
    /*
        Count the number of lines in file.
        Direct modifies the private attribute numLine.
     */
    void countNumLine(char *fileName);
};

#endif