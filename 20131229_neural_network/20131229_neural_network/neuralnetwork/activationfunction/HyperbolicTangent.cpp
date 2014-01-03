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

double HyperbolicTangent::inverse(double input) {
    if (input > -1 && input < 1)
        // Calculate atanh, wich is missing in cmath.
        return (log(1 + input) - log(1 - input)) * scaling / 2;
    else if (input == -1)
        return -1 * 1e8 * scaling;
    else if (input == 1)
        return      1e8 * scaling;
    else
        return 0;
        
}

double HyperbolicTangent::derivative(double input) {
    double temp = tanh(input / scaling);
    return (1 - temp * temp) * scaling;
}