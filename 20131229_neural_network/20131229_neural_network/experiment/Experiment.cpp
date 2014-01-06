#include "Experiment.h"

#include <iostream>
#include "../data/Data.h"
#include "../neuralnetwork/CascadeCorrelationNeuralNetwork.h"
using namespace std;

Experiment::Experiment() {
    dataValidationTrain = new List<Data *>();
    dataValidationValidate = new List<Data *>();
}

Experiment::~Experiment() {
    delete network;
}

void Experiment::setNFoldCrossValidation(int numFold) {
    this->numFold = numFold;
}

void Experiment::setTrainingData(Data *dataTraining) {
    this->dataTraining = dataTraining;
    cutData();
    cout << "cutData complete" << endl;
}

void Experiment::execute() {
    network = new CascadeCorrelationNeuralNetwork();
    network->initialize(12810, 12);
    network->setMaxNumHiddenLayers(2);
    network->setPatience(160);
    network->setTrainingData(dataValidationTrain->get(0));
    cout << "network initialization complete" << endl;
    network->run();
    cout << "run complete" << endl;
    cout << "Out of sample correct rate: " << calculateCorrectRate(0);
}

/*
    Private:
 */
void Experiment::cutData() {
    // N-fold Cross validation.
    // Calculate the indices to cut.
    // Length of the list is n+1.
    List<int> *indicesToCut = new List<int>();
    for (int i = 0; i < numFold; i++)
        indicesToCut->add(dataTraining->getNumItems() * i / 5);
    indicesToCut->add(dataTraining->getNumItems());

    // Cut the training data.
    for (int i = 0; i < numFold; i++) {
        Data *dataVal = new Data();
        Data *dataNewTrain = new Data();
        // For every data item.
        for (int j = 0; j < dataTraining->getNumItems(); j++) {
            // Put into validation data.
            if (j >= indicesToCut->get(i) && j < indicesToCut->get(i + 1))
                dataVal->addDataItem(dataTraining->getDataItem(j));
            // Put into new training data.
            else
                dataNewTrain->addDataItem(dataTraining->getDataItem(j));
        }
        // Store as member variable.
        dataValidationValidate->add(dataVal);
        dataValidationTrain->add(dataNewTrain);
    }
}

double Experiment::calculateCorrectRate(int indexFold) {
    return network->calculateCorrectRate(dataValidationValidate->get(indexFold));
}
