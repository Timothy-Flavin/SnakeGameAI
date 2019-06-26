#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<cmath>
#include"Matrix.h"
#include"DenseNet.h"
#include"CSV.h"

class Gummy {
    private:
        csv* trainingData;
        int numIterations=0;
        double stepSize=0;
        int type=0;
        char* netFileName;
        char* csvFileName;
    public:
        Gummy();
        DenseNet* userInit();
        DenseNet* manualInit(char* fileName, char* nFileName, int ntype, int numLayers, int* layerSizes, bool sigmoid);
        void train(DenseNet* net);
        void saveNet(DenseNet* net);
        DenseNet* loadNet(char* fileName);
        void setNetFileName(char* nfn){netFileName = nfn;}
        void setCsvFileName(char* cfn){csvFileName = cfn;}
        csv* readCSV(char* fileName);
        void csvToDouble(csv* file);
        void updateTrainingData(bool);
        void setNumIterations(int n){numIterations=n;}
        void setStepSize(double n){stepSize=n;}
};