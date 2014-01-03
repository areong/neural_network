#include "NeuronAdditionalData.h"

NeuronAdditionalData::NeuronAdditionalData(int numWeights) {
    dEdwLastCycle = new List<double>();
    dwLastCycle = new List<double>();
    dEdThresholdLastCycle = 0;
    dThresholdLastCycle = 0;
    setNumWeights(numWeights);
}

NeuronAdditionalData::~NeuronAdditionalData() {
    delete dEdwLastCycle;
    delete dwLastCycle;
}

void NeuronAdditionalData::setNumWeights(int numWeights) {
    dEdwLastCycle->clear();
    dwLastCycle->clear();
    for (int i = 0; i < numWeights; i++) {
        dEdwLastCycle->add(1);
        dwLastCycle->add(1);
    }
}

double NeuronAdditionalData::getdEdwLastCycle(int index) {
    return dEdwLastCycle->get(index);
}

void NeuronAdditionalData::setdEdwLastCycle(int index, double dEdw) {
    dEdwLastCycle->set(index, dEdw);
}

double NeuronAdditionalData::getdwLastCycle(int index) {
    return dwLastCycle->get(index);
}

void NeuronAdditionalData::setdwLastCycle(int index, double dw) {
    dwLastCycle->set(index, dw);
}

double NeuronAdditionalData::getdEdThresholdLastCycle() {
    return dEdThresholdLastCycle;
}

void NeuronAdditionalData::setdEdThresholdLastCycle(double dEdThreshold) {
    dEdThresholdLastCycle = dEdThreshold;
}

double NeuronAdditionalData::getdThresholdLastCycle() {
    return dThresholdLastCycle;
}

void NeuronAdditionalData::setdThresholdLastCycle(double dThreshold) {
    dThresholdLastCycle = dThreshold;
}
