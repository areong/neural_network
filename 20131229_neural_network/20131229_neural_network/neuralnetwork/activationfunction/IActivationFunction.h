#ifndef NEURALNETWORK_ACTIVATIONFUNCTION_IACTIVATIONFUNCTION_H_
#define NEURALNETWORK_ACTIVATIONFUNCTION_IACTIVATIONFUNCTION_H_

class IActivationFunction {
public:
    virtual double activate(double input) = 0;
    virtual double inverse(double output) = 0;
    virtual double derivative(double input) = 0;
    virtual double derivativeFromOutput(double output) = 0;
};

#endif