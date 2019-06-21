#include "Gummy.h"

int main(){
    Gummy gummy = Gummy();
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
	DenseNet* copy = gummy.loadNet(userNet->getName());
	copy->setName("copy.csv");
	gummy.saveNet(copy);
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout<<"done and authored by Timothy-Flavin"<<std::endl;
	std::cin.get();
    return 0;
}