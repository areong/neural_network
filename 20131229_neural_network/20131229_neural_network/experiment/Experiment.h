#ifndef EXPERIMENT_EXPERIMENT_H_
#define EXPERIMENT_EXPERIMENT_H_

#include "../utils/list/List.h"

class Data;
class CascadeCorrelationNeuralNetwork;

class Experiment {
public:
    Experiment();
    ~Experiment();

    void setNFoldCrossValidation(int fold);
    void setTrainingData(Data *dataTraining);
    void execute();

private:
    void cutData();
    double calculateCorrectRate(int indexFold);

    int numFold;

    CascadeCorrelationNeuralNetwork *network;
    Data *dataTraining;
    List<Data *> *dataValidationTrain;
    List<Data *> *dataValidationValidate;
};

#endif