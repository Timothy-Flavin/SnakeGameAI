#include"Gummy.h"

Gummy::Gummy(){
	std::cout<<"constructing Gummy"<<std::endl;
	srand(time(NULL));
	//csvFileName = new char[20];
	std::cout << "\nChoose number of iterations to train: ";
	std::cin >> numIterations;
	std::cout << "\nChoose step size: ";
	std::cin >> stepSize;
	std::cout<< "\nput file into gummy.cpp folder and enter file name: ";
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.getline(csvFileName, 20);
}

csv* Gummy::readCSV(char* fileName) {
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
	file->name = fileName;
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
void Gummy::updateTrainingData(bool numbersOnly) {
	trainingData = readCSV(csvFileName);
	if(numbersOnly)
		csvToDouble(trainingData);
}
void Gummy::csvToDouble(csv* file) {
	std::cout << "------------------------CSV TO DOUBLE---------------\n";
	file->numData = new double*[file->numLines];
	for (int i = 0; i < file->numLines; i++) {
		//std::cout << "starting thing\n";
		int valCount = 1;
		for (int j = 0; j < file->data[i].size(); j++) {
			if(file->data[i].at(j) == ','){
				valCount++;
			}
		}
		file->numData[i] = new double[valCount];
		//std::cout << "after error thing\n";
		int commaPos = 0;
		int vals = 0;
		for (int j = 0; j < file->data[i].size(); j++) {
			if (file->data[i].at(j) == ',') {
				//std::cout << "\nfirst if";
				file->numData[i][vals] = std::stod(file->data[i].substr(commaPos, j - commaPos));
				std::cout<<file->numData[i][vals]<<',';
				commaPos = j+1;
				vals++;
			}
			else if (j == file->data[i].size() - 1) {
				//std::cout << "\nsecond if";
				file->numData[i][vals] = std::stod(file->data[i].substr(commaPos));
				std::cout<<file->numData[i][vals]<<',';
				commaPos = j;
				vals++;
			}
		}
		std::cout<<std::endl;
	}
}

DenseNet* Gummy::userInit() {
	std::cout<< "\nEnter the file name to save your net: ";
	std::cin.clear();
	//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.getline(netFileName, 20);
	std::cout << "\nWhat type of Neural net do you want?\n1: Dense, 2: Recurrent(NA), 3: convolutional\n";
	std::cin >> type;
	int numLayers = 0;
	int*layerSizes=NULL;
	bool sigmoidOutput = true;
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
	std::cout<<"FINISHED GATHERING DATA\n"<<csvFileName<<std::endl;
	DenseNet* net = new DenseNet(numLayers, layerSizes, sigmoidOutput, netFileName);
	//net->print();
	std::cout<<"reading file"<<std::endl;
	trainingData = readCSV(csvFileName);
	csvToDouble(trainingData);
	std::cout<<"FINISHED MAKING DENSE NET\n";
	return net;
}

void Gummy::train(DenseNet* net) {
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
	Matrix* netOutput = new Matrix(numOut, 1);
	std::cout<<"made training matrices"<<std::endl;
	for (int i = 0; i < numIterations; i++) {
		rowNum = rand() % trainingData->numLines;
		//std::cout<<"picked a row"<<std::endl;
		for(int j = 0; j < numIn; j++){
			inputMatrix->set(j,0,trainingData->numData[rowNum][j]);
		}
		for(int j = 0; j < numOut; j++){
			outputMatrix->set(j,0,trainingData->numData[rowNum][j+numIn]);
		}
		//std::cout<<"set input and outputs"<<std::endl;
		
		net->feedForward(inputMatrix);
		//std::cout<<"fed forward"<<std::endl;
		net->backProp(outputMatrix, stepSize);
		//std::cout<<"back propped"<<std::endl;
		if (i%percentNum == 0) {
			std::cout << "percent done: " << i / percentNum<<"%\n";
			double error = 0;
			for(int rowNum = 0; rowNum < trainingData->numLines; rowNum++){
				for(int j = 0; j < numIn; j++){
					inputMatrix->set(j,0,trainingData->numData[rowNum][j]);
				}
				for(int j = 0; j < numOut; j++){
					outputMatrix->set(j,0,trainingData->numData[rowNum][j+numIn]);
				}
				net->feedForward(inputMatrix);
				error+=net->calcError(outputMatrix);
			}
			std::cout<<"error: "<<error<<std::endl;
		}
	}
	for(int i = 0; i < 10; i++){
		rowNum = rand() % trainingData->numLines;
		std::cout<<"input: ";
		for(int j = 0; j < numIn; j++){
			inputMatrix->set(j,0,trainingData->numData[rowNum][j]);
			std::cout<<inputMatrix->get(j,0)<<", ";
		}
		std::cout<<std::endl;
		std::cout<<"desired output: ";
		for(int j = 0; j < numOut; j++){
			outputMatrix->set(j,0,trainingData->numData[rowNum][j+numIn]);
			std::cout<<outputMatrix->get(j,0)<<", ";
		}
		std::cout<<std::endl;
		std::cout<<"actual output: ";
		netOutput = net->feedForward(inputMatrix);
		for(int j = 0; j < netOutput->getM(); j++){
			std::cout<<netOutput->get(j,0)<<", ";
		}
		std::cout<<std::endl;
	}
	
}

void Gummy::saveNet(DenseNet* net){
	net->save();
}

DenseNet* Gummy::loadNet(char* fileName){
	csv* loadnet = readCSV(fileName);
	csvToDouble(loadnet);
	DenseNet* net = new DenseNet(loadnet);
}