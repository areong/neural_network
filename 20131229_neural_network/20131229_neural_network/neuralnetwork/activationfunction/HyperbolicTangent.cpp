#include "HyperbolicTangent.h"
#include <cmath>

HyperbolicTangent::~HyperbolicTangent() {
}

double HyperbolicTangent::activate(double input) {
    return tanh(input);
}

HyperbolicTangent *HyperbolicTangent::getInstance() {
    if (instance == 0)
        instance = new HyperbolicTangent();
    return instance;
}

/*
    Private:
 */

HyperbolicTangent::HyperbolicTangent() {
}