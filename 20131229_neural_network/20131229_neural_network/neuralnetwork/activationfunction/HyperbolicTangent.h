#ifndef NEURALNETWORK_ACTIVATIONFUNCTION_HYPERBOLICTANGENT_H_
#define NEURALNETWORK_ACTIVATIONFUNCTION_HYPERBOLICTANGENT_H_

#include "IActivationFunction.h"

class HyperbolicTangent : public IActivationFunction {
public:
    HyperbolicTangent();
    ~HyperbolicTangent();
    double activate(double input);
    double inverse(double input);
    double derivative(double input);

private:
    double scaling;
};

#endif