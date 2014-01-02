#include "HyperbolicTangent.h"
#include <cmath>

HyperbolicTangent::HyperbolicTangent() {
}

HyperbolicTangent::~HyperbolicTangent() {
}

double HyperbolicTangent::activate(double input) {
    return tanh(input / 1);
}

double HyperbolicTangent::derivative(double input) {
    double temp = tanh(input / 1);
    return 1 - temp * temp;
}