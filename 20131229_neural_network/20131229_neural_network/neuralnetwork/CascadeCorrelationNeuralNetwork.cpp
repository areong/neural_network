#include "CascadeCorrelationNeuralNetwork.h"

#include "../data/Data.h"
#include "activationfunction/HyperbolicTangent.h"
#include "neuronadditionaldata/NeuronAdditionalData.h"
#include "NeuralNetwork.h"
#include "Neuron.h"

CascadeCorrelationNeuralNetwork::CascadeCorrelationNeuralNetwork() {
    neuralNetwork = new NeuralNetwork();
    activationFunction = new HyperbolicTangent();
    learningRate = 0.1;
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
        neuron->setAdditionalData(additionalData);
        // Link to all input Neurons.
        neuralNetwork->linkNeuronInputsToAllInputNeurons(neuron);
        // Add the Neuron.
        neuralNetwork->addOutputNeuron(neuron);
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
            setCorrectOutputs();
            feedForward();
            backPropagate();
            
            numTrainingCycles += 1;
            currentIndexTrainingData = (currentIndexTrainingData + 1) % trainingData->getNumItems();
        }
        // Out of patience.
        else {
            // If do not reach maximum number of hidden layers.
            if (neuralNetwork->getNumHiddenLayers() < maxNumHiddenLayers) {
                addHidderLayer();
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
        int indexChosenOutputNeuron;
        for (int j = 0; j < neuralNetwork->getNumOutputNeurons(); j++) {
            Neuron *outputNeuron = neuralNetwork->getOutputNeuron(j);
            if (outputNeuron->getStoredOutputWithoutCalculation() > outputMax) {
                outputMax = outputNeuron->getStoredOutputWithoutCalculation();
                indexChosenOutputNeuron = j;                   
            }
        }

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

void CascadeCorrelationNeuralNetwork::setCorrectOutputs() {
    setCorrectOutputsForMLFinal();
}

void CascadeCorrelationNeuralNetwork::setCorrectOutputsForMLFinal() {
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

                // Update weight.
                outputNeuron->setWeight(i, outputNeuron->getWeight(i) + dw);

                // Store to additional data.
                outputNeuron->getAdditionalData()->setdEdwLastCycle(i, dEdw);
                outputNeuron->getAdditionalData()->setdwLastCycle(i, dw);
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
                double dw = dEdw /
                            (outputNeuron->getAdditionalData()->getdEdwLastCycle(i) - dEdw) *
                            outputNeuron->getAdditionalData()->getdwLastCycle(i);

                // Update weight.
                outputNeuron->setWeight(i, outputNeuron->getWeight(i) + dw);

                // Store to additional data.
                outputNeuron->getAdditionalData()->setdEdwLastCycle(i, dEdw);
                outputNeuron->getAdditionalData()->setdwLastCycle(i, dw);
            }
        }
    }
}
 
void CascadeCorrelationNeuralNetwork::addHidderLayer() {
}

