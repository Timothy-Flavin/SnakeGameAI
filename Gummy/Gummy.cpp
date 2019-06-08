#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<cmath>
#include"Matrix.h"
#include"DenseNet.h"
#define testMatrixMult 
#define testMatrixCreate
#define testDenseNetForMultiplication
struct csv {
public:
	std::string* data = NULL;
	int numLines = 0;
	int numVals = 0;
	double**numData = NULL;
};
csv readCSV(const char* fileName);
double** csvToDouble(csv* file);

int main() {
#ifndef testMatrixMult
	Matrix multTest1 = Matrix(2, 3);
	multTest1.fillRandInt(1, 3);
	Matrix multTest2 = Matrix(3, 2);
	multTest2.fillRandInt(1, 3);
	multTest1.print();
	std::cout << "/n hi \n";
	multTest2.print();
	std::cout << "/n hi \n";
	multTest1.multiply(multTest2).print();
#endif
#ifndef testDenseNetForMultiplication
	srand(time(NULL));
	int nl = 4;
	int*ll = new int[4];
	ll[0] = 2;
	ll[1] = 10;
	ll[2] = 10;
	ll[3] = 1;

	DenseNet test = DenseNet(nl, ll, false);
	Matrix* c = new Matrix(2, 1);
	Matrix* d = new Matrix(2, 1);
	c->fillRandDouble(0, 2);
	c->print();
	d->fillRandDouble(-1, 5);
	d->print();
	int numSamples = 21;
	double* x = new double[numSamples];

	for (int i = 0; i < numSamples; i++) {
		x[i] = i -(numSamples / 2);
		std::cout << "x[i]: " << x[i] << std::endl;
	}

	//test.print();
	//Matrix* m = test.feedForward(c);
	//test.print();
	//float x, y;
	clock_t time_req;

	/* Using pow function
	time_req = clock();
	for (int i = 0; i < 100; i++) {
		//std::cout << "i in the feed loop: " << i << std::endl;
		test.feedForward(c);
	}
	time_req = clock() - time_req;
	std::cout << "\n it took " << (float)time_req / CLOCKS_PER_SEC << " seconds" << std::endl;
	*/
	Matrix* errorm = new Matrix(1, 1);
	//errorm.set(0, 0, 0.544983);
	//errorm.set(1, 0, 0.10000000);
	//std::cout << "printing error matrix\n";
	//errorm.print();
	//std::cout<<"\nerror: "<<test.calcError(&errorm);
	//m->print();
	time_req = clock();
	int numTimes = 1000000;
	double avgEr = 0;
	int eSampleSize = 20;
	for (int i = 0; i < numTimes; i++) {
	
		c->set(0, 0, x[rand() % numSamples]);
		//std::cout << "c(0,0): " << c->get(0, 0);
		c->set(1, 0, x[rand() % numSamples]);
		//std::cout << "c(1,0): " << c->get(1, 0);
		//std::cout << "\npringting thinghy:";
		//c->print();
		
		errorm->set(0, 0, c->get(0, 0) + c->get(1, 0));
		//std::cout << "\nb feed";
		test.feedForward(c);
		//std::cout << "\na feed";
		//test.print();
		//test.printGradient();
		//for(int i = 0; i < 3; i++)
			//test.backProp(&errorm);
		test.backProp(errorm);
		//test.printGradient();
		if (numTimes > 100 && i % (numTimes/100) == 0) {
			std::cout << "\npercent done: " << i * 100.0 / numTimes<<"%";
			avgEr = 0;
			for (int j = 0; j < eSampleSize; j++) {
				c->set(0, 0, x[rand() % numSamples]);
				c->set(1, 0, x[rand() % numSamples]);
				errorm->set(0, 0, c->get(0, 0) + c->get(1, 0));
				test.feedForward(c);
				avgEr += test.calcError(errorm);
			}
			avgEr = avgEr / eSampleSize;
			std::cout << "\nsample Error: " << avgEr;
		}
	}
	time_req = clock() - time_req;
	std::cout << "\n it took " << (float)time_req / CLOCKS_PER_SEC << " seconds" << std::endl;
	//c->print();
	Matrix* m = test.feedForward(c);
	for (int i = 0; i < 10; i++) {
		c->set(0, 0, (rand() % 21) - 10);
		c->set(1, 0, (rand() % 21) - 10);
		std::cout << "input numbers round " << i << ": " << c->get(0, 0) << ", " << c->get(1, 0) << std::endl;
		m = test.feedForward(c);
		std::cout << "output number round " << i << ": " << m->get(0,0) << std::endl;
	}
	std::cout << "\nresult matrix after training with c";
	m->print();
	//test.print();
#endif
	csv* newcsv = NULL;
	newcsv = &readCSV("C:\\Users\\tcfla\\Desktop\\testcsv.csv");
	double** dat = csvToDouble(newcsv);
	std::cout << "printing dat\n";
	for (int i = 0; i < newcsv->numLines; i++) {
		for (int j = 0; j < newcsv->numVals; j++) {
			std::cout << dat[i][j] << ", ";
		}
		std::cout << std::endl;
	}
	std::cin.get();
	return 0;
}

csv readCSV(const char* fileName) {
	std::ifstream infile;
	infile.open(fileName);
	int curLine = 0;
	std::string dead = "";
	while (std::getline(infile, dead)) {
		curLine++;
	}
	std::cout << "line num: " << curLine;
	infile.clear();
	infile.seekg(0, std::ios::beg);
	csv file;
	file.numLines = curLine;
	file.data = new std::string[curLine];
	curLine = 0;
	while (std::getline(infile, file.data[curLine])) {
		std::cout << file.data[curLine] << std::endl;
		curLine++;
	}
	int numVals = 1;
	for (int i = 0; i < file.data[0].size(); i++) {
		if (file.data[0].at(i) == ',') {
			numVals++;
		}
	}
	std::cout << "\nnumVals: " << numVals;
	file.numVals = numVals;
	return file;
}

double** csvToDouble(csv* file) {
	double** dat = new double*[file->numLines];
	for (int i = 0; i < file->numLines; i++) {
		//std::cout << "starting thing\n";
		dat[i] = new double[file->numVals];
		//std::cout << "after error thing\n";
		int commaPos = 0;
		int vals = 0;
		for (int j = 0; j < file->data[i].size(); j++) {
			if (file->data[i].at(j) == ',') {
				//std::cout << "\nfirst if";
				dat[i][vals] = std::stod(file->data[i].substr(commaPos, j - commaPos));
				commaPos = j+1;
				vals++;
			}
			else if (j == file->data[i].size() - 1) {
				//std::cout << "\nsecond if";
				dat[i][vals] = std::stod(file->data[i].substr(commaPos));
				commaPos = j;
				vals++;
			}
		}
	}
	return dat;
}

//change made