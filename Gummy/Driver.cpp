#include "Gummy.h"
#include<string>
//#define printGames
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

void update();
void init();
void clearBoard();
void printBoard();
void resetSnake();
void spawnFruit();
void updateSnake();
void updateBoard();
void updateGameState();
	
int main(){
	srand(time(0));
	init();
	while(!lost){
		std::cin>>dir;
		//dir = rand()%3;
		update();
		printBoard();
	}
	bool load = true;
	std::cout<<"load or save? 1, 0: "<<std::endl;
	std::cin>>load; 
    Gummy gummy = Gummy();
    std::cout<<"Gummy init done";
	int layerSizes[4];
	layerSizes[0]=27;
	layerSizes[1]=243;
	layerSizes[2]=9;
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
	for(int w = 0; w < WIDTH+2; w++){
			std::cout<<'-';
	}
	std::cout<<std::endl;
	for(int h = 0; h < HEIGHT; h++){
		std::cout<<'|';
		for(int w = 0; w < WIDTH; w++){
			std::cout<<board[h*WIDTH+w];
		}
		std::cout<<'|'<<std::endl;
	}
	for(int w = 0; w < WIDTH+2; w++){
			std::cout<<'-';
	}
}

void init(){
	clearBoard();
	resetSnake();
	spawnFruit();
}

void clearBoard(){
	for(int h = 0; h < HEIGHT; h++){
		for(int w = 0; w < WIDTH; w++){
			board[h*WIDTH+w]=' ';
		}
	}
}

void resetSnake(){
	snake[0].x = WIDTH/2;
	snake[0].y = HEIGHT/2;
	dir = 0;
	for(int i = 1; i < WIDTH*HEIGHT; i++){
		snake[i].x = WIDTH/2;
		snake[i].y = HEIGHT/2;
		//dir = 0;
	}
}

void spawnFruit(){
	do{
		fruitX = rand()%WIDTH;
		fruitY = rand()%HEIGHT;
		std::cout<<fruitX<<", "<<fruitY<<std::endl;
	}while(board[fruitY*WIDTH+fruitX]!=' ');
	board[fruitY*WIDTH+fruitX]='@';
}

void update(){
	std::cout<<"updating Snake"<<std::endl;
	updateSnake();
	std::cout<<"updating Board"<<std::endl;
	updateBoard();
	std::cout<<"updating GameState"<<std::endl;
	updateGameState();
	std::cout<<"Done updating"<<std::endl;
}

void updateSnake(){
	for(int i = snakeLength-1; i > 0; i--){
		snake[i].x = snake[i-1].x;
		snake[i].y = snake[i-1].y;
	}
	switch(dir){
		case 0:
			snake[0].y--;
			if(snake[0].y<0)
				snake[0].y = HEIGHT-1;
		break;
		case 1:
			snake[0].x++;
			if(snake[0].x>=WIDTH)
				snake[0].x = 0;
		break;
		case 2:
			snake[0].y++;
			if(snake[0].y>=HEIGHT)
				snake[0].y = 0;
		break;
		case 3:
			snake[0].x--;
			if(snake[0].x<0)
				snake[0].x = WIDTH-1;
		break;
	}
}

void updateBoard(){
	clearBoard();
	board[fruitY*WIDTH + fruitX] = '@';
	board[snake[0].y*WIDTH + snake[0].x]='O';
	for(int i = 1; i<snakeLength;i++){
		board[snake[i].y * WIDTH + snake[i].x] = 'o';
	}
}

void updateGameState(){
	if(board[snake[0].y*WIDTH+snake[0].x]!='O'){
		lost = true;
	}
	else if(board[fruitY*WIDTH+fruitX]!='@'){
		snakeLength++;
		spawnFruit();
	}
}