#include <cstdlib>
#include <iostream>
using namespace std;

#include "data/Data.h"
#include "experiment/Experiment.h"
#include "file/FileReader.h"
#include "neuralnetwork/CascadeCorrelationNeuralNetwork.h"

int main(int argc, char *argv[]) {

    char *fileNameTrain = "res/ml2013final_train.dat";

    FileReader *fileReader = new FileReader();
    Data *dataTrain = new Data();
    fileReader->setNumLineToRead(10000);
    fileReader->read(fileNameTrain, dataTrain);

    //for (int i = 0; i < dataTrain->getNumItems(); i++)
    //cout << i << '\t' << dataTrain->get_y_byIndex(i) << endl;

    cout << dataTrain->getNumItems() << endl;

    Experiment *experiment = new Experiment();
    experiment->setNFoldCrossValidation(5);
    experiment->setNeuralNetworkParameters(12810, 12);
    experiment->setExperimentParameters(0, 0,
                                        4916, 1, 10,
                                        2, 2, 1,
                                        3);
    experiment->setTrainingData(dataTrain);
    experiment->execute();

    system("pause");
    return 0;
}