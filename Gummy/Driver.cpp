#include "Gummy.h"
#include<string>
//#define printGames
bool checkWin(char player);
void printBoard();
bool checkLegal(int pos, char player);
void setInput(Matrix* numberBoard, char turn);
void setData(int** data, int turn, Matrix* numData, int pos);
void playGames(DenseNet* n1, DenseNet* n2, int numGames, std::ofstream* of); //2 neural nets dueling
void playGames(DenseNet* net1, int numGames, std::ofstream* gameDataFile); //neural net vs random
char board[9];
double maxWinRatio = 0;
bool best = false;
//bool lostFromIlligal = false;
	
int main(){
	bool load = true;
	std::cout<<"load or save? 1, 0: "<<std::endl;
	std::cin>>load;
	for(int i = 0; i < 9;i++){
		board[i]='-';
	}
	//first 9 are player 1's
	//char ** 9 by 28 then 
    Gummy gummy = Gummy();
    std::cout<<"Gummy init done";
	int layerSizes[4];
	layerSizes[0]=18;
	layerSizes[1]=90;
	layerSizes[2]=30;
	layerSizes[3]=9;
	srand(time(0));
	int numNets = 1;
	
	DenseNet** nets = new DenseNet*[numNets];
	for(int i = 0; i < numNets; i++){
		if(load)
			nets[i]=gummy.loadNet("ticTacToeNet.csv");
		else if(!load)
			nets[i]=gummy.manualInit("gameData.csv", "ticTacToeNet.csv", 1, 4, layerSizes, true);
	}
	std::cin.get();
	std::ofstream* of = new std::ofstream;
	//of->open("gameData.csv");
	std::cout<<"done constructing nets 1 and 2. "<<std::endl;
	gummy.setCsvFileName("gameData.csv");
	gummy.updateTrainingData(true);
	for(int j = 0; j < numNets; j++){
		//gummy.train(nets[j]);
		//gummy.train(nets[j]);
	}
	std::cin.get();
	while(true){
		playGames(nets[0], 1000, of);
		int trainingRounds = 10;
		std::cout<<"max win %"<<maxWinRatio<<std::endl;
		char save = 'n';
		std::cout<<"save net? ";
		std::cin>>save;
		if(save == 'y'){
			gummy.saveNet(nets[0]);
		}
		std::cout<<"How many rounds next time?";
		std::cin>>trainingRounds;
		if(trainingRounds == 0)
			break;
		for(int i = 0; i < trainingRounds; i++){
			of->open("gameData.csv");
			playGames(nets[0], 10000, of);
			of->close();
			if(best){
				gummy.saveNet(nets[0]);
				best = false;
			}
			std::cout<<"updating training data"<<std::endl;
			gummy.updateTrainingData(true);
			std::cout<<"done updating"<<std::endl;
			//std::cin.get();
			for(int j = 0; j < numNets; j++){
				gummy.train(nets[j]);
			}
		}
	}
	std::cout<<"done playing games"<<std::endl;
	//gummy.saveNet(nets[0]);
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout<<"done and authored by Timothy-Flavin"<<std::endl;
	std::cin.get();
    return 0;
}

bool checkWin(char player){
	
	if(board[0]==player && board[1]==player && board[2]==player){
		#ifndef printGames
		std::cout<<"player "<<player<<" won!"<<std::endl;
		printBoard();
		#endif
		return true;
	}
	else if(board[3]==player && board[4]==player && board[5]==player){
		#ifndef printGames
		std::cout<<"player "<<player<<" won!"<<std::endl;
		printBoard();
		#endif
		return true;
	}
	else if(board[6]==player && board[7]==player && board[8]==player){
		#ifndef printGames
		std::cout<<"player "<<player<<" won!"<<std::endl;
		printBoard();
		#endif
		return true;
	}

	else if(board[0]==player && board[3]==player && board[6]==player){
		#ifndef printGames
		std::cout<<"player "<<player<<" won!"<<std::endl;
		printBoard();
		#endif
		return true;
	}
	else if(board[1]==player && board[4]==player && board[7]==player){
		#ifndef printGames
		std::cout<<"player "<<player<<" won!"<<std::endl;
		printBoard();
		#endif
		return true;
	}
	else if(board[2]==player && board[5]==player && board[8]==player){
		#ifndef printGames
		std::cout<<"player "<<player<<" won!"<<std::endl;
		printBoard();
		#endif
		return true;
	}

	else if(board[0]==player && board[4]==player && board[8]==player){
		#ifndef printGames
		std::cout<<"player "<<player<<" won!"<<std::endl;
		printBoard();
		#endif
		return true;
	}
	else if(board[2]==player && board[4]==player && board[6]==player){
		#ifndef printGames
		std::cout<<"player "<<player<<" won!"<<std::endl;
		printBoard();
		#endif
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
	//std::cout<<"settingInpout with new number board"<<std::endl;
	for(int i = 0; i < 9; i++){
		if(turn == 'x')
			switch (board[i]){
				case 'x':
					numberBoard->set(i+9,0,1);
					numberBoard->set(i,0,0);
				break;
				case 'o':
					numberBoard->set(i,0,1);
					numberBoard->set(i+9,0,0);
				break;
				case '-':
					numberBoard->set(i,0,0);
					numberBoard->set(i+9,0,0);
				break;
				default:
					std::cout<<"bruh wtf how did this happen, board not = x o or -"<<std::endl;
				break;
			}
		else{
			switch (board[i]){
				case 'x':
					numberBoard->set(i,0,1);
					numberBoard->set(i+9,0,0);
				break;
				case 'o':
					numberBoard->set(i+9,0,1);
					numberBoard->set(i,0,0);
				break;
				case '-':
					numberBoard->set(i,0,0);
					numberBoard->set(i+9,0,0);
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
		data[turn][i+9]=numData->get(i+9,0);
		data[turn][i+18]=(i==pos?1:0);
	}
}

void record(int** goodData, int**badData, std::ofstream* oFile, int numLines, int numBadLines){
	for(int i = 0; i < numLines; i++){
		for(int j = 0; j < 27; j++){
			if(j!=0) *oFile<<',';
			*oFile<<goodData[i][j];
		}
		*oFile<<std::endl;
	}
	/*
	if(lostFromIlligal){
		for(int j = 0; j < 27; j++){
			if(j!=0) *oFile<<',';
			j<18?(*oFile<<badData[numBadLines][j]):(*oFile<<!badData[numBadLines][j]);
		}
		*oFile<<std::endl;
		lostFromIlligal = false;
	}
	*/
	
}

void playGames(DenseNet* net1, int numGames, std::ofstream* gameDataFile){
	//std::cout<<"setting up data"<<std::endl;
	int numGamesWithCats = 0;
	int** n1data = new int*[5];
	for(int i = 0; i < 5; i++){
		n1data[i] = new int[27];
	}
	int** n2data = new int*[5];
	for(int i = 0; i < 5; i++){
		n2data[i] = new int[27];
	}
	int** data;
	int numDataPoints = 0;
	//std::cout<<"opening gameData.csv file"<<std::endl;
	//gameDataFile->open("gameData.csv");
	//std::cout<<"creating matrices"<<std::endl;
	Matrix* numberBoard = new Matrix(18,1);
	Matrix* chosenSpace = new Matrix(9,1);
	//std::cout<<"entering game loop"<<std::endl;

	DenseNet* n1 = net1;
	bool firstPlayer = false;
	int netWins = 0;
	int randomWins = 0;
	int catGames = 0;
	for(int ga = 0; ga < numGames; ga++){
		if(ga%1000==0)
			std::cout<<"game: "<<ga<<std::endl;
		firstPlayer = !firstPlayer;
		bool reset = true;
		while(reset){
			
			//numGamesWithCats++;
			numDataPoints=0;
			reset = false;
			bool xwon= false;
			bool owon= false;
			bool cat = false;
			bool wonGame = false;
			bool p1 = firstPlayer;
			int turn = 0, xTurn=0, oTurn=0;
		#ifndef printGames
			std::cout<<"finished setting game data"<<std::endl;
		#endif
			while(!xwon&&!owon&&!cat){

				//std::cout<<"setting input, p1: "<<p1<<std::endl;
				setInput(numberBoard, p1?'x':'o');
				//numberBoard->print();
				//delete chosenSpace;
				#ifndef printGames
				std::cout<<"feeding forward"<<std::endl;
				#endif
				int pos=0;
				double posAmount = 0;
				if(p1){
					#ifndef printGames
					std::cout<<"net turn"<<std::endl;
					#endif
					//n1->print();
					chosenSpace = n1->feedForward(numberBoard);
					//chosenSpace->print();
					for(int i = 0; i < chosenSpace->getM(); i++) 
						if(chosenSpace->get(i,0)>posAmount){
							posAmount = chosenSpace->get(i,0);
							pos=i;
						}
					#ifndef printGames
					std::cout<<"pos: "<<pos<<std::endl;
					#endif
					/*
					bool chosen = false;
					std::cout<<"------------------testAI Picking--------------"<<std::endl;
					for(int i = 0; i < 9; i++){
						if(board[i]=='-'){
							board[i]='o';
							if(checkWin('o')){
								pos = i;
								chosen=true;
							}
							else if (!chosen){
								board[i]='x';
								if(checkWin('x')){
									pos=i;
									chosen = true;
								}
								else{
									board[i]='-';
								}
							}
							board[i]='-';
						}
					}
					if(chosen == false){
						do{
							pos=rand()%9;
						}while(board[pos]!='-');
					}
					
					std::cout<<"------------------testAI Done Picking--------------"<<std::endl;
					*/
				
				} else{
					
					std::cout<<"Choose position for o: ";
					std::cin.clear();
					std::cin>>pos;
					
					/*
					do{
						pos=rand()%9;
					}while(board[pos]!='-');
					*/
				    /*
					bool chosen = false;
					#ifndef printGames
					std::cout<<"------------------DumbAI Picking--------------"<<std::endl;
					#endif
					for(int i = 0; i < 9; i++){
						if(board[i]=='-'){
							board[i]='o';
							if(checkWin('o')){
								pos = i;
								chosen=true;
							}
							else if (!chosen){
								board[i]='x';
								if(checkWin('x')){
									pos=i;
									chosen = true;
								}
								else{
									board[i]='-';
								}
							}
							board[i]='-';
						}
					}
					if(chosen == false){
						int choices[5];
						choices[0]=0;
						choices[1]=2;
						choices[2]=4;
						choices[3]=6;
						choices[4]=8;
						if(turn <= 1)
							do{
								pos=choices[rand()%5];
							}while(board[pos]!='-');
						else{
							do{
								pos=rand()%9;
							}while(board[pos]!='-');
						}
					}
					*/
					#ifndef printGames
					std::cout<<"------------------DumbAI Done Picking--------------"<<std::endl;
					#endif
				}
				//chosenSpace->print();
				//std::cout<<"choosing position"<<std::endl;
				
				#ifndef printGames
				std::cout<<"position: "<<pos<<std::endl;
				std::cout<<"setting data"<<std::endl;
				#endif
				setData(p1?n1data:n2data, p1?xTurn:oTurn, numberBoard, pos);
				#ifndef printGames
				std::cout<<"xturn: "<<xTurn<<", oturn data: "<<*n1data[xTurn]<<std::endl;
				std::cout<<"oturn: "<<oTurn<<", oturn data: "<<*n1data[xTurn]<<std::endl;
				#endif
				turn++;
				p1?xTurn++:oTurn++;

				#ifndef printGames
				std::cout<<"checking legality and setting board"<<std::endl;
				#endif
				(p1?owon:xwon) = !checkLegal(pos,p1?'x':'o'); //set's the board too
				if(p1?!owon:!xwon){
					(p1?xwon:owon) = checkWin(p1?'x':'o');
					wonGame = p1?xwon:owon;
				} 
				if(!owon&&!xwon&&turn>=8){
					#ifndef printGames
					std::cout<<"cat"<<std::endl;
					#endif
					if(!p1){
						//xwon=true;
						//netWins--;
					}
					if(p1){
						//randomWins--;
						//owon=true;
					}
					cat=true;
				}

				#ifndef printGames
				std::cout<<"printing game state at turn "<<turn-1<<std::endl;
				std::cout<<"player "<<(p1?'x':'o')<<", choice: "<<pos<<std::endl;
				for(int r = 0; r < 3; r++){
					for(int c = 0; c < 3; c++){
						std::cout<<board[r*3+c];
					}
					std::cout<<std::endl;
				}
				#endif
				p1=!p1;
				//std::cout<<"done with turn "<<turn-1<<std::endl;
				//std::cout<<"xturn: "<<xTurn<<", oturn data: "<<*n1data[xTurn-1]<<std::endl;
				//std::cout<<"oturn: "<<oTurn<<", oturn data: "<<*n2data[oTurn-1]<<std::endl;
			}
			if(xwon){ 
				netWins++;
				#ifndef printGames
				std::cout<<"recording x data"<<std::endl;
				#endif
				record(n1data, n2data, gameDataFile, xTurn, oTurn);
			}
			else if(owon){
				randomWins++;
				#ifndef printGames
				std::cout<<"recording o data"<<std::endl;
				#endif
				record(n2data, n1data, gameDataFile, oTurn, xTurn);
			}
			if(cat){
				#ifndef printGames
				std::cout<<"resetting from cat"<<std::endl;
				#endif
				reset = true;
				catGames++;
			} 
			
			for(int i = 0; i < 9; i++){
				board[i]='-';
			}
		}
	}
	double tempwr = (1.0*netWins)/(netWins+randomWins);
	if(tempwr>maxWinRatio){
		maxWinRatio = tempwr;
		best = true;
	}
	std::cout<<"netWins: "<<netWins<<", randomWins "<<randomWins<<", cat Games:"<<catGames<<std::endl;
	//delete numberBoard;
	//gameDataFile->close();
}

void printBoard(){
	for(int r = 0; r < 3; r++){
		for(int c = 0; c < 3; c++){
			std::cout<<board[r*3+c];
		}
		std::cout<<std::endl;
	}
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