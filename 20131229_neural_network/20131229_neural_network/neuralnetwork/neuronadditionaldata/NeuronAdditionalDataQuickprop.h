#ifndef NEURALNETWORK_NEURONADDITIONALDATAQUICKPROP_H_
#define NEURALNETWORK_NEURONADDITIONALDATAQUICKPROP_H_

#include "INeuronAdditionalData.h"

#include "../../utils/list/List.h"

class NeuronAdditionalDataQuickprop : public INeuronAdditionalData {
public:
    NeuronAdditionalDataQuickprop();
    ~NeuronAdditionalDataQuickprop();

    /*
        Initialize 2 List to all zero.
     */
    void setNumWeights(int numWeights);
    double getdEdwLastCycle(int index);
    void   setdEdwLastCycle(int index, double dEdw);
    double getdwLastCycle(int index);
    void   setdwLastCycle(int index, double dw);

private:
    List<double> *dEdwLastCycle;
    List<double> *dwLastCycle;
};

#endif