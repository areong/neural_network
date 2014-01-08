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
    void setNeuralNetworkParameters(int numInput, int numOutput);
    void setExperimentParameters(int numHiddenStart, int numHiddenEnd,
                                 int patienceBase, int patienceMultiplyStart, int patienceMultiplyEnd,
                                 int numBigTrainingCyclesAddHiddenStart, int numBigTrainingCyclesAddHiddenEnd, int numBigTrainingCyclesAddHiddenIncr,
                                 int numTimesEachParameter);
    void setTrainingData(Data *dataTraining);
    void execute();

private:
    void cutData();
    double calculateCorrectRate(int indexFold);

    int numFold;

    int numInput;
    int numOutput;

    int numHiddenStart;
    int numHiddenEnd;
    int patienceBase;
    int patienceMultiplyStart;
    int patienceMultiplyEnd;
    int numBigTrainingCyclesAddHiddenStart;
    int numBigTrainingCyclesAddHiddenEnd;
    int numBigTrainingCyclesAddHiddenIncr;
    int numTimesEachParameter;

    CascadeCorrelationNeuralNetwork *network;
    Data *dataTraining;
    List<Data *> *dataValidationTrain;
    List<Data *> *dataValidationValidate;
};

#endif