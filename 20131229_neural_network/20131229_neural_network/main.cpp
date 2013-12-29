#include <cstdlib>

#include "file/FileReader.h"
#include "data/Data.h"

int main(int argc, char *argv[]) {

    char *fileNameTrain = "res/ml2013final_train_small.dat";

    FileReader fileReader;
    Data dataTrain;
    fileReader.setNumLine(196);
    fileReader.read(fileNameTrain, &dataTrain);

    system("pause");
    return 0;
}