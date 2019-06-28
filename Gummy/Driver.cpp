#include "Gummy.h"
#include<string>
bool checkWin(char player);
bool checkLegal(int pos, char player);
void setInput(Matrix* numberBoard, char turn);
void setData(int** data, int turn, Matrix* numData, int pos);
void playGames(DenseNet* n1, DenseNet* n2, int numGames, std::ofstream* of); //2 neural nets dueling
void playGames(DenseNet* net1, int numGames, std::ofstream* gameDataFile); //neural net vs random
char board[9];
	
int main(){
	
	for(int i = 0; i < 9;i++){
		board[i]='-';
	}
	//first 9 are player 1's
	//char ** 9 by 28 then 
    Gummy gummy = Gummy();
    std::cout<<"Gummy init done";
	int layerSizes[4];
	layerSizes[0]=9;
	layerSizes[1]=50;
	layerSizes[2]=50;
	layerSizes[3]=9;
	srand(time(0));
	int numNets = 1;
	
	DenseNet** nets = new DenseNet*[numNets];
	//nets[0]=gummy.loadNet("ticTacToeNet.csv");
	//nets[0]->print();
	
	for(int i = 0; i < numNets; i++){
		nets[i]=gummy.manualInit("gameData.csv", "ticTacToeNet.csv", 1, 4, layerSizes, true);
	}
	
	std::cin.get();
	std::ofstream* of = new std::ofstream;
	//of->open("gameData.csv");
	std::cout<<"done constructing nets 1 and 2. "<<std::endl;
	gummy.setCsvFileName("gameData.csv");
	std::cin.get();
	char save = 'n';
	for(int i = 0; i < 100; i++){
		of->open("gameData.csv");
		playGames(nets[0], 500, of);
		of->close();
		std::cout<<"save net? ";
		std::cin>>save;
		if(save){
			gummy.saveNet(nets[0]);
		}
		std::cout<<"updating training data"<<std::endl;
		gummy.updateTrainingData(true);
		std::cout<<"done updating"<<std::endl;
		//std::cin.get();
		for(int j = 0; j < numNets; j++){
			gummy.train(nets[j]);
		}
	}
	std::cout<<"done playing games"<<std::endl;
	gummy.saveNet(nets[0]);
    //DenseNet * userNet = gummy.userInit();
	//userNet->print();
	/*bool stillGoing = true;
    int numIterations=0;
    double stepSize=0;
	while(stillGoing){
		stillGoing=false;
		//gummy.train(userNet);
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
	*/
	//gummy.saveNet(userNet);
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
		data[turn][i+9]=(i==pos?1:0);
	}
}

void record(int** goodData, int**badData, std::ofstream* oFile, int numLines, int numBadLines){
	for(int i = 0; i < numLines; i++){
		for(int j = 0; j < 18; j++){
			if(j!=0) *oFile<<',';
			*oFile<<goodData[i][j];
		}
		*oFile<<std::endl;
	}
	
	for(int j = 0; j < 18; j++){
		if(j!=0) *oFile<<',';
		j<9?(*oFile<<badData[numBadLines-1][j]):(*oFile<<-badData[numBadLines-1][j]);
	}
	*oFile<<std::endl;
}

void playGames(DenseNet* net1, DenseNet* net2, int numGames, std::ofstream* gameDataFile){
	std::cout<<"setting up data"<<std::endl;
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
	std::cout<<"opening gameData.csv file"<<std::endl;
	//gameDataFile->open("gameData.csv");
	std::cout<<"creating matrices"<<std::endl;
	Matrix* numberBoard = new Matrix(9,1);
	Matrix* chosenSpace = new Matrix(9,1);
	std::cout<<"entering game loop"<<std::endl;

	DenseNet* n1;
	DenseNet* n2;
	bool firstPlayer = true;

	for(int ga = 0; ga < numGames; ga++){
		if(firstPlayer){
			n1 = net1;
			n2 = net2;
		}
		else{
			n1 = net2;
			n2 = net1;
		}
		firstPlayer = !firstPlayer;
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
			std::cout<<"finished setting game data"<<std::endl;

			while(!xwon&&!owon&&!cat){

				std::cout<<"setting input, p1: "<<p1<<std::endl;
				setInput(numberBoard, p1?'x':'o');
				//delete chosenSpace;
				std::cout<<"feeding forward"<<std::endl;
				chosenSpace = p1?(n1->feedForward(numberBoard)):(n2->feedForward(numberBoard));
				//chosenSpace->print();
				std::cout<<"choosing position"<<std::endl;
				int pos=0;
				double posAmount = 0;
				for(int i = 0; i < chosenSpace->getM(); i++) 
					if(chosenSpace->get(i,0)>posAmount){
						posAmount = chosenSpace->get(i,0);
						pos=i;
					}
				std::cout<<"position: "<<pos<<std::endl;
				std::cout<<"setting data"<<std::endl;
				setData(p1?n1data:n2data, p1?xTurn:oTurn, numberBoard, pos);
				turn++;
				p1?xTurn++:oTurn++;

				std::cout<<"printing game state at turn "<<turn-1<<std::endl;
				std::cout<<"player "<<(p1?'x':'o')<<", choice: "<<pos<<std::endl;
				for(int r = 0; r < 3; r++){
					for(int c = 0; c < 3; c++){
						std::cout<<board[r*3+c];
					}
					std::cout<<std::endl;
				}
				std::cout<<"checking legality and setting board"<<std::endl;
				(p1?owon:xwon) = !checkLegal(pos,p1?'x':'o'); //set's the board too
				if(p1?!owon:!xwon){
					(p1?xwon:owon) = checkWin(p1?'x':'o');
				} else if(!owon&&!xwon&&turn==8){
					cat=true;
				}
				p1=!p1;
				std::cout<<"done with turn "<<turn-1<<std::endl;
			}
			if(xwon){ 
				std::cout<<"recording x data"<<std::endl;
				record(n1data, n2data, gameDataFile, xTurn, oTurn);
			}
			else if(owon){
				std::cout<<"recording o data"<<std::endl;
				record(n2data, n1data, gameDataFile, oTurn, xTurn);
			}
			if(cat){
				std::cout<<"resetting from cat"<<std::endl;
				reset = true;
			} else{

			}
			for(int i = 0; i < 9; i++){
				board[i]='-';
			}
		}
	}
	delete numberBoard;
	//gameDataFile->close();
}
void playGames(DenseNet* net1, int numGames, std::ofstream* gameDataFile){
	std::cout<<"setting up data"<<std::endl;
	int numGamesWithCats = 0;
	int** n1data = new int*[5];
	for(int i = 0; i < 5; i++){
		n1data[i] = new int[18];
	}
	int** n2data = new int*[5];
	for(int i = 0; i < 5; i++){
		n2data[i] = new int[18];
	}
	int** data;
	int numDataPoints = 0;
	//std::cout<<"opening gameData.csv file"<<std::endl;
	//gameDataFile->open("gameData.csv");
	//std::cout<<"creating matrices"<<std::endl;
	Matrix* numberBoard = new Matrix(9,1);
	Matrix* chosenSpace = new Matrix(9,1);
	std::cout<<"entering game loop"<<std::endl;

	DenseNet* n1 = net1;
	bool firstPlayer = false;
	int netWins = 0;
	int randomWins = 0;
	for(int ga = 0; ga < numGames; ga++){
		firstPlayer = !firstPlayer;
		bool reset = true;
		while(reset){
			
			numGamesWithCats++;
			numDataPoints=0;
			reset = false;
			bool xwon= false;
			bool owon= false;
			bool cat = false;
			bool p1 = firstPlayer;
			int turn = 0, xTurn=0, oTurn=0;
			std::cout<<"finished setting game data"<<std::endl;

			while(!xwon&&!owon&&!cat){

				//std::cout<<"setting input, p1: "<<p1<<std::endl;
				setInput(numberBoard, p1?'x':'o');
				//numberBoard->print();
				//delete chosenSpace;
				std::cout<<"feeding forward"<<std::endl;
				int pos=0;
				double posAmount = 0;
				if(p1){
					std::cout<<"net turn"<<std::endl;
					chosenSpace = n1->feedForward(numberBoard);
					chosenSpace->print();
					for(int i = 0; i < chosenSpace->getM(); i++) 
						if(chosenSpace->get(i,0)>posAmount){
							posAmount = chosenSpace->get(i,0);
							pos=i;
						}
					std::cout<<"pos: "<<pos<<std::endl;
				} else{
					std::cout<<"Choose position for o: ";
					std::cin>>pos;
					/*do{
						pos=rand()%9;
					}while(board[pos]!='-');
					*/
				}
				//chosenSpace->print();
				//std::cout<<"choosing position"<<std::endl;
				
				
				std::cout<<"position: "<<pos<<std::endl;
				std::cout<<"setting data"<<std::endl;
				setData(p1?n1data:n2data, p1?xTurn:oTurn, numberBoard, pos);
				std::cout<<"xturn: "<<xTurn<<", oturn data: "<<*n1data[xTurn]<<std::endl;
				std::cout<<"oturn: "<<oTurn<<", oturn data: "<<*n1data[xTurn]<<std::endl;
				turn++;
				p1?xTurn++:oTurn++;

				std::cout<<"printing game state at turn "<<turn-1<<std::endl;
				std::cout<<"player "<<(p1?'x':'o')<<", choice: "<<pos<<std::endl;
				for(int r = 0; r < 3; r++){
					for(int c = 0; c < 3; c++){
						std::cout<<board[r*3+c];
					}
					std::cout<<std::endl;
				}
				std::cout<<"checking legality and setting board"<<std::endl;
				(p1?owon:xwon) = !checkLegal(pos,p1?'x':'o'); //set's the board too
				if(p1?!owon:!xwon){
					(p1?xwon:owon) = checkWin(p1?'x':'o');
				} else if(!owon&&!xwon&&turn>=8){
					std::cout<<"cat"<<std::endl;
					cat=true;
				}
				p1=!p1;
				std::cout<<"done with turn "<<turn-1<<std::endl;
				//std::cout<<"xturn: "<<xTurn<<", oturn data: "<<*n1data[xTurn-1]<<std::endl;
				//std::cout<<"oturn: "<<oTurn<<", oturn data: "<<*n2data[oTurn-1]<<std::endl;
			}
			if(xwon){ 
				netWins++;
				std::cout<<"recording x data"<<std::endl;
				record(n1data, n2data, gameDataFile, xTurn, oTurn);
			}
			else if(owon){
				randomWins++;
				std::cout<<"recording o data"<<std::endl;
				record(n2data, n1data, gameDataFile, oTurn, xTurn);
			}
			else if(cat){
				std::cout<<"resetting from cat"<<std::endl;
				reset = true;
			} else{
				std::cout<<"messed up coding win checker."<<std::endl;
			}
			
			for(int i = 0; i < 9; i++){
				board[i]='-';
			}
		}
	}
	std::cout<<"netWins: "<<netWins<<", randomWins "<<randomWins<<std::endl;
	//delete numberBoard;
	//gameDataFile->close();
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