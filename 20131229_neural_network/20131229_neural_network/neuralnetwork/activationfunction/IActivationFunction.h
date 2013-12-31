#ifndef NEURALNETWORK_ACTIVATIONFUNCTION_IACTIVATIONFUNCTION_H_
#define NEURALNETWORK_ACTIVATIONFUNCTION_IACTIVATIONFUNCTION_H_

class IActivationFunction {
public:
    virtual double activate(double input) = 0;
};

#endif