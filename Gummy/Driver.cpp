#include "Gummy.h"
#include<string>
bool checkWin(int pos);
bool checkLose(int pos);
void checkMove(Matrix* output);
char board[9];
	
int main(){
	for(int i = 0; i < 9;i++){
		board[i]='-';
	}
	//first 9 are player 1's
	//char ** 9 by 28 then 
    Gummy gummy = Gummy();
    std::cout<<"Gummy init done";
    DenseNet * userNet = gummy.userInit();
	userNet->print();
	bool stillGoing = true;
    int numIterations=0;
    double stepSize=0;
	while(stillGoing){
		stillGoing=false;
		gummy.train(userNet);
		std::cout<<"would you like to train more? y/n"<<std::endl;
		char ans = 'a';
		std::cin>>ans;
		if(ans == 'y'){
			stillGoing = true;
			std::cout<<"How many iterations? ";
			std::cin>>numIterations;
            gummy.setNumIterations(numIterations);
			std::cout<<"step size: ";
			std::cin>>stepSize;
            gummy.setStepSize(stepSize);
		}
	}
	gummy.saveNet(userNet);
	//DenseNet* copy = gummy.loadNet(userNet->getName());
	//copy->setName("copy.csv");
	//gummy.saveNet(copy);
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout<<"done and authored by Timothy-Flavin"<<std::endl;
	std::cin.get();
    return 0;
}

bool checkWin(char player){

}
bool checkLegal(int pos, char player){

}

void setInput(Matrix* numberBoard, char turn){
	for(int i = 0; i < numberBoard->getM(); i++){
		if(turn == 'x')
			switch (board[i]){
				case 'x':
					numberBoard->set(i,0,1);
				break;
				case 'o':
					numberBoard->set(i,0,-1);
				break;
				case '-':
					numberBoard->set(i,0,0);
				break;
				default:
					std::cout<<"bruh wtf how did this happen, board not = x o or -"<<std::endl;
				break;
			}
		else{
			switch (board[i]){
				case 'x':
					numberBoard->set(i,0,-1);
				break;
				case 'o':
					numberBoard->set(i,0,1);
				break;
				case '-':
					numberBoard->set(i,0,0);
				break;
				default:
					std::cout<<"bruh wtf how did this happen, board not = x o or -"<<std::endl;
				break;
			}
		}
	}
}

void setData(int** data, int turn, Matrix* numData, int pos){
	for(int i = 0; i < 9; i++){
		data[turn][i]=numData->get(i,0);
		data[turn][i+9]=i==pos?1:0;
	}
}

void playGames(DenseNet* n1, DenseNet* n2, int numGames){
	bool reset = true;
	int** n1data = new int*[5];
	for(int i = 0; i < 5; i++){
		n1data[i] = new int[18];
	}
	int** n2data = new int*[4];
	for(int i = 0; i < 4; i++){
		n2data[i] = new int[18];
	}
	int** data;
	int numDataPoints = 0;
	Matrix* numberBoard = new Matrix(9,1);
	Matrix* chosenSpace = new Matrix(9,1);
	while(reset){
		numDataPoints=0;
		reset = false;
		bool xwon= false;
		bool owon= false;
		bool cat = false;
		int turn = 0, xTurn=0, oTurn=0;
		
		while(!xwon&&!owon&&!cat){
			setInput(numberBoard, 'x');
			delete chosenSpace;
			chosenSpace = n1->feedForward(numberBoard);
			int pos=0;
			for(int i = 0; i < chosenSpace->getM(); i++) 
				pos=chosenSpace->get(i,0)>pos?i:pos;
			setData(n1data, xTurn, numberBoard, pos);
			turn++;
			xTurn++;
			owon = !checkLegal(pos,'x'); //set's the board too
			if(!owon){
				xwon = checkWin('x');
			} else if(!owon&&!xwon&&turn==8){
				cat=true;
			}

			if(!xwon&&!cat&&!owon){
				setInput(numberBoard, 'o');
				delete chosenSpace;
				chosenSpace = n2->feedForward(numberBoard);
				pos=0;
				for(int i = 0; i < chosenSpace->getM(); i++) 
					pos=chosenSpace->get(i,0)>pos?i:pos;
				setData(n2data, oTurn, numberBoard, pos);
				turn++;
				oTurn++;
				xwon = !checkLegal(pos,'o');
				if(!xwon){
					owon = checkWin('o');
				}
			}
		}
		if(xwon){ 
			data = n1data;
			numDataPoints = xTurn;
		}
		else if(owon){
			data = n2data;
			numDataPoints = oTurn;
		}
		else if(cat){
			reset = true;
		}
	}
	for(int i = 0; i < numDataPoints; i++){
		std::cout<<data[i]<<std::endl;
	}
	delete numberBoard;
}
/*
std::ofstream outfile;
	outfile.open(name);
	
	if(outfile.is_open()){
		outfile<<numLayers<<','<<sigmoidOutput<<',';
		for(int i = 0; i < numLayers; i++){
			if(i>0)outfile<<',';
			outfile<<layerList[i];
		}
		outfile<<std::endl;

Game flow
nets n1, n2
char** n1data, n2data. 4-5 rows 19 cols x,o,-,0: 0 is unused turns
1,2,3,4,5,6,7,8,9,output
1000 games of data at a time?

n1 takes turn
	1 n1 alters board
		1 cat
			new game no recording
		2 n2's turn
			add to n1's data
	2 n1 wins
		add to n1's data,
		set n1's data as training data
	3 n1 loses
		set n2's data as training data

n2 takes turn
	1 n2 alters board
		add to n2's data
	2 n2 wins
		add to n2's data 
		set n2's data as training data
	3 n2 loses
		set n1's data as training data

*/