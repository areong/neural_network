#ifndef NEURALNETWORK_ACTIVATIONFUNCTION_HYPERBOLICTANGENT_H_
#define NEURALNETWORK_ACTIVATIONFUNCTION_HYPERBOLICTANGENT_H_

#include "IActivationFunction.h"

class HyperbolicTangent : public IActivationFunction {
public:
    ~HyperbolicTangent();
    double activate(double input);
    static HyperbolicTangent *getInstance();

private:
    HyperbolicTangent();
    static HyperbolicTangent *instance;
};

#endif