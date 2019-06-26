#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Matrix.h"

Matrix::Matrix() {
	m = 0;
	n = 0;
	matrix = NULL;
	//srand(time(0));
}
Matrix::Matrix(int n1, int n2) {
	m = n1;
	n = n2;
	//srand(time(0));
	matrix = new double*[m];
	for (int i = 0; i < m; i++) {
		matrix[i] = new double[n];
		for (int j = 0; j < n; j++)
			matrix[i][j] = NULL;
	}
}
void Matrix::construct(int n1, int n2) {
	m = n1;
	n = n2;
	matrix = new double*[m];
	for (int i = 0; i < m; i++) {
		matrix[i] = new double[n];
		for (int j = 0; j < n; j++)
			matrix[i][j] = 0.01;
	}
}
void Matrix::fillRandInt(int min, int max) {
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			matrix[i][j] = rand() % (max - min) + min;
}
void Matrix::fillRandDouble(double min, double max) {
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			matrix[i][j] = (double)rand() / RAND_MAX *(max-min)+min;
}
Matrix* Matrix::multiply(Matrix* A) {
	Matrix *tempM = new Matrix(m, A->getN());
	if (n != A->getM()) {
		std::cout << "No matchy no multiply bruh";
	}
	else if (A->get(0, 0) == NULL) {
		std::cout << "null shit bruh";
	}
	else {
		for (int i = 0; i < m; i++)
			for (int j = 0; j < A->getN(); j++) {
				double val = 0;
				for (int k = 0; k < n; k++) {
					val += matrix[i][k] * A->get(k, j);
				}
				tempM->set(i, j, val);
			}
	}
	return tempM;
}
void Matrix::multiply(Matrix* A, Matrix* B) {
	if (n != A->getM()) {
		std::cout << "No matchy no multiply bruh";
	}
	else if (A->get(0, 0) == NULL && A->get(0,0) != 0) {
		std::cout << "null shit bruh";
		A->print();
	}
	else {
		for (int i = 0; i < m; i++)
			for (int j = 0; j < A->getN(); j++) {
				double val = 0;
				for (int k = 0; k < n; k++) {
					val += matrix[i][k] * A->get(k, j);
				}
				B->set(i, j, val);
			}
	}
}
Matrix::~Matrix() {
	for(int i = 0; i<m; i++)
		delete[] matrix[i];
	delete[] matrix;
}
Matrix* Matrix::multiply(double d) {
	Matrix* tempM = new Matrix(m, n);
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			tempM->set(i, j, matrix[i][j] * d);
	return tempM;
}
Matrix* Matrix::multiply(int n1) {
	Matrix* tempM = new Matrix(m, n);
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			tempM->set(i, j, matrix[i][j] * n1);
	return tempM;
}
Matrix* Matrix::multiply(float f) {
	Matrix* tempM = new Matrix(m, n);
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			tempM->set(i, j, matrix[i][j] * f);
	return tempM;
}
Matrix* Matrix::add(Matrix* A) {
	Matrix* tempM = new Matrix(m, n);
	if (m == A->getM() && n == A->getN()) {
		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++)
				tempM->set(i, j, matrix[i][j] + A->get(i, j));
	}
	else {
		std::cout << "Not addable";
	}
	return tempM;
}
void Matrix::add(Matrix* A, Matrix* B) {
	if (m == A->getM() && n == A->getN()) {
		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++)
				B->set(i, j, matrix[i][j] + A->get(i, j));
	}
	else {
		std::cout << "Not addable";
	}
}
void Matrix::print() {
	std::cout << "\nm: " << m <<std::endl;
	std::cout << "n: " << n << std::endl;
	std::cout << "matrix: " << matrix << std::endl;
	if (matrix == NULL) {
		std::cout << "tf?";
	}
	else {
		for (int i = 0; i < m; i++) {
			std::cout << std::endl;
			for (int j = 0; j < n; j++)
				if(matrix[i][j] != NULL)
					std::cout << " " << matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

