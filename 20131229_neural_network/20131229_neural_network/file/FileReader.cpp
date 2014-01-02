#include "FileReader.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "../data/Data.h"

FileReader::FileReader() {
    numLineToRead = 0;
}

FileReader::~FileReader() {

}

int FileReader::read(char *fileName, Data *data) {
    // Open file.
    ifstream file(fileName);    
    if (file.is_open()) {
        int countLine = 0;
        string line;
        while (getline(file, line)) {    
            // Each line.
            readAtEachLineFor102MLFinal(&line, data);
            countLine += 1;
            if (countLine >= numLineToRead)
                break;
        }
        file.close();
    }
    return 0;
}

void FileReader::setNumLineToRead(int numLine) {
    this->numLineToRead = numLine;
}

/* --------------------
    Private:
 * -------------------- */

void FileReader::readAtEachLineFor102MLFinal(string *line, Data *data) {
    // Start adding one item.
    data->startAddingOneItem();

    // y
    char bufferY[4];
    int numDigitY = line->find(" ");
    int y;
    if (numDigitY <= 3) {
        // In VC, if shows
        // "Function call with parameters that may be unsafe" message,
        // it is caused by calling std::string::copy.
        // Add preprocessor definitions:
        // _SCL_SECURE_NO_WARNINGS
        // in Project Properties > Configuration Properties > C/C++ >
        // Preprocessor > Preprocessor Definitions
        // and the warning will disappear.
        line->copy(bufferY, numDigitY);
        y = atoi(bufferY);

        // Add y.
        data->add_y(y);
    }

    // i:x_i
    int positionStartSpace = numDigitY;
    while(true) {
        // i
        char bufferI[7];
        int numDigitI = line->find(":", positionStartSpace + 1) - positionStartSpace - 1;
        int i;
        if (numDigitI <= 5) {
            line->copy(bufferI, numDigitI, positionStartSpace + 1);
            i = atoi(bufferI);
        }

        // x_i
        char bufferX_I[30];
        int numDigitX_I = line->find(" ", positionStartSpace + 1) - positionStartSpace - numDigitI - 2;
        double x_i;
        if (numDigitX_I <= 29) {
            line->copy(bufferX_I, numDigitX_I, positionStartSpace + numDigitI + 2);
            x_i = atof(bufferX_I);
        }

        // Add i and x_i
        data->add_i_and_x_i(i, x_i);

        // It is the last x_i component.
        if (line->find(" ", positionStartSpace + 1) == string::npos) {
            break;
        }
        // There exits next x_i component.
        else {
            // Move to next.
            positionStartSpace += numDigitI + numDigitX_I + 2;
        }
    }

    // Finish adding one item.
    data->finishAddingOneItem();
}

void FileReader::countNumLine(char *fileName) {
    numLineToRead = 0;
    
    // Open file.
    ifstream file(fileName);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            numLineToRead++;
        }
        file.close();
    }
}