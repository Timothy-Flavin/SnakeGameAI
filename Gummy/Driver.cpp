#include "Gummy.h"
#include<string>
//#define printGames
struct SnakePart {
	public:
		int x = 0;
		int y = 0;
		int dir = 0;
		bool active = false;
};
const int WIDTH = 20;
const int HEIGHT = 20;
int board[WIDTH*HEIGHT];
SnakePart snake[WIDTH*HEIGHT];


void init();
void clearBoard();
void printBoard();
void resetSnake();
	
int main(){
	bool load = true;
	std::cout<<"load or save? 1, 0: "<<std::endl;
	std::cin>>load;
	//first 9 are player 1's
	//char ** 9 by 28 then 
    Gummy gummy = Gummy();
    std::cout<<"Gummy init done";
	int layerSizes[4];
	layerSizes[0]=27;
	layerSizes[1]=243;
	layerSizes[2]=9;
	srand(time(0));
	int numNets = 1;
	
	DenseNet** nets = new DenseNet*[numNets];
	for(int i = 0; i < numNets; i++){
		if(load)
			nets[i]=gummy.loadNet("snakeNet.csv");
		else if(!load)
			nets[i]=gummy.manualInit("gameData.csv", "snakeNet.csv", 1, 3, layerSizes, true);
	}
	std::cin.get();
	std::ofstream* of = new std::ofstream;
	gummy.setCsvFileName("gameData.csv");
	gummy.updateTrainingData(true);
	std::cin.get();
	
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout<<"done and authored by Timothy-Flavin"<<std::endl;
	std::cin.get();
    return 0;
}

void printBoard(){
	for(int w = 0; w < WIDTH; w++){
			std::cout<<'-';
		}
	for(int h = 0; h < HEIGHT; h++){
		std::cout<<'|';
		for(int w = 0; w < WIDTH; w++){
			std::cout<<board[h*3+w];
		}
		std::cout<<'|'<<std::endl;
	}
	for(int w = 0; w < WIDTH; w++){
			std::cout<<'-';
	}
}

void init(){
	clearBoard();
	resetSnake();
}

void clearBoard(){
	for(int h = 0; h < HEIGHT; h++){
		for(int w = 0; w < WIDTH; w++){
			board[h*3+w]=' ';
		}
	}
}

void resetSnake(){
	snake[0].active=true;
	snake[0].x = WIDTH/2;
	snake[0].y = HEIGHT/2;
	snake[0].dir = 0;
	for(int i = 1; i < WIDTH*HEIGHT; i++){
		snake[i].active=false;
		snake[i].x = WIDTH/2;
		snake[i].y = HEIGHT/2;
		snake[i].dir = 0;
	}
}