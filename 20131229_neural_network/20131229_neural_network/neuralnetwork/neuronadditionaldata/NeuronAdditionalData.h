#ifndef NEURALNETWORK_NEURONADDITIONALDATAQUICKPROP_H_
#define NEURALNETWORK_NEURONADDITIONALDATAQUICKPROP_H_

#include "../../utils/list/List.h"

class NeuronAdditionalData {
public:
    NeuronAdditionalData(int numWeights);
    ~NeuronAdditionalData();

    /*
        Initialize 2 List to all zero.
     */
    void setNumWeights(int numWeights);
    void increaseNumWeights();
    void removeByIndex(int index);
    double getdEdwLastCycle(int index);
    void   setdEdwLastCycle(int index, double dEdw);
    double getdwLastCycle(int index);
    void   setdwLastCycle(int index, double dw);
    double getdEdThresholdLastCycle();
    void   setdEdThresholdLastCycle(double dEdThreshold);
    double getdThresholdLastCycle();
    void   setdThresholdLastCycle(double dThreshold);

private:
    List<double> *dEdwLastCycle;
    List<double> *dwLastCycle;
    double dEdThresholdLastCycle;
    double dThresholdLastCycle;
};

#endif