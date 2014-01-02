#include "NeuronAdditionalDataQuickprop.h"

NeuronAdditionalDataQuickprop::NeuronAdditionalDataQuickprop() {
    dEdwLastCycle = new List<double>();
    dwLastCycle = new List<double>();
}

NeuronAdditionalDataQuickprop::~NeuronAdditionalDataQuickprop() {
    delete dEdwLastCycle;
    delete dwLastCycle;
}

void NeuronAdditionalDataQuickprop::setNumWeights(int numWeights) {
    dEdwLastCycle->clear();
    dwLastCycle->clear();
    for (int i = 0; i < numWeights; i++) {
        dEdwLastCycle->add(0);
        dwLastCycle->add(0);
    }
}

double NeuronAdditionalDataQuickprop::getdEdwLastCycle(int index) {
    return dEdwLastCycle->get(index);
}

void NeuronAdditionalDataQuickprop::setdEdwLastCycle(int index, double dEdw) {
    dEdwLastCycle->set(index, dEdw);
}

double NeuronAdditionalDataQuickprop::getdwLastCycle(int index) {
    return dwLastCycle->get(index);
}

void NeuronAdditionalDataQuickprop::setdwLastCycle(int index, double dw) {
    dwLastCycle->set(index, dw);
}
