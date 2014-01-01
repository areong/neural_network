#ifndef NEURALNETWORK_NEURALNETWORK_H_
#define NEURALNETWORK_NEURALNETWORK_H_

#include "../utils/list/List.h"

class Neuron;

class NeuralNetwork {
public:
    NeuralNetwork();
    ~NeuralNetwork();

    void addInputNeuron(Neuron *neuron);
    void removeInputNeuron(Neuron *neuron);
    void removeInputNeuronByIndex(int index);
    Neuron *getInputNeuron(int index);
    int getNumInputNeurons();

    void addOutputNeuron(Neuron *neuron);
    void removeOutputNeuron(Neuron *neuron);
    void removeOutputNeuronByIndex(int index);
    Neuron *getOutputNeuron(int index);
    int getNumOutputNeurons();

    /*
        Increase a hidden layer, i.e. add a empty hidden layer.
        Used before calling addHiddenNeuron
        if the given indexLayer is unvalid.
        Otherwise the addition is ignored.
     */
    void increaseHiddenLayer();
    /*
        Add a neuron to a hidden layer.
        If the index is unvalid, addition is ignored.
     */
    void addHiddenNeuron(int indexLayer, Neuron *neuron);
    /*
        Get a hidden Neuron by two indices.
        The index of layer starts from zero.
     */
    Neuron *getHiddenNeuron(int indexLayer, int indexNeuron);
    int getNumHiddenLayers();
    int getNumHiddenNeuronsInOneLayer(int indexLayer);

    void linkNeuronInputsToAllInputNeurons(Neuron *neuron);
    void linkNeuronInputsToAllNeuronsInOneHiddenLayer(int indexLayer, Neuron *neuron);

private:
    List<Neuron *> *inputLayer;
    List<Neuron *> *outputLayer;
    List<List<Neuron *> *> *hiddenLayers;
};

#endif