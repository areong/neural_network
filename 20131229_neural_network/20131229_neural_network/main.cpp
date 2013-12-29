#include "file/FileReader.h"
#include "data/Data.h"

int main(int argc, char *argv[]) {

    char *fileNameTrain = "res/ml2013final_train.dat";

    FileReader fileReader;
    Data dataTrain;
    fileReader.storeFileIn(fileNameTrain, &dataTrain);

    return 0;
}