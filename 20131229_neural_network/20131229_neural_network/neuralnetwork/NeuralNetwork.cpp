#include "NeuralNetwork.h"
#include "Neuron.h"

NeuralNetwork::NeuralNetwork() {
    inputLayer = new List<Neuron *>();
    outputLayer = new List<Neuron *>();
    hiddenLayers = new List<List<Neuron *> *>();
}

NeuralNetwork::~NeuralNetwork() {
    // Delete input neurons.
    for (int i = 0; i < inputLayer->getLength(); i++)
        delete inputLayer->get(i);
    delete inputLayer;

    // Delete output neurons.
    for (int i = 0; i < outputLayer->getLength(); i++)
        delete outputLayer->get(i);
    delete outputLayer;

    // Delete hidden neurons and layers.
    for (int i = 0; i < hiddenLayers->getLength(); i++)
        for (int j = 0; j < hiddenLayers->get(i)->getLength(); j++)
            delete hiddenLayers->get(i)->get(j);
    for (int i = 0; i < hiddenLayers->getLength(); i++)
        delete hiddenLayers->get(i);
    delete hiddenLayers;
}

void NeuralNetwork::addInputNeuron(Neuron *neuron) {
    inputLayer->add(neuron);
}

void NeuralNetwork::removeInputNeuron(Neuron *neuron) {
    inputLayer->remove(neuron);
}

void NeuralNetwork::removeInputNeuronByIndex(int index) {
    inputLayer->removeByIndex(index);
}

Neuron *NeuralNetwork::getInputNeuron(int index) {
    return inputLayer->get(index);
}

int NeuralNetwork::getNumInputNeurons() {
    return inputLayer->getLength();
}

void NeuralNetwork::addOutputNeuron(Neuron *neuron) {
    outputLayer->add(neuron);
}

void NeuralNetwork::removeOutputNeuron(Neuron *neuron) {
    outputLayer->remove(neuron);
}

void NeuralNetwork::removeOutputNeuronByIndex(int index) {
    outputLayer->removeByIndex(index);
}

Neuron *NeuralNetwork::getOutputNeuron(int index) {
    return outputLayer->get(index);
}

int NeuralNetwork::getNumOutputNeurons() {
    return outputLayer->getLength();
}

void NeuralNetwork::increaseHiddenLayer() {
    List<Neuron *> *newLayer = new List<Neuron *>();
    hiddenLayers->add(newLayer);
}

void NeuralNetwork::addHiddenNeuron(int indexLayer, Neuron *neuron) {
    if (indexLayer >= 0 && indexLayer < hiddenLayers->getLength())
        hiddenLayers->get(indexLayer)->add(neuron);
}

Neuron *NeuralNetwork::getHiddenNeuron(int indexLayer, int indexNeuron) {
    if (indexLayer >= 0 && indexLayer < hiddenLayers->getLength())
        return hiddenLayers->get(indexLayer)->get(indexNeuron);
    else
        return 0;
}

int NeuralNetwork::getNumHiddenLayers() {
    return hiddenLayers->getLength();
}

int NeuralNetwork::getNumHiddenNeuronsInOneLayer(int indexLayer) {
    if (indexLayer >= 0 && indexLayer < hiddenLayers->getLength())
        return hiddenLayers->get(indexLayer)->getLength();
    else
        return 0;
}

void NeuralNetwork::linkNeuronInputsToAllInputNeurons(Neuron *neuron) {
    for (int i = 0; i < inputLayer->getLength(); i++)
        neuron->addFrontNeuron(inputLayer->get(i));
}

void NeuralNetwork::linkNeuronInputsToAllNeuronsInOneHiddenLayer(int indexLayer, Neuron *neuron) {
    if (indexLayer >= 0 && indexLayer < hiddenLayers->getLength())
        for (int i = 0; i < hiddenLayers->get(indexLayer)->getLength(); i++)
            neuron->addFrontNeuron(hiddenLayers->get(indexLayer)->get(i));
}
