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
        Set number of lines in file.
     */
    void setNumLine(int numLine);

private:
    void readAtEachLineFor102MLFinal(string *line, Data *data);

    /*
        Number of lines in file.
     */
    int numLine;
    /*
        Whether setNumLine() is called.
     */
    bool isSetNumLine;
    /*
        Count the number of lines in file.
        Direct modifies the private attribute numLine.
     */
    void countNumLine(char *fileName);
};

#endif