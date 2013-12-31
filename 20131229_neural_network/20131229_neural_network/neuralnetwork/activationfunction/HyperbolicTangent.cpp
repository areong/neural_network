#include "HyperbolicTangent.h"
#include <cmath>

HyperbolicTangent::HyperbolicTangent() {
}

HyperbolicTangent::~HyperbolicTangent() {
}

double HyperbolicTangent::activate(double input) {
    return tanh(input);
}