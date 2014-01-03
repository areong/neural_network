#include "CascadeCorrelationNeuralNetwork.h"

#include <iostream>
#include "../data/Data.h"
#include "activationfunction/HyperbolicTangent.h"
#include "neuronadditionaldata/NeuronAdditionalData.h"
#include "NeuralNetwork.h"
#include "Neuron.h"
using namespace std;

CascadeCorrelationNeuralNetwork::CascadeCorrelationNeuralNetwork() {
    neuralNetwork = new NeuralNetwork();
    activationFunction = new HyperbolicTangent();
    learningRate = 1;
    maxNumHiddenLayers = 0;
    numTrainingCycles = 0;
    patience = 0;
    trainingData = 0;
    currentIndexTrainingData = 0;
    currentIndexTrainingDataForAddingHiddenLayer = 0;
    correctOutputs = new List<double>();
    isTheFirstCycle = true;
}

CascadeCorrelationNeuralNetwork::~CascadeCorrelationNeuralNetwork() {
    delete neuralNetwork;
    delete activationFunction;
    delete correctOutputs;
}

void CascadeCorrelationNeuralNetwork::initialize(int numInputs, int numOutputs) {
    // Input layer.
    for (int i = 0; i < numInputs; i++) {
        // Create Neuron.
        Neuron *neuron = new Neuron(activationFunction);
        // Add the Neuron.
        neuralNetwork->addInputNeuron(neuron);
    }
    // Output layer.
    for (int i = 0; i < numOutputs; i++) {
        // Create Neuron.
        Neuron *neuron = new Neuron(activationFunction);
                
        // Link to all input Neurons.
        neuralNetwork->linkNeuronInputsToAllInputNeurons(neuron);

        // Add additional data.
        NeuronAdditionalData *additionalData = new NeuronAdditionalData(neuron->getNumFrontNeurons());
        neuron->setAdditionalData(additionalData);

        // Set weights randomly.
        //neuron->setWeightsRandomly();

        // Add the Neuron.
        neuralNetwork->addOutputNeuron(neuron);

        // List of correct outputs.
        correctOutputs->add(0);
    }
}

void CascadeCorrelationNeuralNetwork::setLearningRate(double learningRate) {
    this->learningRate = learningRate;
}

void CascadeCorrelationNeuralNetwork::setMaxNumHiddenLayers(int numLayers) {
    maxNumHiddenLayers = numLayers;
}

void CascadeCorrelationNeuralNetwork::setPatience(int numTrainingCycles) {
    patience = numTrainingCycles;
}

void CascadeCorrelationNeuralNetwork::setTrainingData(Data *trainingData) {
    this->trainingData = trainingData;
}

void CascadeCorrelationNeuralNetwork::run() {
    bool goalReached = false;
    while (!goalReached) {
        // If within patience.
        if (numTrainingCycles < patience) {
            feedInputs(trainingData, currentIndexTrainingData);
            setCorrectOutputs(trainingData, currentIndexTrainingData);
            feedForward();
            backPropagate();

            if (currentIndexTrainingData % 100 == 0)
                cout << "DataItem: " << currentIndexTrainingData << endl;
            
            numTrainingCycles += 1;
            currentIndexTrainingData = (currentIndexTrainingData + 1) % trainingData->getNumItems();
        }
        // Out of patience.
        else {
            // If do not reach maximum number of hidden layers.
            if (neuralNetwork->getNumHiddenLayers() < maxNumHiddenLayers) {
                addHiddenLayer();
                numTrainingCycles = 0;
            }
            // Else finish.
            else {
                goalReached = true;
            }
        }
    }
}

double CascadeCorrelationNeuralNetwork::calculateCorrectRate(Data *data) {
    int countCorrect = 0;
    // For each data item.
    for (int i = 0; i < data->getNumItems(); i++) {
        // Calculate output.
        feedInputs(data, i);
        feedForward();

        // Find output Neuron with the largest output.
        int outputMax = 0;
        int indexChosenOutputNeuron = 0;
        for (int j = 0; j < neuralNetwork->getNumOutputNeurons(); j++) {
            Neuron *outputNeuron = neuralNetwork->getOutputNeuron(j);
            if (outputNeuron->getStoredOutputWithoutCalculation() > outputMax) {
                outputMax = outputNeuron->getStoredOutputWithoutCalculation();
                indexChosenOutputNeuron = j;                   
            }
        }

        cout << i << '\t' << indexChosenOutputNeuron + 1 << endl;

        // If correct.
        if (indexChosenOutputNeuron + 1 == data->get_y_byIndex(i))
            countCorrect += 1;
    }
    return (double)countCorrect / (double)(data->getNumItems());
}

/*
    Private:
 */

void CascadeCorrelationNeuralNetwork::feedInputs(Data *data, int index) {
    for (int i = 0; i < neuralNetwork->getNumInputNeurons(); i++)
        neuralNetwork->getInputNeuron(i)->setOutput(
            data->get_x_i_byIndexAnd_i(index, i));
}

void CascadeCorrelationNeuralNetwork::setCorrectOutputs(Data *data, int index) {
    setCorrectOutputsForMLFinal(data, index);
}

void CascadeCorrelationNeuralNetwork::setCorrectOutputsForMLFinal(Data *data, int index) {
    int correct_y = trainingData->get_y_byIndex(currentIndexTrainingData);
    for (int i = 0; i < neuralNetwork->getNumOutputNeurons(); i++) {
        if (i == correct_y - 1)
            correctOutputs->set(i, 1);
        else
            correctOutputs->set(i, -1);
    }
}

void CascadeCorrelationNeuralNetwork::feedForward() {
    // Hidden layers.
    for (int indexHidden = 0; indexHidden < neuralNetwork->getNumHiddenLayers(); indexHidden++)
        for (int i = 0; i < neuralNetwork->getNumHiddenNeuronsInOneLayer(indexHidden); i++)
            neuralNetwork->getHiddenNeuron(indexHidden, i)->calculateOutput();
    // Output layer.
    for (int i = 0; i < neuralNetwork->getNumOutputNeurons(); i++)
        neuralNetwork->getOutputNeuron(i)->calculateOutput();
}

void CascadeCorrelationNeuralNetwork::backPropagate() {
    backPropagateByQuickprop();
}

void CascadeCorrelationNeuralNetwork::backPropagateByTraditionalBackprop() {
    // Only one layer update is needed.
    // For each output Neuron.
    for (int j = 0; j < neuralNetwork->getNumOutputNeurons(); j++) {
        Neuron *outputNeuron = neuralNetwork->getOutputNeuron(j);
        // Calculate y - y_true.
        double diff = outputNeuron->getStoredOutputWithoutCalculation() - 
                      correctOutputs->get(j);
            
        // For each front Neuron.
        for (int i = 0; i < outputNeuron->getNumFrontNeurons(); i++) {
            // Output of front Neuron
            double outputFront = outputNeuron->getFrontNeuron(i)->getStoredOutputWithoutCalculation();
            // storedWeightedSum
            double storedWeightedSum = outputNeuron->getStoredWeightedSumWithoutCalculation();
            // Calculate dE/dw_i
            double dEdw = diff * 
                            outputNeuron->getActivationFunction()->derivative(storedWeightedSum) *
                            outputFront;
            // Calculate delta w_i
            double dw = calculate_dw_byTraditionalBackprop(dEdw);

            //cout << i << '\t' << outputFront << '\t' << diff << '\t' << dEdw << '\t' << dw << endl;

            // Update weight.
            outputNeuron->setWeight(i, outputNeuron->getWeight(i) + dw);
        }

        // Threshold.
        // Output of front Neuron
        double outputFront = 1;
        // storedWeightedSum
        double storedWeightedSum = outputNeuron->getStoredWeightedSumWithoutCalculation();
        // Calculate dE/dw_i
        double dEdThreshold = diff * 
                      outputNeuron->getActivationFunction()->derivative(storedWeightedSum) *
                      outputFront;
        // Calculate delta w_i
        double dThreshold = calculate_dw_byTraditionalBackprop(dEdThreshold);

        //cout << outputFront << '\t' << dEdw << '\t' << dw << endl;

        // Update weight.
        outputNeuron->setThreshold(outputNeuron->getThreshold() + dThreshold);

        // End of each output Neuron.
    }
}

void CascadeCorrelationNeuralNetwork::backPropagateByQuickprop() {
    // Only one layer update is needed.
    // For each output Neuron.
    for (int j = 0; j < neuralNetwork->getNumOutputNeurons(); j++) {
        Neuron *outputNeuron = neuralNetwork->getOutputNeuron(j);
        // Calculate y - y_true.
        double diff = outputNeuron->getStoredOutputWithoutCalculation() - 
                      correctOutputs->get(j);
            
        // For each front Neuron.
        for (int i = 0; i < outputNeuron->getNumFrontNeurons(); i++) {
            // Output of front Neuron
            double outputFront = outputNeuron->getFrontNeuron(i)->getStoredOutputWithoutCalculation();
            // storedWeightedSum
            double storedWeightedSum = outputNeuron->getStoredWeightedSumWithoutCalculation();
            // Calculate dE/dw_i
            double dEdw = diff * 
                            outputNeuron->getActivationFunction()->derivative(storedWeightedSum) *
                            outputFront;
            
            // Calculate delta w_i
            // If is the first cycle, use traditional backprop.
            double dw = 0;
            if (isTheFirstCycle) {
                dw = calculate_dw_byTraditionalBackprop(dEdw);
            }
            // If not the first cycle, use Quickprop.
            else {
                dw = calculate_dw_byQuickprop(dEdw,
                                outputNeuron->getAdditionalData()->getdEdwLastCycle(i),
                                outputNeuron->getAdditionalData()->getdwLastCycle(i));
            }

            //cout << i << '\t' << outputFront << '\t' << diff << '\t' << dEdw << '\t' << dw << endl;

            // Update weight.
            outputNeuron->setWeight(i, outputNeuron->getWeight(i) + dw);

            if (dw != 0) {
                // Store to additional data.
                outputNeuron->getAdditionalData()->setdEdwLastCycle(i, dEdw);
                outputNeuron->getAdditionalData()->setdwLastCycle(i, dw);
            }
        }

        // Threshold.

        // Output of front Neuron
        double outputFront = 1;

        // storedWeightedSum
        double storedWeightedSum = outputNeuron->getStoredWeightedSumWithoutCalculation();

        // Calculate dE/dThreshold
        double dEdThreshold = diff * 
                      outputNeuron->getActivationFunction()->derivative(storedWeightedSum) *
                      outputFront;
        
        // Calculate dThreshold.
        // If is the first cycle, use traditional backprop.
        double dThreshold = 0;
        if (isTheFirstCycle) {
            dThreshold = calculate_dw_byTraditionalBackprop(dEdThreshold);
        }
        // If not the first cycle, use Quickprop.
        else {
            dThreshold = calculate_dw_byQuickprop(dEdThreshold,
                            outputNeuron->getAdditionalData()->getdEdThresholdLastCycle(),
                            outputNeuron->getAdditionalData()->getdThresholdLastCycle());
        }

        //cout << i << '\t' << outputFront << '\t' << diff << '\t' << dEdw << '\t' << dw << endl;

        // Update weight.
        outputNeuron->setThreshold(outputNeuron->getThreshold() + dThreshold);

        if (dThreshold != 0) {
            // Store to additional data.
            outputNeuron->getAdditionalData()->setdEdThresholdLastCycle(dEdThreshold);
            outputNeuron->getAdditionalData()->setdThresholdLastCycle(dThreshold);
        }

        // End of each output Neuron.
    }

    // Update the isTheFirstCycle.
    if (isTheFirstCycle)
        isTheFirstCycle = false;
}

double CascadeCorrelationNeuralNetwork::calculate_dw_byTraditionalBackprop(double dEdw) {
    return -1 * learningRate * dEdw;
}

double CascadeCorrelationNeuralNetwork::calculate_dw_byQuickprop(double dEdw,
                                                                 double dEdwLastCycle,
                                                                 double dwLastCycle) {
    double temp = dEdwLastCycle - dEdw;
    if (temp == 0)
        temp = 1;
    return dEdw / temp *dwLastCycle;
}
 
void CascadeCorrelationNeuralNetwork::addHiddenLayer() {
    // Create a Neuron.
    Neuron *neuron = new Neuron(activationFunction);
    
    // Link to all existing Neurons except for output Neurons.
    neuralNetwork->linkNeuronInputsToAllInputNeurons(neuron);
    for (int i = 0; i < neuralNetwork->getNumHiddenLayers(); i++)
        neuralNetwork->linkNeuronInputsToAllNeuronsInOneHiddenLayer(i, neuron);    
    
    // Set weights randomly.
    neuron->setWeightsRandomly();
    
    // Add additional data.
    NeuronAdditionalData *additionalData = new NeuronAdditionalData(neuron->getNumFrontNeurons());
    neuron->setAdditionalData(additionalData);

    // To store V_p, V_average, E_p_o and E_o_average.
    // Annotations follows original paper.
    List<double> *V_p = new List<double>();
    double V_average = 0;
    List<List<double> *> *E_p_o = new List<List<double> *>();
    List<double> *E_o_average = new List<double>();

    // Also store O_p_o, the outputs of each data item.
    List<List<double> *> *O_p_o = new List<List<double> *>();

    // And H_p_o, storing outputs of hidden Neurons.
    List<List<List<double> *> *> *H_p_o = new List<List<List<double> *> *>();

    // Calculate E_p_o and E_o_average, which will not change
    // through out the adding-hidden-layer process.
    // Also stores O_p_o and H_p_o.
    for (int i = 0; i < trainingData->getNumItems(); i++) {
        // Feed.
        feedInputs(trainingData, i);
        setCorrectOutputs(trainingData, i);
        feedForward();

        // H_p_o.
        List<List<double> *> *H_p_o_eachDataItem = new List<List<double> *>();
        for (int iLayer = 0; iLayer < neuralNetwork->getNumHiddenLayers(); iLayer++) {
            List<double> *H_p_o_eachLayer = new List<double>();
            for (int iHidden = 0; iHidden < neuralNetwork->getNumHiddenNeuronsInOneLayer(iLayer); iHidden++) {
                double output = neuralNetwork->getHiddenNeuron(iLayer, iHidden)->getStoredOutputWithoutCalculation();
                H_p_o_eachLayer->add(output);
            }
            H_p_o_eachDataItem->add(H_p_o_eachLayer);
        }
        H_p_o->add(H_p_o_eachDataItem);

        // E_p_o.
        List<double> *E_p_o_eachDataItem = new List<double>();
        List<double> *O_p_o_eachDataItem = new List<double>();
        for (int j = 0; j < neuralNetwork->getNumOutputNeurons(); j++) {
            // O_p_o
            double output = neuralNetwork->getOutputNeuron(j)->getStoredOutputWithoutCalculation();
            O_p_o_eachDataItem->add(output);

            // Square error.
            double error = correctOutputs->get(j) - output;            
            error = error * error / 2;
            E_p_o_eachDataItem->add(error);

            // E_o_average accumulate.
            E_o_average->add(E_o_average->get(j) + error);
        }
        E_p_o->add(E_p_o_eachDataItem);
        O_p_o->add(O_p_o_eachDataItem);
    }
    // E_o_average.
    for (int i = 0; i < neuralNetwork->getNumOutputNeurons(); i++)
        E_o_average->set(i, E_o_average->get(i) / trainingData->getNumItems());

    // For number of big training cycles.
    int numBigTrainingCycles = 6;
    for (int iBigCycle = 0; iBigCycle < numBigTrainingCycles; iBigCycle++) {
        // Clear V_p and V_average.
        V_p->clear();
        V_average = 0;

        // Calculate V_p and V_average, which will change
        // through out the adding-hidden-layer process.
        for (int i = 0; i < trainingData->getNumItems(); i++) {
            // Feed.
            feedInputs(trainingData, i);

            // V_p.
            V_p->add(neuron->getOutput());

            // V_average accumulate.
            V_average += neuron->getStoredOutputWithoutCalculation();
            //cout << "neuron->getOutput(): " << neuron->getStoredOutputWithoutCalculation() << endl;
        }
        // V_average.
        V_average /= trainingData->getNumItems();
    
        //cout << "V_average: " << V_average << endl;
        //for (int i = 0; i < neuralNetwork->getNumOutputNeurons(); i++)
        //    cout << "E_o_average->get(" << i << "): " << E_o_average->get(i) << endl;

        // Calculate S.
        double S = 0;
        for (int j = 0; j < neuralNetwork->getNumOutputNeurons(); j++) {
            double innerSum = 0;
            for (int i = 0; i < trainingData->getNumItems(); i++) {
                innerSum += (V_p->get(i) - V_average) *
                            (E_p_o->get(i)->get(j) - E_o_average->get(j));
            }
            // Absolute value.
            if (innerSum < 0)
                innerSum *= -1;
            S += innerSum;
            //cout << "j: " << j << ", innerSum: " << innerSum << endl;
        }

        cout << "Big cycle: " << iBigCycle << ", S: " << S << endl;

        // Update weights of the new Neuron.
        // Code is a little bit similar to 
        // the CascadeCorrelationNeuralNetwork::backPropagateByQuickprop().

        // For a weight to input a Neuron.
        for (int iWeight = 0; iWeight < neuralNetwork->getNumInputNeurons(); iWeight++) {

            // Outer sum over p and o.
            double outerSum = 0;

            // For each data item.
            for (int i = 0; i < trainingData->getNumItems(); i++) {
                // Calculate I_i_p and f_p_derivative
                // First iWeights are the indices of input Neurons.
                double I_i_p = trainingData->get_x_i_byIndexAnd_i(i, iWeight);
                // Inversely calculate the weightedSum
                double weightedSum = neuron->getActivationFunction()->inverse(V_p->get(i));
                double f_p_derivative = neuron->getActivationFunction()->derivative(weightedSum);

                // Inner sum over o.
                double innerSum = 0;

                // For each output Neuron, or o.
                for (int j = 0; j < neuralNetwork->getNumOutputNeurons(); j++) {
                    // Difference between E_p_o and E_o_average.
                    double diff = E_p_o->get(i)->get(j) - E_o_average->get(j);

                    // "Sign of correlation between V_p and Output_p_o."
                    // Assume to be "sign between V_p and Output_p_o."
                    double sigma_o = 1;
                    if (V_p->get(i) > 0) {
                        if (O_p_o->get(i)->get(j) < 0)
                            sigma_o = -1;
                    }
                    else {
                        if (O_p_o->get(i)->get(j) > 0)
                            sigma_o = -1;
                    }

                    // Add to innerSum.
                    innerSum += sigma_o * diff;
                        
                    // End of for each output Neuron.
                }

                // Add to outer sum.
                outerSum += innerSum * f_p_derivative * I_i_p;
            }

            // dS/dw_i = outerSum.
            double dSdw = outerSum;

            // dw
            // If is the first big cycle, use traditional backprop.
            double dw = 0;
            if (iBigCycle == 0) {
                dw = calculate_dw_byTraditionalBackprop(dSdw);
            }
            // If not the first big cycle, use Quickprop.
            else {
                dw = calculate_dw_byQuickprop(dSdw,
                                neuron->getAdditionalData()->getdEdwLastCycle(iWeight),
                                neuron->getAdditionalData()->getdwLastCycle(iWeight));
            }

            // Update weight
            neuron->setWeight(iWeight, neuron->getWeight(iWeight) + dw);

            if (dw != 0) {
                // Store to additional data.
                neuron->getAdditionalData()->setdEdwLastCycle(iWeight, dSdw);
                neuron->getAdditionalData()->setdwLastCycle(iWeight, dw);
            }

            // End of each weight to a input Neuron.
        }
            
        // Threshold.

        // Outer sum over p and o.
        double outerSum = 0;

        // For each data item.
        for (int i = 0; i < trainingData->getNumItems(); i++) {
            // Calculate I_i_p and f_p_derivative
            // First iWeights are the indices of input Neurons.
            double I_i_p = 1;
            // inversely calculate the weightedSum
            double weightedSum = neuron->getActivationFunction()->inverse(V_p->get(i));
            double f_p_derivative = neuron->getActivationFunction()->derivative(weightedSum);

            // Inner sum over o.
            double innerSum = 0;

            // For each output Neuron, or o.
            for (int j = 0; j < neuralNetwork->getNumOutputNeurons(); j++) {
                // Difference between E_p_o and E_o_average.
                double diff = E_p_o->get(i)->get(j) - E_o_average->get(j);

                // "Sign of correlation between V_p and Output_p_o."
                // Assume to be "sign between V_p and Output_p_o."
                double sigma_o = 1;
                if (V_p->get(i) > 0) {
                    if (O_p_o->get(i)->get(j) < 0)
                        sigma_o = -1;
                }
                else {
                    if (O_p_o->get(i)->get(j) > 0)
                        sigma_o = -1;
                }

                // Add to innerSum.
                innerSum += sigma_o * diff;
                        
                // End of for each output Neuron.
            }

            // Add to outer sum.
            outerSum += innerSum * f_p_derivative * I_i_p;
        }

        // dS/dThreshold = outerSum.
        double dSdThreshold = outerSum;

        // dThreshold
        // If is the first big cycle, use traditional backprop.
        double dThreshold = 0;
        if (iBigCycle == 0) {
            dThreshold = calculate_dw_byTraditionalBackprop(dSdThreshold);
        }
        // If not the first big cycle, use Quickprop.
        else {
            dThreshold = calculate_dw_byQuickprop(dSdThreshold,
                            neuron->getAdditionalData()->getdEdThresholdLastCycle(),
                            neuron->getAdditionalData()->getdThresholdLastCycle());
        }

        // Update weight
        neuron->setThreshold(neuron->getThreshold() + dThreshold);

        if (dThreshold != 0) {
            // Store to additional data.
            neuron->getAdditionalData()->setdEdThresholdLastCycle(dSdThreshold);
            neuron->getAdditionalData()->setdThresholdLastCycle(dThreshold);
        }
        // End of threshold.
        
        // For each weight to a hidden Neuron.
        for (int iLayer = 0; iLayer < neuralNetwork->getNumHiddenLayers(); iLayer++) {
            for (int iHidden = 0; iHidden < neuralNetwork->getNumHiddenNeuronsInOneLayer(iLayer); iHidden++) {
                Neuron *hiddenNeuron = neuralNetwork->getHiddenNeuron(iLayer, iHidden);

                // Index of weight.
                int iWeight = neuron->getFrontNeuronIndex(hiddenNeuron);
                
                // Check if it is really linked to the new Neuron.
                // (It should be yes here.)
                if (iWeight < 0)
                    continue;

                // Start calculate.

                // Outer sum over p and o.
                double outerSum = 0;

                // For each data item.
                for (int i = 0; i < trainingData->getNumItems(); i++) {
                    // I_i_p is the output of the hidden neuron.
                    double I_i_p = H_p_o->get(i)->get(iLayer)->get(iHidden);

                    // Inversely calculate the weightedSum
                    double weightedSum = neuron->getActivationFunction()->inverse(V_p->get(i));
                    
                    // f_p_derivative.
                    double f_p_derivative = neuron->getActivationFunction()->derivative(weightedSum);

                    // Inner sum over o.
                    double innerSum = 0;

                    // For each output Neuron, or o.
                    for (int j = 0; j < neuralNetwork->getNumOutputNeurons(); j++) {
                        // Difference between E_p_o and E_o_average.
                        double diff = E_p_o->get(i)->get(j) - E_o_average->get(j);

                        // "Sign of correlation between V_p and Output_p_o."
                        // Assume to be "sign between V_p and Output_p_o."
                        double sigma_o = 1;
                        if (V_p->get(i) > 0) {
                            if (O_p_o->get(i)->get(j) < 0)
                                sigma_o = -1;
                        }
                        else {
                            if (O_p_o->get(i)->get(j) > 0)
                                sigma_o = -1;
                        }

                        // Add to innerSum.
                        innerSum += sigma_o * diff;
                        
                        // End of for each output Neuron.
                    }

                    // Add to outer sum.
                    outerSum += innerSum * f_p_derivative * I_i_p;
                }

                // dS/dw_i = outerSum.
                double dSdw = outerSum;

                // dw
                // If is the first big cycle, use traditional backprop.
                double dw = 0;
                if (iBigCycle == 0) {
                    dw = calculate_dw_byTraditionalBackprop(dSdw);
                }
                // If not the first big cycle, use Quickprop.
                else {
                    dw = calculate_dw_byQuickprop(dSdw,
                                    neuron->getAdditionalData()->getdEdwLastCycle(iWeight),
                                    neuron->getAdditionalData()->getdwLastCycle(iWeight));
                }

                // Update weight
                neuron->setWeight(iWeight, neuron->getWeight(iWeight) + dw);

                if (dw != 0) {
                    // Store to additional data.
                    neuron->getAdditionalData()->setdEdwLastCycle(iWeight, dSdw);
                    neuron->getAdditionalData()->setdwLastCycle(iWeight, dw);
                }


                // End
            }
        }
        // End of each weight to a hidden Neuron.

        // End of Big training cycle.
        //cout << "End of Big training cycle " << iBigCycle << endl;
    }

    // Add the neuron to the neural network.

    // Lock the weights of the Neuron.
    neuron->lockWeights(true);
    
    // Add a empty hidden layer.
    neuralNetwork->increaseHiddenLayer();

    // Add to the layer.
    int indexLayer = neuralNetwork->getNumHiddenLayers() - 1;
    neuralNetwork->addHiddenNeuron(indexLayer, neuron);

    // Link all output neurons to the Neuron.
    for (int j = 0; j < neuralNetwork->getNumOutputNeurons(); j++) {
        Neuron *outputNeuron = neuralNetwork->getOutputNeuron(j);
        neuralNetwork->linkNeuronInputsToAllNeuronsInOneHiddenLayer(indexLayer, outputNeuron);
    }

    // Complete.
}

