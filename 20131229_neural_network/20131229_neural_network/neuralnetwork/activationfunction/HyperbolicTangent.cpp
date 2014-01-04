#include "HyperbolicTangent.h"
#include <cmath>

HyperbolicTangent::HyperbolicTangent() {
    scaling = 100;
}

HyperbolicTangent::~HyperbolicTangent() {
}

double HyperbolicTangent::activate(double input) {
    return tanh(input / scaling);
}

double HyperbolicTangent::inverse(double output) {
    if (output > -1 && output < 1)
        // Calculate atanh, wich is missing in cmath.
        return (log(1 + output) - log(1 - output)) * scaling / 2;
    else if (output == -1)
        return -1 * 1e8 * scaling;
    else if (output == 1)
        return      1e8 * scaling;
    else
        return 0;
        
}

double HyperbolicTangent::derivative(double input) {
    double temp = tanh(input / scaling);
    return (1 - temp * temp) / scaling;
}

double HyperbolicTangent::derivativeFromOutput(double output) {
    return (1 - output * output) / scaling;
}