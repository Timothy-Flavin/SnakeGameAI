#pragma once
#include "Matrix.h"
#include <iostream>

class DenseNet {
private:
	bool sigmoidOutput = true;
	int numLayers = NULL;
	int*layerList = NULL;
	Matrix* weights = NULL;
	Matrix* bias = NULL;
	Matrix* activations = NULL;
	Matrix* eWeights = NULL;
	Matrix* eBias = NULL;
	Matrix* eActivation = NULL;
	double* errorList = NULL;
	void sigmoid(Matrix* A);
	double sigmoidPrime(double a);
public:
	DenseNet(int nl, int*ll, bool so);
	Matrix* feedForward(Matrix* inputs);
	double calcError(Matrix* A);
	void backProp(Matrix* feedback);
	void print();
	void printGradient();
};