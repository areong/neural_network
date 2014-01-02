#ifndef NEURALNETWORK_NEURON_H_
#define NEURALNETWORK_NEURON_H_

#include "../utils/list/List.h"

class IActivationFunction;
class NeuronAdditionalData;

class Neuron {
public:
    Neuron();
    /*
        Deconstructor. Do not delete front Neurons.
     */
    ~Neuron();

    /*
        Calculate and return output:
            f( weighted sum + threshold ),
            f is the activation function.
        It uses the stored output of front neurons
        and won't let them to calculate again.
        If no front neurons (like input neurons)
        or no activation function is set,
        return the previously set output (default is zero).
     */
    double getOutput();
    /*
        Return the stored output without calculate again.
        Used in backpropagation.
     */
    double getStoredOutputWithoutCalculation();
    /*
        Calculate output.
        It's the same as getOutput() excepting
        not returning anything.
     */
    void calculateOutput();
    /*
        Set the output. Used in the input layer.
     */
    void setOutput(double output);

    /*
        Return the stored weighted sum without calculate again.
     */
    double getStoredWeightedSumWithoutCalculation();

    double getThreshold();
    /*
        Set the threshold, which is referenced as threshold weight
        or weight_0 in some case.
     */
    void setThreshold(double threshould);
    
    double getWeight(int index);
    void setWeight(int index, double weight);
    /*
        Set the weight to a front neuron given.
        Used in updating weights.
        If not in front neurons, do nothing.
     */
    void setWeightByAFrontNeuron(Neuron *neuron, double weight);
    void setWeightsAllZero();
    /*
        Set all weights randomly in (-1, 1).
     */
    void setWeightsRandomly();
    /*
        Lock the weights and leave them unchanged.
        @param lock true to lock, false to unlock.
     */
    void lockWeights(bool lock);
    bool isWeightsLocked();

    /*
        Add a front neuron to this neuron.
        The corresponding added weight is zero by default.
     */
    void addFrontNeuron(Neuron *neuron);
    void removeFrontNeuron(Neuron *neuron);
    void removeFrontNeuronByIndex(int index);
    Neuron *getFrontNeuron(int index);
    int getNumFrontNeurons();

    /*
        Set the activation function to use.
        If not set, no calculation will be done when getting output.
     */
    void setActivationFunction(IActivationFunction *function);
    IActivationFunction *getActivationFunction();

    /*
        Set instance to store additional data if needed.
     */
    void setAdditionalData(NeuronAdditionalData *additionalData);
    NeuronAdditionalData *getAdditionalData();

private:
    double output;
    double weightedSum;
    double threshould;
    
    List<double> *weights;
    bool weightsLocked;

    List<Neuron *> *frontNeurons;

    IActivationFunction *activationFunction;

    NeuronAdditionalData *additionalData;
};

#endif