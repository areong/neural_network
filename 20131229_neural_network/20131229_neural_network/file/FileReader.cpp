#include "FileReader.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "../data/Data.h"

FileReader::FileReader() {
    numLine = 0;
    isSetNumLine = false;
}

FileReader::~FileReader() {

}

int FileReader::read(char *fileName, Data *data) {
    // Count number of lines in file.
    if (!isSetNumLine)
        countNumLine(fileName);
    
    // Initialize Data object.
    data->initialize(numLine);

    // Open file.
    ifstream file(fileName);    
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            // Each line.
            readAtEachLineFor102MLFinal(&line, data);
        }
        file.close();
    }
    return 0;
}

void FileReader::setNumLine(int numLine) {
    this->numLine = numLine;
    isSetNumLine = true;
}

/* --------------------
    Private:
 * -------------------- */

void FileReader::readAtEachLineFor102MLFinal(string *line, Data *data) {
    
}

void FileReader::countNumLine(char *fileName) {
    numLine = 0;
    
    // Open file.
    ifstream file(fileName);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            numLine++;
        }
        file.close();
    }
}