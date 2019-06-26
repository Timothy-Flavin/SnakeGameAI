#include "Gummy.h"
#include<string>
bool checkWin(char player);
bool checkLegal(int pos, char player);
void setInput(Matrix* numberBoard, char turn);
void setData(int** data, int turn, Matrix* numData, int pos);
void playGames(DenseNet* n1, DenseNet* n2, int numGames);
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
	if(board[0]==player && board[1]==player && board[2]==player){
		return true;
	}
	else if(board[3]==player && board[4]==player && board[5]==player){
		return true;
	}
	else if(board[6]==player && board[7]==player && board[8]==player){
		return true;
	}

	else if(board[0]==player && board[3]==player && board[6]==player){
		return true;
	}
	else if(board[1]==player && board[4]==player && board[7]==player){
		return true;
	}
	else if(board[2]==player && board[5]==player && board[8]==player){
		return true;
	}

	else if(board[0]==player && board[4]==player && board[8]==player){
		return true;
	}
	else if(board[2]==player && board[4]==player && board[6]==player){
		return true;
	} else{
		return false;
	}
}
bool checkLegal(int pos, char player){
	if(board[pos]=='-'){
		board[pos]=player;
		return true;
	} else{
		return false;
	}
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

void record(int** data, std::ofstream* oFile, int numLines){
	for(int i = 0; i < numLines; i++){
		for(int j = 0; j < 9; j++){
			if(j!=0) *oFile<<',';
			*oFile<<data[i][j];
		}
		*oFile<<std::endl;
	}
}

void playGames(DenseNet* n1, DenseNet* n2, int numGames){
	
	int numGamesWithCats = 0;
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
	std::ofstream* gameDataFile;
	gameDataFile->open("gameData.csv");
	Matrix* numberBoard = new Matrix(9,1);
	Matrix* chosenSpace = new Matrix(9,1);
	for(int ga = 0; ga < numGames; ga++){
		bool reset = true;
		while(reset){
			numGamesWithCats++;
			numDataPoints=0;
			reset = false;
			bool xwon= false;
			bool owon= false;
			bool cat = false;
			bool p1 = true;
			int turn = 0, xTurn=0, oTurn=0;
			
			while(!xwon&&!owon&&!cat){
				setInput(numberBoard, p1?'x':'o');
				delete chosenSpace;
				chosenSpace = p1?n1->feedForward(numberBoard):n2->feedForward(numberBoard);
				int pos=0;
				for(int i = 0; i < chosenSpace->getM(); i++) 
					pos=chosenSpace->get(i,0)>pos?i:pos;
				setData(p1?n1data:n2data, p1?xTurn:oTurn, numberBoard, pos);
				turn++;
				p1?xTurn++:oTurn++;
				(p1?owon:xwon) = !checkLegal(pos,p1?'x':'o'); //set's the board too
				if(p1?!owon:!xwon){
					(p1?xwon:owon) = checkWin(p1?'x':'o');
				} else if(!owon&&!xwon&&turn==8){
					cat=true;
				}
				p1=!p1;
			}
			if(xwon){ 
				record(n1data, gameDataFile, xTurn);
			}
			else if(owon){
				record(n2data, gameDataFile, oTurn);
			}
			if(cat){
				reset = true;
			} else{

			}
		}
		for(int i = 0; i < numDataPoints; i++){
			std::cout<<data[i]<<std::endl;
		}
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