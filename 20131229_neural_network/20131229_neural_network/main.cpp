#include <cstdlib>
#include <iostream>
using namespace std;

#include "data/Data.h"
#include "file/FileReader.h"
#include "neuralnetwork/CascadeCorrelationNeuralNetwork.h"

int main(int argc, char *argv[]) {

    char *fileNameTrain = "res/ml2013final_train_small.dat";

    FileReader *fileReader = new FileReader();
    Data *dataTrain = new Data();
    fileReader->setNumLineToRead(10);
    fileReader->read(fileNameTrain, dataTrain);

    cout << dataTrain->getNumItems() << endl;

    CascadeCorrelationNeuralNetwork *network = new CascadeCorrelationNeuralNetwork();
    network->initialize(12810, 12);
    network->setMaxNumHiddenLayers(0);
    network->setPatience(196);
    network->setTrainingData(dataTrain);
    network->run();

    system("pause");
    return 0;
}