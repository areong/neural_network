#include "Experiment.h"

#include <iostream>
#include <time.h>
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

void Experiment::setNeuralNetworkParameters(int numInput, int numOutput) {
    this->numInput  = numInput;
    this->numOutput = numOutput;
}

void Experiment::setExperimentParameters(int numHiddenStart, int numHiddenEnd,
                                         int patienceBase, int patienceMultiplyStart, int patienceMultiplyEnd,
                                         int numBigTrainingCyclesAddHiddenStart,  int numBigTrainingCyclesAddHiddenEnd, int numBigTrainingCyclesAddHiddenIncr,
                                         int numTimesEachParameter) {
    this->numHiddenStart = numHiddenStart;
    this->numHiddenEnd = numHiddenEnd;
    this->patienceBase = patienceBase;
    this->patienceMultiplyStart = patienceMultiplyStart;
    this->patienceMultiplyEnd = patienceMultiplyEnd;
    this->numBigTrainingCyclesAddHiddenStart = numBigTrainingCyclesAddHiddenStart;
    this->numBigTrainingCyclesAddHiddenEnd   = numBigTrainingCyclesAddHiddenEnd;
    this->numBigTrainingCyclesAddHiddenIncr  = numBigTrainingCyclesAddHiddenIncr;
    this->numTimesEachParameter = numTimesEachParameter;
}

void Experiment::setTrainingData(Data *dataTraining) {
    this->dataTraining = dataTraining;
    cutData();
    cout << "cutData complete" << endl;
}

void Experiment::execute() {
    // Change number of hidden layers.
    for (int iNumHidden = numHiddenStart; iNumHidden <= numHiddenEnd; iNumHidden++) {
        cout << "iNumHidden = " << iNumHidden << endl;

        //double foldInAverage = 0;
        //double foldOutAverage = 0;
        
        // Patience
        for (int iPatience = patienceMultiplyStart; iPatience <= patienceMultiplyEnd; iPatience++) {
            cout << "    iPatience = " << iPatience
                    << ", patience = " << patienceBase * iPatience << endl;

            // Number of big cycles training hidden neuron.
            for (int iTrainHidden = numBigTrainingCyclesAddHiddenStart;
                    iTrainHidden <= numBigTrainingCyclesAddHiddenEnd;
                    iTrainHidden += numBigTrainingCyclesAddHiddenIncr) {
                cout << "        iTrainHidden = " << iTrainHidden << endl;

                //double inAverage = 0;
                //double outAverage = 0;

                // For every validation set.
                for (int iFold = 0; iFold < numFold; iFold++) {
                    cout << "            iFold = " << iFold << endl;

                    // Do several times with the same parameters.
                    for (int i = 0; i < numTimesEachParameter; i++) {

                        cout << "                i = " << i << endl;
                        cout << "                    sec: " << clock() / (float)CLOCKS_PER_SEC
                                << " start" << endl;
                        network = new CascadeCorrelationNeuralNetwork();
                        network->initialize(numInput, numOutput);
                        network->setMaxNumHiddenLayers(iNumHidden);
                        network->setPatience(patienceBase * iPatience);
                        network->setNumBigTrainingCyclesAddHidden(iTrainHidden);
                        network->setTrainingData(dataValidationTrain->get(iFold));
                        //cout << "    network initialization complete" << endl;
                        cout << "                    sec: " << clock() / (float)CLOCKS_PER_SEC
                             << " run" << endl;
                        network->run();
                        //cout << "    run complete" << endl;
                        cout << "                    sec: " << clock() / (float)CLOCKS_PER_SEC
                             << " run finish" << endl;
                        double correct_in = network->calculateCorrectRate(dataValidationTrain->get(iFold));
                        double correct_out = calculateCorrectRate(iFold);
                        cout << "                    Correct_in  = " << correct_in << endl;
                        cout << "                    Correct_out = " << correct_out << endl;

                        delete network;

                        //inAverage += correct_in;
                        //outAverage += correct_out;

                        // No random behavior occurs at 0 hiddon layer,
                        // thus break at first run.
                        if (iNumHidden == 0)
                            break;
                    }
                    //double denom = 3;
                    //if (iNumHidden == 0)
                    //    denom = 1;
                    //cout << "            Correct_in_average  = " << inAverage / denom << endl;
                    //cout << "            Correct_out_average = " << outAverage / denom << endl;

                    //foldInAverage += inAverage / denom;
                    //foldOutAverage += outAverage / denom;                    
                }

                // No hidden neuron to train at 0 hiddon layer,
                    // thus break at first run.
                    if (iNumHidden == 0)
                        break;
            }
        }
        
        //cout << "        Correct_fold_in_average  = " << foldInAverage / (double)numFold << endl;
        //cout << "        Correct_fold_out_average = " << foldOutAverage / (double)numFold << endl;
    }
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
