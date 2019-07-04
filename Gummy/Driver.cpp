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
bool watch = true;

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
void playGames(DenseNet* nets, Matrix* choice, Matrix* inputs, int numGames, std::ofstream*);
void setInputs(Matrix*);
	
int main(){
	srand(time(0));
	/*init();
	while(!lost){
		std::cin>>dir;
		update();
		printBoard();
	}*/
	bool load = true;
	std::cout<<"load or save? 1, 0: "<<std::endl;
	std::cin>>load; 
    Gummy gummy = Gummy();
    std::cout<<"Gummy init done";
	int layer0 = 17;
	int layerSizes[3];
	layerSizes[0]=layer0;
	layerSizes[1]=100;
	layerSizes[2]=4;
	DenseNet* nets;// = new DenseNet();
	if(load)
		nets=gummy.loadNet("snakeNet.csv");
	else if(!load)
		nets=gummy.manualInit("gameData.csv", "snakeNet.csv", 1, 3, layerSizes, true);
	std::cin.get();
	std::ofstream* of = new std::ofstream;
	std::cout<<"updating training data"<<std::endl;
	gummy.setCsvFileName("gameData.csv");
	gummy.updateTrainingData(true);
	std::cout<<"done Updating"<<std::endl;
	std::cin.get();
	Matrix* choice = new Matrix(4,1);
	Matrix inputs = Matrix(layer0,1);


	//gummy.train(nets);


	of->open("gameData.csv");
	of->close();
	int ga = 0;
	if(watch) ga = 3;
	else ga = 500000;
	playGames(nets, choice, &inputs, ga, of);
	gummy.updateTrainingData(true);
	gummy.train(nets);
	char save = 'n';
	std::cout<<"save? ";
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
	char space = ' ';
	for(int i = 0; i < 3; i++){
		switch(i){
			case 0:
				space = ' ';
			break;
			case 1:
				space = 'o';
			break;
			case 2:
				space = '@';
			break;
		}
	    inputs->set(i*4,0,board[((snake[0].y-1)%HEIGHT)*WIDTH+snake[0].x]==space?1:0);
		inputs->set(i*4+1,0,board[snake[0].y*WIDTH+(snake[0].x+1)%WIDTH]==space?1:0);
		inputs->set(i*4+2,0,board[((snake[0].y+1)%HEIGHT)*WIDTH+snake[0].x]==space?1:0);
		inputs->set(i*4+3,0,board[snake[0].y*WIDTH+(snake[0].x-1)%WIDTH]==space?1:0);
	}
	inputs->set(12,0,(fruitX-snake[0].x)*1.0/WIDTH);
	inputs->set(13,0,(fruitY-snake[0].y)*1.0/HEIGHT);
	inputs->set(14,0,std::pow(1.0*(fruitX-snake[0].x)*(fruitX-snake[0].x)+1.0*(fruitY-snake[0].y)*(fruitY-snake[0].y),0.5));
	inputs->set(15,0,snake[0].x);
	inputs->set(16,0,snake[0].y);
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

void playGames(DenseNet* nets, Matrix* choice, Matrix* inputs, int numGames, std::ofstream* of){
	of->open ("gameData.csv", std::fstream::in | std::fstream::out | std::fstream::app);
	(*of)<<"0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";
	int numTurns = 30;
	double dataToPrint[21*numTurns];
	bool takenTurn = false;
	for(int ga = 0; ga < numGames; ga++){
		//std::cout<<"-----------------------------GAME START----------------------"<<std::endl;
		int turnNumber = 0;
		init();
		lost = false;
		int oldLength = snakeLength;
		while(!lost){
			if(takenTurn){
				for(int i = 0; i < inputs->getM();i++){
					dataToPrint[21*turnNumber+i]=inputs->get(i,0);
				}
				for(int i = 0; i < choice->getM();i++){
					if(i == dir)
						dataToPrint[21*turnNumber+i+inputs->getM()]=1;
					else
						dataToPrint[21*turnNumber+i+inputs->getM()]=0;
				}
				
			}
			setInputs(inputs);
			if(watch)std::cin.get();
			/*
			char cdir = ' ';
			std::cin>>cdir;
			if(cdir == 'w') dir = 0;
			else if(cdir=='d') dir = 1;
			else if(cdir=='s') dir = 2;
			else if(cdir=='a') dir = 3;
			*/
			
			if(watch)makeMove(choice, inputs, nets);
			else dir = rand()%4;
			
			update();
			if(snakeLength > oldLength){
				for(int i = 0; i < turnNumber; i++){
					(*of)<<std::endl;
					for(int j = 0; j < 21; j++){
						if(j>0) (*of)<<',';
						(*of)<<dataToPrint[i*21+j];
					}
				}
				oldLength = snakeLength;
				turnNumber = 0;
			}
			
			takenTurn = true;
			if(watch)printBoard();
			turnNumber++;
			if(turnNumber ==numTurns){
				lost = true;
			}
		}
		//std::cout<<"-----------------------------GAME END----------------------"<<std::endl;
	
	}
	of->close();
	//fs.open ("test.txt", std::fstream::in | std::fstream::out | std::fstream::app);
	//std::ofstream out("Info.txt", std::ios_base::app);
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
	snakeLength = 3;
	snake[0].x = rand()%WIDTH;
	snake[0].y = rand()%HEIGHT;
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
		//std::cout<<fruitX<<", "<<fruitY<<std::endl;
	}while(board[fruitY*WIDTH+fruitX]!=' ');
	board[fruitY*WIDTH+fruitX]='@';
}

void update(){
	//std::cout<<"updating Snake"<<std::endl;
	updateSnake();
		//std::cout<<"updating Board"<<std::endl;
	updateBoard();
		//std::cout<<"updating GameState"<<std::endl;
	updateGameState();
		//std::cout<<"Done updating"<<std::endl;
	
}

void updateSnake(){
	for(int i = snakeLength-1; i > 0; i--){
		snake[i].x = snake[i-1].x;
		snake[i].y = snake[i-1].y;
	}
	switch(dir){
		case 0:
			snake[0].y--;
			if(snake[0].y<0){
				lost = true;
				snake[0].y = 0;//HEIGHT-1;
			}
		break;
		case 1:
			snake[0].x++;
			if(snake[0].x>=WIDTH){
				lost = true;
				snake[0].x = WIDTH-1;//0;
			}
		break;
		case 2:
			snake[0].y++;
			if(snake[0].y>=HEIGHT){
				lost = true;
				snake[0].y = HEIGHT-1;//0;
			}
		break;
		case 3:
			snake[0].x--;
			if(snake[0].x<0){
				lost = true;
				snake[0].x = 0;//WIDTH-1;
			}
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