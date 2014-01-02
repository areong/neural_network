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
    fileReader->setNumLine(196);
    fileReader->read(fileNameTrain, dataTrain);

    for (int i = 0; i < 10000; i++) {
        cout << i << '\t' << dataTrain->get_x_i_byIndexAnd_i(0, i) << endl;
    }

    CascadeCorrelationNeuralNetwork *network = new CascadeCorrelationNeuralNetwork();
    network->initialize(12810, 12);
    network->setMaxNumHiddenLayers(0);
    network->setPatience(196);
    network->setTrainingData(dataTrain);
    network->run();

    system("pause");
    return 0;
}