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
#define testcsvreader
struct csv {
public:
	std::string* data = NULL;
	int numLines = 0;
	int numVals = 0;
	double**numData = NULL;
};
csv* readCSV(const char* fileName);
void csvToDouble(csv* file);
DenseNet* init(DenseNet* net, int* numIterations, double* stepSize, csv* file);
void train(DenseNet* net, int numIterations, double* stepSize, csv* file);

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
		test.backProp(errorm, 0.005);
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
#ifndef testcsvreader
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
#endif
	DenseNet * userNet = NULL;
	int numIterations = 0;
	double* stepSize=new double;
	csv* newcsv = new csv;
	userNet = init(userNet, &numIterations, stepSize, newcsv);
	userNet->print();
	train(userNet, numIterations, stepSize, newcsv);
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.get();
	return 0;
}

csv* readCSV(char* fileName) {
	std::ifstream infile;
	std::cout<<"file being read "<<fileName<<std::endl;
	infile.open(fileName);
	std::cout<<"opened file"<<std::endl;
	int curLine = 0;
	std::string dead = "";
	while (std::getline(infile,dead)) {
		curLine++;
		std::cout<<"why not working"<<std::endl;
		//infile>>dead;
		std::cout<<infile.good()<<", got line "<<curLine<<", "<<dead<<std::endl;
		/*if(infile.good()==0){
			std::cout<<"done"<<std::endl;
			break;
		}*/
	}
	std::cout << "line num: "<<curLine<<std::endl; //<< curLine;
	infile.clear();
	std::cout<<"cleared"<<std::endl;
	infile.seekg(0, std::ios::beg);
	std::cout<<"seeked 0"<<std::endl;
	csv* file = new csv;
	file->numLines = curLine;
	std::cout<<"made num lines"<<std::endl;
	file->data = new std::string[curLine];
	curLine = 0;
	std::cout<<"made empty file"<<std::endl;
	while (std::getline(infile, file->data[curLine])) {
		std::cout << file->data[curLine] << std::endl;
		curLine++;
	}
	std::cout<<"filled file"<<std::endl;
	int numVals = 1;
	for (int i = 0; i < file->data[0].size(); i++) {
		if (file->data[0].at(i) == ',') {
			numVals++;
		}
	}
	std::cout<<"got num vals"<<std::endl;
	std::cout << "\nnumVals: " << numVals<<std::endl;
	file->numVals = numVals;
	return file;
}

void csvToDouble(csv* file) {
	file->numData = new double*[file->numLines];
	for (int i = 0; i < file->numLines; i++) {
		//std::cout << "starting thing\n";
		file->numData[i] = new double[file->numVals];
		//std::cout << "after error thing\n";
		int commaPos = 0;
		int vals = 0;
		for (int j = 0; j < file->data[i].size(); j++) {
			if (file->data[i].at(j) == ',') {
				//std::cout << "\nfirst if";
				file->numData[i][vals] = std::stod(file->data[i].substr(commaPos, j - commaPos));
				commaPos = j+1;
				vals++;
			}
			else if (j == file->data[i].size() - 1) {
				//std::cout << "\nsecond if";
				file->numData[i][vals] = std::stod(file->data[i].substr(commaPos));
				commaPos = j;
				vals++;
			}
		}
	}
}

DenseNet* init(DenseNet* nete, int* numIterations, double* stepSize, csv* file) {
	srand(time(NULL));
	int netType = 0, numLayers = 0;
	int*layerSizes=NULL;
	char*fileName = new char[20];
	bool sigmoidOutput = true;
	std::string path = "";
	std::cout << "\nWhat type of Neural net do you want?\n1: Dense, 2: Recurrent(NA), 3: TBD\n";
	std::cin >> netType;
	std::cout << "\nHow many layers do you want?\n";
	std::cin >> numLayers;
	layerSizes = new int[numLayers];
	std::cout << "number of nodes in layer 0 and last layer must \nadd up to number of values in csv file\n";
	for (int i = 0; i < numLayers; i++) {
		std::cout << "how many nodes in layer " << i << std::endl;
		std::cin >> layerSizes[i];
	}
	std::cout << "\nChoose output mode: 1: sigmoid, 0: raw\n";
	std::cin >> sigmoidOutput;
	std::cout << "\nChoose number of iterations to train: ";
	std::cin >> *numIterations;
	std::cout << "\nChoose step size: ";
	std::cin >> *stepSize;
	std::cout<< "\nput file into gummy.cpp folder and enter file name: ";
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.getline(fileName, 20);
	std::cout<<"FINISHED GATHERING DATA\n"<<fileName<<std::endl;
	DenseNet* net = new DenseNet(numLayers, layerSizes, sigmoidOutput);
	//net->print();
	std::cout<<"reading file"<<std::endl;
	csv* temp = readCSV(fileName);
	file->data=temp->data;
	file->numLines=temp->numLines;
	file->numVals=temp->numVals;
	csvToDouble(file);
	std::cout<<"FINISHED MAKING DENSE NET\n";
	return net;
}

void train(DenseNet* net, int numIterations, double* stepSize, csv* file) {
	std::cout<<"training net"<<std::endl;
	int percentNum = 1;
	if (numIterations >= 100) {
		percentNum = numIterations / 100;
	}
	std::cout<<"assigned percent "<<numIterations<<std::endl;
	net->print();
	int rowNum = 0;
	int numIn = net->getNumInputs();
	int numOut = net->getNumOutputs();
	std::cout<<"got in and outputs"<<std::endl;
	Matrix* inputMatrix = new Matrix(numIn,1);
	Matrix* outputMatrix = new Matrix(numOut,1);
	std::cout<<"made training matrices"<<std::endl;
	for (int i = 0; i < numIterations; i++) {
		rowNum = rand() % file->numLines;
		std::cout<<"picked a row"<<std::endl;
		for(int j = 0; j < numIn; j++){
			inputMatrix->set(j,0,file->numData[rowNum][j]);
		}
		for(int j = 0; j < numOut; j++){
			outputMatrix->set(j,0,file->numData[rowNum][j+numIn]);
		}
		std::cout<<"set input and outputs"<<std::endl;
		net->feedForward(inputMatrix);
		std::cout<<"fed forward"<<std::endl;
		net->backProp(outputMatrix, *stepSize);
		std::cout<<"back propped"<<std::endl;
		if (i%percentNum == 0) {
			std::cout << "percent done: " << i / percentNum<<"%\n";
		}
	}
}