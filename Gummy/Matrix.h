#pragma once

class Matrix {
private:
	int m, n;
	double**matrix;
	bool isSquare = false;
public:
	Matrix();
	~Matrix();
	Matrix(int m, int n);
	void construct(int m, int n);
	void fillRandInt(int min, int max);
	void fillRandDouble(double min, double max);
	Matrix* multiply(Matrix* A);
	void multiply(Matrix* A, Matrix* B);
	Matrix* multiply(double d);
	Matrix* multiply(int i);
	Matrix* multiply(float f);
	Matrix* add(Matrix *A);
	void add(Matrix *A, Matrix* B);
	//Matrix inverse(Matrix A);
	void print();
	void set(int i, int j, double n) { matrix[i][j] = n; }
	double get(int i, int j) { return matrix[i][j]; }
	bool square() { return isSquare; }
	int getN() { return n; }
	int getM() { return m; }
};
