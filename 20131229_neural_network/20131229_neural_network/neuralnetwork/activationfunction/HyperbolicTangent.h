#ifndef NEURALNETWORK_ACTIVATIONFUNCTION_HYPERBOLICTANGENT_H_
#define NEURALNETWORK_ACTIVATIONFUNCTION_HYPERBOLICTANGENT_H_

#include "IActivationFunction.h"

class HyperbolicTangent : public IActivationFunction {
public:
    HyperbolicTangent();
    ~HyperbolicTangent();
    double activate(double input);
    double inverse(double output);
    double derivative(double input);
    double derivativeFromOutput(double output);

private:
    double scaling;
};

#endif