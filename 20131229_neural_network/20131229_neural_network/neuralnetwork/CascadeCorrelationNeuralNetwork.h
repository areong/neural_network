#ifndef NEURALNETWORK_CASCADECORRELATIONNEURALNETWORK_H_
#define NEURALNETWORK_CASCADECORRELATIONNEURALNETWORK_H_

#include "../utils/list/List.h"

class NeuralNetwork;
class HyperbolicTangent;
class Data;

class CascadeCorrelationNeuralNetwork {
public:
    CascadeCorrelationNeuralNetwork();
    ~CascadeCorrelationNeuralNetwork();

    void initialize(int numInputs, int numOutputs);
    /*
        Set the learning rate.
        It is only used in the first cycle where normal backprop is used
        instead of Quickprop.
     */
    void setLearningRate(double learningRate);
    void setMaxNumHiddenLayers(int numLayers);
    /*
        Set the number of training cycles which is used
        decide when to check whether it reaches the goal,
        or it needs to add another hidden layer.
     */
    void setPatience(int numTrainingCycles);
    void setNumBigTrainingCyclesAddHidden(int numBigTrainingCyclesAddHidden);
    void setTrainingData(Data *trainingData);
    void run();

    double calculateCorrectRate(Data *data);

private:
    void feedInputs(Data *data, int index);
    void setCorrectOutputs(Data *data, int index);
    /*
        For example, change y = 3 to (0,0,1,0,0,0,0,0,0,0,0,0).
     */
    void setCorrectOutputsForMLFinal(Data *data, int index);

    void feedForward();
    double calculateErrorOneItem();
    void backPropagate();
    void backPropagateByTraditionalBackprop();
    void backPropagateByQuickprop();
    double calculate_dw_byTraditionalBackprop(double dEdw);
    double calculate_dw_byQuickprop(double dEdw, double dEdwLastCycle, double dwLastCycle);
    /*
        Add a hidden layer, the core of the algorithm.
     */
    void addHiddenLayer();
    bool isSameSign(double variableA, double variableB);

    NeuralNetwork *neuralNetwork;
    HyperbolicTangent *activationFunction;

    double learningRate;

    int maxNumHiddenLayers;
    
    /*
        (The total number of cycles) % (patience)
     */
    int numTrainingCycles;
    int patience;

    int numBigTrainingCyclesAddHidden;

    Data *trainingData;
    int currentIndexTrainingData;
    int currentIndexTrainingDataForAddingHiddenLayer;
    
    List<double> *correctOutputs;

    /*
        Used by Quickprop to decide whether to use
        normal backpropagation to set initial NeuronAdditionalData.
     */
    bool isTheFirstCycle;
};

#endif