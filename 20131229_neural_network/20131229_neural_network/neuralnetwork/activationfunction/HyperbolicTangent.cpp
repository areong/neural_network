#include "HyperbolicTangent.h"
#include <cmath>

HyperbolicTangent::HyperbolicTangent() {
}

HyperbolicTangent::~HyperbolicTangent() {
}

double HyperbolicTangent::activate(double input) {
    return tanh(input);
}

double HyperbolicTangent::derivative(double input) {
    double temp = tanh(input);
    return 1 - temp * temp;
}