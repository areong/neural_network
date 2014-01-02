#include "CascadeCorrelationNeuralNetwork.h"

#include <iostream>
#include "../data/Data.h"
#include "activationfunction/HyperbolicTangent.h"
#include "neuronadditionaldata/NeuronAdditionalData.h"
#include "NeuralNetwork.h"
#include "Neuron.h"
using namespace std;

CascadeCorrelationNeuralNetwork::CascadeCorrelationNeuralNetwork() {
    neuralNetwork = new NeuralNetwork();
    activationFunction = new HyperbolicTangent();
    learningRate = 1;
    maxNumHiddenLayers = 0;
    numTrainingCycles = 0;
    patience = 0;
    trainingData = 0;
    currentIndexTrainingData = 0;
    currentIndexTrainingDataForAddingHiddenLayer = 0;
    correctOutputs = new List<double>();
    isTheFirstCycle = true;
}

CascadeCorrelationNeuralNetwork::~CascadeCorrelationNeuralNetwork() {
    delete neuralNetwork;
    delete activationFunction;
    delete correctOutputs;
}

void CascadeCorrelationNeuralNetwork::initialize(int numInputs, int numOutputs) {
    // Input layer.
    for (int i = 0; i < numInputs; i++) {
        // Create Neuron.
        Neuron *neuron = new Neuron();
        // Do not needs activation function and additional data.
        // Add the Neuron.
        neuralNetwork->addInputNeuron(neuron);
    }
    // Output layer.
    for (int i = 0; i < numOutputs; i++) {
        // Create Neuron.
        Neuron *neuron = new Neuron();
        // Set activation function.
        neuron->setActivationFunction(activationFunction);
        // Add additional data.
        NeuronAdditionalData *additionalData = new NeuronAdditionalData();
        additionalData->setNumWeights(numInputs);
        neuron->setAdditionalData(additionalData);
        // Link to all input Neurons.
        neuralNetwork->linkNeuronInputsToAllInputNeurons(neuron);

        // Set weights randomly.
        neuron->setWeightsRandomly();

        // Add the Neuron.
        neuralNetwork->addOutputNeuron(neuron);

        // List of correct outputs.
        correctOutputs->add(0);
    }
}

void CascadeCorrelationNeuralNetwork::setLearningRate(double learningRate) {
    this->learningRate = learningRate;
}

void CascadeCorrelationNeuralNetwork::setMaxNumHiddenLayers(int numLayers) {
    maxNumHiddenLayers = numLayers;
}

void CascadeCorrelationNeuralNetwork::setPatience(int numTrainingCycles) {
    patience = numTrainingCycles;
}

void CascadeCorrelationNeuralNetwork::setTrainingData(Data *trainingData) {
    this->trainingData = trainingData;
}

void CascadeCorrelationNeuralNetwork::run() {
    bool goalReached = false;
    while (!goalReached) {
        // If within patience.
        if (numTrainingCycles < patience) {
            feedInputs(trainingData, currentIndexTrainingData);
            setCorrectOutputs(trainingData, currentIndexTrainingData);
            feedForward();
            backPropagate();
            
            numTrainingCycles += 1;
            currentIndexTrainingData = (currentIndexTrainingData + 1) % trainingData->getNumItems();
            cout << currentIndexTrainingData << endl;
        }
        // Out of patience.
        else {
            // If do not reach maximum number of hidden layers.
            if (neuralNetwork->getNumHiddenLayers() < maxNumHiddenLayers) {
                addHiddenLayer();
                numTrainingCycles = 0;
            }
            // Else finish.
            else {
                goalReached = true;
            }
        }
    }
}

double CascadeCorrelationNeuralNetwork::calculateCorrectRate(Data *data) {
    int countCorrect = 0;
    // For each data item.
    for (int i = 0; i < data->getNumItems(); i++) {
        // Calculate output.
        feedInputs(data, i);
        feedForward();

        // Find output Neuron with the largest output.
        int outputMax = 0;
        int indexChosenOutputNeuron = 0;
        for (int j = 0; j < neuralNetwork->getNumOutputNeurons(); j++) {
            Neuron *outputNeuron = neuralNetwork->getOutputNeuron(j);
            if (outputNeuron->getStoredOutputWithoutCalculation() > outputMax) {
                outputMax = outputNeuron->getStoredOutputWithoutCalculation();
                indexChosenOutputNeuron = j;                   
            }
        }

        cout << i << '\t' << indexChosenOutputNeuron + 1 << endl;

        // If correct.
        if (indexChosenOutputNeuron + 1 == data->get_y_byIndex(i))
            countCorrect += 1;
    }
    return (double)countCorrect / (double)(data->getNumItems());
}

/*
    Private:
 */

void CascadeCorrelationNeuralNetwork::feedInputs(Data *data, int index) {
    for (int i = 0; i < neuralNetwork->getNumInputNeurons(); i++)
        neuralNetwork->getInputNeuron(i)->setOutput(
            data->get_x_i_byIndexAnd_i(index, i));
}

void CascadeCorrelationNeuralNetwork::setCorrectOutputs(Data *data, int index) {
    setCorrectOutputsForMLFinal(data, index);
}

void CascadeCorrelationNeuralNetwork::setCorrectOutputsForMLFinal(Data *data, int index) {
    int correct_y = trainingData->get_y_byIndex(currentIndexTrainingData);
    for (int i = 0; i < neuralNetwork->getNumOutputNeurons(); i++) {
        if (i == correct_y - 1)
            correctOutputs->set(i, 1);
        else
            correctOutputs->set(i, -1);
    }
}

void CascadeCorrelationNeuralNetwork::feedForward() {
    // Hidden layers.
    for (int indexHidden = 0; indexHidden < neuralNetwork->getNumHiddenLayers(); indexHidden++)
        for (int i = 0; i < neuralNetwork->getNumHiddenNeuronsInOneLayer(indexHidden); i++)
            neuralNetwork->getHiddenNeuron(indexHidden, i)->calculateOutput();
    // Output layer.
    for (int i = 0; i < neuralNetwork->getNumOutputNeurons(); i++)
        neuralNetwork->getOutputNeuron(i)->calculateOutput();
}

void CascadeCorrelationNeuralNetwork::backPropagate() {
    backPropagateByQuickprop();
}

void CascadeCorrelationNeuralNetwork::backPropagateByTraditionalBackprop() {
    // Only one layer update is needed.
    // For each output Neuron.
    for (int j = 0; j < neuralNetwork->getNumOutputNeurons(); j++) {
        Neuron *outputNeuron = neuralNetwork->getOutputNeuron(j);
        // Calculate y - y_true.
        double diff = outputNeuron->getStoredOutputWithoutCalculation() - 
                        correctOutputs->get(j);
        //cout << "s: " << outputNeuron->getStoredWeightedSumWithoutCalculation() << endl;
        //cout << "y: " << outputNeuron->getStoredOutputWithoutCalculation() << endl;
        //cout << "c: " << correctOutputs->get(j) << endl;
            
        // For each front Neuron.
        for (int i = 0; i < outputNeuron->getNumFrontNeurons(); i++) {
            // Output of front Neuron
            double outputFront = outputNeuron->getFrontNeuron(i)->getStoredOutputWithoutCalculation();
            // Calculate dE/dw_i
            double dEdw = diff * 
                            outputNeuron->getActivationFunction()->derivative(outputFront) *
                            outputFront;
            // Calculate delta w_i
            double dw = -1 * learningRate * dEdw;

            //cout << i << '\t' << outputFront << '\t' << diff << '\t' << dEdw << '\t' << dw << endl;

            // Update weight.
            outputNeuron->setWeight(i, outputNeuron->getWeight(i) + dw);
        }

        // Threshold.
        // Output of front Neuron
        double outputFront = 1;
        // Calculate dE/dw_i
        double dEdw = diff * 
                      outputNeuron->getActivationFunction()->derivative(outputFront) *
                      outputFront;
        // Calculate delta w_i
        double dw = -1 * learningRate * dEdw;

        //cout << outputFront << '\t' << dEdw << '\t' << dw << endl;

        // Update weight.
        outputNeuron->setThreshold(outputNeuron->getThreshold() + dw);

        // End of each output Neuron.
    }
}

void CascadeCorrelationNeuralNetwork::backPropagateByQuickprop() {
    // Only one layer update is needed.
    // If is the first cycle, use normal backpropagation.
    if (isTheFirstCycle) {
        // For each output Neuron.
        for (int j = 0; j < neuralNetwork->getNumOutputNeurons(); j++) {
            Neuron *outputNeuron = neuralNetwork->getOutputNeuron(j);
            // Calculate y - y_true.
            double diff = outputNeuron->getStoredOutputWithoutCalculation() - 
                          correctOutputs->get(j);
            
            // For each front Neuron.
            for (int i = 0; i < outputNeuron->getNumFrontNeurons(); i++) {
                // Output of front Neuron
                double outputFront = outputNeuron->getFrontNeuron(i)->getStoredOutputWithoutCalculation();
                // Calculate dE/dw_i
                double dEdw = diff * 
                              outputNeuron->getActivationFunction()->derivative(outputFront) *
                              outputFront;
                // Calculate delta w_i
                double dw = -1 * learningRate * dEdw;

                //cout << i << '\t' << outputFront << '\t' << diff << '\t' << dEdw << '\t' << dw << endl;

                // Update weight.
                outputNeuron->setWeight(i, outputNeuron->getWeight(i) + dw);

                if (dw != 0) {
                    // Store to additional data.
                    outputNeuron->getAdditionalData()->setdEdwLastCycle(i, dEdw);
                    outputNeuron->getAdditionalData()->setdwLastCycle(i, dw);
                }
            }

            // Threshold.
            // Output of front Neuron
            double outputFront = 1;
            // Calculate dE/dw_i
            double dEdw = diff * 
                            outputNeuron->getActivationFunction()->derivative(outputFront) *
                            outputFront;
            // Calculate delta w_i
            double dw = -1 * learningRate * dEdw;

            //cout << i << '\t' << outputFront << '\t' << diff << '\t' << dEdw << '\t' << dw << endl;

            // Update weight.
            outputNeuron->setThreshold(outputNeuron->getThreshold() + dw);

            if (dw != 0) {
                // Store to additional data.
                outputNeuron->getAdditionalData()->setdEdThresholdLastCycle(dEdw);
                outputNeuron->getAdditionalData()->setdThresholdLastCycle(dw);
            }

            // End of each output Neuron.
        }
        isTheFirstCycle = false;
    }
    // Not the first cycle, use Quickprop.
    else {
        // For each output Neuron.
        for (int j = 0; j < neuralNetwork->getNumOutputNeurons(); j++) {
            Neuron *outputNeuron = neuralNetwork->getOutputNeuron(j);
            // Calculate y - y_true.
            double diff = outputNeuron->getStoredOutputWithoutCalculation() - 
                          correctOutputs->get(j);

            // For each front Neuron.
            for (int i = 0; i < outputNeuron->getNumFrontNeurons(); i++) {
                // Output of front Neuron
                double outputFront = outputNeuron->getFrontNeuron(i)->getStoredOutputWithoutCalculation();
                // Calculate dE/dw_i
                double dEdw = diff * 
                              outputNeuron->getActivationFunction()->derivative(outputFront) *
                              outputFront;
                // Calculate delta w_i
                double temp = outputNeuron->getAdditionalData()->getdEdwLastCycle(i) - dEdw;
                if (temp == 0)
                    temp = 1;
                double dw = dEdw / temp *
                            outputNeuron->getAdditionalData()->getdwLastCycle(i);

                //cout << diff << '\t' << i << '\t' << dEdw << '\t' << dw << endl;
                
                // Update weight.
                outputNeuron->setWeight(i, outputNeuron->getWeight(i) + dw);

                if (dw != 0) {
                    // Store to additional data.
                    outputNeuron->getAdditionalData()->setdEdwLastCycle(i, dEdw);
                    outputNeuron->getAdditionalData()->setdwLastCycle(i, dw);
                }
            }

            // Threshold.
            // Output of front Neuron
            double outputFront = 1;
            // Calculate dE/dw_i
            double dEdw = diff * 
                            outputNeuron->getActivationFunction()->derivative(outputFront) *
                            outputFront;
            // Calculate delta w_i
            double temp = outputNeuron->getAdditionalData()->getdEdThresholdLastCycle() - dEdw;
            if (temp == 0)
                temp = 1;
            double dw = dEdw / temp *
                        outputNeuron->getAdditionalData()->getdThresholdLastCycle();

            //cout << diff << '\t' << i << '\t' << dEdw << '\t' << dw << endl;
                
            // Update weight.
            outputNeuron->setThreshold(outputNeuron->getThreshold() + dw);

            if (dw != 0) {
                // Store to additional data.
                outputNeuron->getAdditionalData()->setdEdThresholdLastCycle(dEdw);
                outputNeuron->getAdditionalData()->setdThresholdLastCycle(dw);
            }
        }
    }
}
 
void CascadeCorrelationNeuralNetwork::addHiddenLayer() {
    // Create a Neuron.
    Neuron *neuron = new Neuron();
    neuron->setActivationFunction(activationFunction);
    NeuronAdditionalData *additionalData = new NeuronAdditionalData();
    additionalData->setNumWeights(neuralNetwork->getNumInputNeurons());
    neuron->setAdditionalData(additionalData);
    // Link to all existing Neurons except for output Neurons.
    neuralNetwork->linkNeuronInputsToAllInputNeurons(neuron);
    for (int i = 0; i < neuralNetwork->getNumHiddenLayers(); i++)
        neuralNetwork->linkNeuronInputsToAllNeuronsInOneHiddenLayer(i, neuron);    
    neuron->setWeightsRandomly();

    // Calculate V_average and E_o_average.
    double V_average = 0;
    List<double> *E_o_average = new List<double>();
    for (int i = 0; i < neuralNetwork->getNumOutputNeurons(); i++)
        E_o_average->add(0);
    for (int i = 0; i < trainingData->getNumItems(); i++) {
        feedInputs(trainingData, i);
        setCorrectOutputs(trainingData, i);
        feedForward();
        V_average += neuron->getOutput();
        cout << "neuron->getOutput(): " << neuron->getOutput() << endl;
        for (int j = 0; j < neuralNetwork->getNumOutputNeurons(); j++)
            E_o_average->set(j, 
                    E_o_average->get(j) + correctOutputs->get(j) -
                    neuralNetwork->getOutputNeuron(j)->getStoredOutputWithoutCalculation());
    }
    V_average /= trainingData->getNumItems();
    for (int i = 0; i < neuralNetwork->getNumOutputNeurons(); i++)
        E_o_average->set(i, E_o_average->get(i) / trainingData->getNumItems());

    cout << "V_average: " << V_average << endl;
    for (int i = 0; i < neuralNetwork->getNumOutputNeurons(); i++)
        cout << "E_o_average->get(" << i << "): " << E_o_average->get(i) << endl;

    // Calculate S.
    double S = 0;
    for (int i = 0; i < neuralNetwork->getNumOutputNeurons(); i++) {
        double innerSum = 0;
        for (int j = 0; j < trainingData->getNumItems(); j++) {
            feedInputs(trainingData, j);
            setCorrectOutputs(trainingData, j);
            feedForward();
            innerSum += (neuron->getOutput() - V_average) *
                        (neuralNetwork->getOutputNeuron(i)->getStoredOutputWithoutCalculation() -
                         E_o_average->get(i));
        }
        if (innerSum < 0)
            innerSum *= -1;
        S += innerSum;
        cout << "S i: " << i << endl;
        cout << "innerSum" << innerSum << endl;
    }

    cout << "S: " << S << endl;
}

