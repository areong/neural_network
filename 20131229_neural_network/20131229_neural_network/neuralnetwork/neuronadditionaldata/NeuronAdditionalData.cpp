#include "NeuronAdditionalData.h"

NeuronAdditionalData::NeuronAdditionalData() {
    dEdwLastCycle = new List<double>();
    dwLastCycle = new List<double>();
}

NeuronAdditionalData::~NeuronAdditionalData() {
    delete dEdwLastCycle;
    delete dwLastCycle;
}

void NeuronAdditionalData::setNumWeights(int numWeights) {
    dEdwLastCycle->clear();
    dwLastCycle->clear();
    for (int i = 0; i < numWeights; i++) {
        dEdwLastCycle->add(0);
        dwLastCycle->add(0);
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
