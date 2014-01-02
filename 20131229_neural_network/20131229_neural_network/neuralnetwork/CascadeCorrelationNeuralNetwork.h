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
    void setTrainingData(Data *trainingData);
    void run();

    double calculateCorrectRate(Data *data);

private:
    void feedInputs(Data *data, int index);
    void setCorrectOutputs();
    /*
        For example, change y = 3 to (0,0,1,0,0,0,0,0,0,0,0,0).
     */
    void setCorrectOutputsForMLFinal();

    void feedForward();
    void backPropagate();
    void backPropagateByQuickprop();
    /*
        Add a hidden layer, the core of the algorithm.
     */
    void addHidderLayer();

    NeuralNetwork *neuralNetwork;
    HyperbolicTangent *activationFunction;

    double learningRate;

    int maxNumHiddenLayers;
    
    /*
        (The total number of cycles) % (patience)
     */
    int numTrainingCycles;
    int patience;

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