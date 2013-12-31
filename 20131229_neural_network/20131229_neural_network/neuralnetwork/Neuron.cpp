#include "Neuron.h"

#include <stdlib.h>
#include <time.h>
#include "activationfunction\HyperbolicTangent.h"

Neuron::Neuron() {
    output = 0;
    weightedSum = 0;
    threshould = 0;

    weights = new List<double>();
    weightsLocked = false;

    frontNeurons = new List<Neuron *>();

    activationFunction = HyperbolicTangent::getInstance();
}

Neuron::~Neuron() {
    delete weights;
    delete frontNeurons;
}

double Neuron::getOutput() {
    if (frontNeurons->getLength() > 0) {
        weightedSum = 0;
        for (int i = 0; i < frontNeurons->getLength(); i++)
            weightedSum += weights->get(i) * frontNeurons->get(i)->getOutput();
        output = activationFunction->activate(weightedSum + threshould);
    }
    return output;
}

double Neuron::getStoredOutputWithoutCalculation() {
    return output;
}

void Neuron::setOutput(double output) {
    this->output = output;
}

double Neuron::getStoredWeightedSumWithoutCalculation() {
    return weightedSum;
}

double Neuron::getThreshold() {
    return threshould;
}

void Neuron::setThreshold(double threshould) {
    this->threshould = threshould;
}

void Neuron::setWeightByAFrontNeuron(Neuron *neuron, double weight) {
    if (!weightsLocked){
        // Find index.
        int index = frontNeurons->getIndex(neuron);

        // If found.
        if (index >= 0)
            weights->set(index, weight);
    }
}

void Neuron::setWeightsAllZero() {
    if (!weightsLocked){
        if (weights->getLength() > 0)
            for (int i = 0; i < weights->getLength(); i++)
                weights->set(i, 0);
    }
}

void Neuron::setWeightsRandomly() {
    if (!weightsLocked){
        if (weights->getLength() > 0)
            for (int i = 0; i < weights->getLength(); i++) {
                srand(time(NULL));
                weights->set(i, rand() * 2 / RAND_MAX - 1);
            }
    }
}

void Neuron::lockWeights(bool lock) {
    weightsLocked = true;
}

bool Neuron::isWeightsLocked() {
    return weightsLocked;
}

void Neuron::addFrontNeuron(Neuron *neuron) {
    frontNeurons->add(neuron);
}

void Neuron::removeFrontNeuron(Neuron *neuron) {
    frontNeurons->remove(neuron);
}

void Neuron::removeFrontNeuronByIndex(int index) {
    frontNeurons->removeByIndex(index);
}

int Neuron::getNumFrontNeurons() {
    return frontNeurons->getLength();
}

void Neuron::setActivationFunction(IActivationFunction *function) {
    activationFunction = function;
}
