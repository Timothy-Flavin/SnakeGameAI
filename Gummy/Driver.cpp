#include "Gummy.h"
#include<string>
#define debugInputs
#define debugGameState
struct SnakePart {
	public:
		int x = 0;
		int y = 0;
		//bool active = false;
};
int fruitX = 0;
int fruitY = 0;
const int WIDTH = 20;
const int HEIGHT = 20;
char board[WIDTH*HEIGHT];
SnakePart snake[WIDTH*HEIGHT];
int snakeLength = 1;
int dir = 0;
bool lost = false;
bool printGames = false;
bool continueFile = true;
double reward = 0;
int distance = 100;
int snakeVisionY=5;
int snakeVisionX=5;

void update();
void init();
void clearBoard();
void printBoard();
void resetSnake();
void spawnFruit();
void updateSnake();
void updateBoard();
void updateGameState();
void makeMove(Matrix*, Matrix*, DenseNet*);
void playGames(DenseNet*, Matrix*, Matrix*, int, std::ofstream*, int);
void setInputs(Matrix*);
	
int main(){
	srand(time(0));
	bool load = true;
	std::cout<<"load or save? 1, 0: "<<std::endl;
	std::cin>>load; 
    Gummy gummy = Gummy();
    std::cout<<"Gummy init done";
	int layer0 = (snakeVisionX*2+1)*(snakeVisionY*2+1)+2;
	int layerSizes[4];
	layerSizes[0]=layer0;
	layerSizes[1]=50;
	layerSizes[2]=20;
	layerSizes[3]=4;
	DenseNet* nets;
	if(load)
		nets=gummy.loadNet("snakeNet.csv");
	else if(!load)
		nets=gummy.manualInit("gameData.csv", "snakeNet.csv", 1, 4, layerSizes, true);
	std::cin.get();
	std::ofstream* of = new std::ofstream;
	Matrix* choice = new Matrix(4,1);
	Matrix inputs = Matrix(layer0,1);

	char yesNo = 'n';
	std::cout<<"Do you want gummy to train first? (y,n) ";
	std::cin>>yesNo;
	if(yesNo == 'y'){
		gummy.train(nets);
		std::cout<<"Do you want to save the net? (y,n) ";
		std::cin>>yesNo;
		if(yesNo == 'y')
			gummy.saveNet(nets);
	}

	std::cout<<"do you want to clear the gameDataFile?(y,n) ";
	std::cin>>yesNo;
	if(yesNo == 'y'){
		continueFile=false;
		of->open("gameData.csv");
		for(int i = 0; i < layer0;i++){
			(*of)<<"0,";
		}
		(*of)<<"0";
		of->close();
	} else{
		continueFile = true;
	}
	int ga = 0;
	std::cout<<"how many games do you want to play? ";
	std::cin>>ga;

	std::cout<<"What kind of player do you want to play? \n(human: 0, NeuralNet: 1, Random: 2) ";
	int type = 0;
	std::cin>>type;

	std::cout<<"Do you want to print the games to the screen?(No: 0, Yes: 1) ";
	std::cin>>printGames;

	playGames(nets, choice, &inputs, ga, of, type);

	gummy.updateTrainingData(true);
	gummy.train(nets);
	char save = 'n';
	std::cout<<"save? (y,n)";
	std::cin>>save;
	if(save == 'y')
		gummy.saveNet(nets);

	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout<<"done and authored by Timothy-Flavin"<<std::endl;
	std::cin.get();
    return 0;
}

void setInputs(Matrix* inputs){
	int spaceValue;
	//std::cout<<"Setting inputs"<<std::endl;
	for(int y = snake[0].y-snakeVisionY; y <= snake[0].y+snakeVisionY; y++){
		for(int x = snake[0].x-snakeVisionX; x <= snake[0].x+snakeVisionX; x++){
			if(y>-1&&y<HEIGHT && x>-1&&x<WIDTH){
				//std::cout<<"y: "<<y<<", x: "<<x<<std::endl;
				switch(board[y*WIDTH+x]){
					case ' ':
						spaceValue = 0;
					break;
					case 'O':
						spaceValue = 1;
					break;
					case 'o':
						spaceValue = -1;
					break;
					case '@':
						spaceValue = 1;
					break;
				}
			}
			else{
				spaceValue=-1;
			}
			//std::cout<<"pos: "<<(y-(snake[0].y-snakeVisionY))*(snakeVisionX*2+1)+(x-(snake[0].x-snakeVisionX))<<", space: "<<spaceValue<<',';
			#ifndef debugInputs
			if(spaceValue==0||spaceValue==1)
				std::cout<<" ";
			std::cout<<spaceValue<<",";
			#endif
			inputs->set((y-(snake[0].y-snakeVisionY))*(snakeVisionX*2+1)+x-(snake[0].x-snakeVisionX),0, spaceValue);
			#ifndef debugInputs
			std::cout<<"space '"<<y*WIDTH+x<<"', value: "<<spaceValue<<std::endl;
			#endif
		}
		//std::cout<<std::endl;
	}
	
	inputs->set((snakeVisionY*2+1)*(snakeVisionX*2+1),0,(fruitX-snake[0].x)*1.0/WIDTH);
	#ifndef debugInputs
	std::cout<<"(fruitX-snakeX)/Width = "<<inputs->get(12,0)<<std::endl;
	#endif
	inputs->set((snakeVisionY*2+1)*(snakeVisionX*2+1)+1,0,(fruitY-snake[0].y)*1.0/HEIGHT);
	#ifndef debugInputs
	std::cout<<"(fruitY-snakeY)/Height = "<<inputs->get(13,0)<<std::endl;
	#endif
	
}

void makeMove(Matrix* choice, Matrix* inputs, DenseNet* nets){
	choice = nets->feedForward(inputs);
	int choiceNum = 0;
	double choiceAmount = 0;
	for(int i = 0; i < choice->getM(); i++){
		if(choice->get(i,0)>choiceAmount){
			choiceAmount = choice->get(i,0);
			choiceNum = i;
		}
	} 
	dir = choiceNum;
}

void playGames(DenseNet* nets, Matrix* choice, Matrix* inputs, int numGames, std::ofstream* of, int playerType){
	of->open ("gameData.csv", std::fstream::in | std::fstream::out | std::fstream::app);
	int numTurns = 100;//abs(snake[0].x-fruitX)+abs(snake[0].y-fruitY)+5+snakeLength;
	const int NUM_INPUTS = inputs->getM()+choice->getM();
	double dataToPrint[NUM_INPUTS*(WIDTH*HEIGHT+WIDTH+HEIGHT)];
	bool takenTurn = false;
	for(int ga = 0; ga < numGames; ga++){
		if(ga%100==0)
			std::cout<<"Games played: "<<ga<<std::endl;
		bool takenTurn = false;
		//std::cout<<"-----------------------------GAME START----------------------"<<std::endl;
		int turnNumber = 0;
		init();
		if(printGames)printBoard();
		lost = false;
		int oldLength = snakeLength;
		numTurns = 100;//abs(snake[0].x-fruitX)+abs(snake[0].y-fruitY)+5+snakeLength;
		while(!lost){
			#ifndef debugGameState
			std::cout<<"setting data inputs and getting dir"<<std::endl;
			#endif
			setInputs(inputs);
			#ifndef debugGameState
			std::cout<<"Finished setting inputs"<<std::endl;
			#endif
			if(printGames || playerType == 0)
				std::cin.get();
			if(playerType == 0){
				char cdir = ' ';
				std::cin>>cdir;
				if(cdir == 'w') dir = 0;
				else if(cdir=='d') dir = 1;
				else if(cdir=='s') dir = 2;
				else if(cdir=='a') dir = 3;
			} else if (playerType == 1){
				makeMove(choice, inputs, nets);
			} else if (playerType == 2){
				dir = rand()%4;
			} else{
				std::cout<<"invalid player type"<<std::endl;
			}
			#ifndef debugGameState
			std::cout<<"between picking move and update"<<std::endl;
			#endif
			update();
			#ifndef debugGameState
			std::cout<<"between update and spawn fruit"<<std::endl;
			#endif
			if(snakeLength>oldLength){
				spawnFruit();
			}
			if(printGames)printBoard();
			#ifndef debugGameState
			std::cout<<"between print board and finding max turns"<<std::endl;
			#endif
			if(turnNumber==numTurns-1){
				lost = true;
				//std::cout<<"lost because of turns"<<std::endl;
				reward=0;
			}
			//Setting Data To print each turn
			#ifndef debugGameState
			std::cout<<"Setting data to print"<<std::endl;
			#endif
			for(int i = 0; i < inputs->getM();i++){
				dataToPrint[(inputs->getM()+choice->getM())*(turnNumber)+i]=inputs->get(i,0);
			}
			for(int i = 0; i < choice->getM();i++){
				if(i == dir){
					dataToPrint[(inputs->getM()+choice->getM())*(turnNumber)+i+inputs->getM()]=reward;
					//std::cout<<i+1<<"/"<<choice->getM()<<" i=dir, setting data at: "<<((inputs->getM()+choice->getM())*(turnNumber)+i+inputs->getM())<<", to: "<<dataToPrint[(inputs->getM()+choice->getM())*(turnNumber)+i+inputs->getM()]<<std::endl;
				}
				else{
					dataToPrint[(inputs->getM()+choice->getM())*(turnNumber)+i+inputs->getM()]=0;
					//std::cout<<i+1<<"/"<<choice->getM()<<" i!=dir, setting data at: "<<((inputs->getM()+choice->getM())*(turnNumber)+i+inputs->getM())<<", to: "<<dataToPrint[(inputs->getM()+choice->getM())*(turnNumber)+i+inputs->getM()]<<std::endl;					
				}
			}
			turnNumber++;
			#ifndef debugGameState
			std::cout<<"Printing to file"<<std::endl;
			#endif
			//print data
			if(snakeLength > oldLength || lost){
				//std::cout<<"turn number before printing data to file: "<<turnNumber<<std::endl;
				for(int i = 0; i < turnNumber; i++){
					if(i == 0 && continueFile)
						(*of)<<std::endl;
					else if(i!=0)
						(*of)<<std::endl;
					for(int j = 0; j < (inputs->getM()+choice->getM()); j++){
						//std::cout<<"data: "<<dataToPrint[i*(inputs->getM()+choice->getM())+j]<<", at: "<<i<<"*"<<inputs->getM()+choice->getM()<<"+"<<j<<", or  "<<i*(inputs->getM()+choice->getM())+j<<std::endl;
						if(j>0) (*of)<<',';
						(*of)<<dataToPrint[i*(inputs->getM()+choice->getM())+j];
					}
				}
				oldLength = snakeLength;
				turnNumber = 0;
				numTurns = 100;//abs(snake[0].x-fruitX)+abs(snake[0].y-fruitY)+5+snakeLength;
			}
			//std::cout<<"printed turn, turn:"<<turnNumber<<std::endl;
			
		}
		//std::cout<<"-----------------------------GAME END----------------------"<<std::endl;
	
	}
	of->close();
}

void printBoard(){
	for(int w = 0; w < WIDTH+2; w++){
			std::cout<<"--";
	}
	std::cout<<std::endl;
	for(int h = 0; h < HEIGHT; h++){
		std::cout<<" |";
		for(int w = 0; w < WIDTH; w++){
			std::cout<<' '<<board[h*WIDTH+w];
		}
		std::cout<<'|'<<std::endl;
	}
	for(int w = 0; w < WIDTH+2; w++){
			std::cout<<"--";
	}
	std::cout<<std::endl;
}

void init(){
	clearBoard();
	resetSnake();
	spawnFruit();
	updateBoard();
}

void clearBoard(){
	for(int h = 0; h < HEIGHT; h++){
		for(int w = 0; w < WIDTH; w++){
			board[h*WIDTH+w]=' ';
		}
	}
}

void resetSnake(){
	snakeLength = 3;
	snake[0].x = rand()%WIDTH;
	snake[0].y = rand()%HEIGHT;
	dir = 0;
	for(int i = 1; i < WIDTH*HEIGHT; i++){
		snake[i].x = snake[0].x;
		snake[i].y = snake[0].y;
		//dir = 0;
	}
}

void spawnFruit(){
	do{
		fruitX = rand()%WIDTH;
		fruitY = rand()%HEIGHT;
		//std::cout<<fruitX<<", "<<fruitY<<std::endl;
	}while(board[fruitY*WIDTH+fruitX]!=' ');
	board[fruitY*WIDTH+fruitX]='@';
}

void update(){
	#ifndef debugGameState
	std::cout<<"updating snake"<<std::endl;
	#endif
	updateSnake();
	#ifndef debugGameState
	std::cout<<"updating game state"<<std::endl;
	#endif
	updateGameState();
	#ifndef debugGameState
	std::cout<<"updating board"<<std::endl;
	#endif
	updateBoard();
	#ifndef debugGameState
	std::cout<<"done with update"<<std::endl;
	#endif
}

void updateSnake(){
	for(int i = snakeLength-1; i > 0; i--){
		snake[i].x = snake[i-1].x;
		snake[i].y = snake[i-1].y;
	}
	distance = abs(snake[0].x-fruitX)+abs(snake[0].y-fruitY);
	switch(dir){
		case 0:
			snake[0].y--;			
		break;
		case 1:
			snake[0].x++;			
		break;
		case 2:
			snake[0].y++;			
		break;
		case 3:
			snake[0].x--;
		break;
	}
}

void updateBoard(){
	clearBoard();
	board[fruitY*WIDTH + fruitX] = '@';
	if(snake[0].y<HEIGHT && snake[0].x<WIDTH)
		board[snake[0].y*WIDTH + snake[0].x]='O';
	for(int i = 1; i<snakeLength;i++){
		board[snake[i].y * WIDTH + snake[i].x] = 'o';
	}
}

void updateGameState(){
	if(snake[0].y<0||snake[0].y>=HEIGHT||snake[0].x<0||snake[0].x>=WIDTH||board[snake[0].y*WIDTH+snake[0].x]=='o'){
		lost=true;
		reward=-1;
	}
	else if(fruitX==snake[0].x&&fruitY==snake[0].y){
		snake[snakeLength].x=snake[snakeLength-1].x;
		snake[snakeLength].y=snake[snakeLength-1].y;
		snakeLength++;
		reward=2;
	} 
	else if(distance>abs(snake[0].x-fruitX)+abs(snake[0].y-fruitY)){
		reward=1;
	}
	else{
		reward=0.1;
	}
}