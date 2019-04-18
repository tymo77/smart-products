#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>        
#include <errno.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>
#include "wiringPi.h"
#include "wiringSerial.h"
#include <chrono>
#include <thread>
#include <vector>

#include "../../include/Dobot.h"
#include "../../include/Robot.h"
#include "../../include/LidarLite.h"
#include "../../include/CameraCom.h"

#define RED 	1
#define GREEN  2

#define BLOB 	1
#define CIRC	2

#define POS_START 1
#define POS_1		2
#define POS_2		3

//Set Serial TX&RX Buffer Size
#define SERIAL_TX_BUFFER_SIZE 64
#define SERIAL_RX_BUFFER_SIZE 256

int fd = serialOpen( UART_PATH, BAUD_RATE);// Dobot

sp::Dobot bot;
sp::CameraCom cam(2);//2 Second timeout.



int main(int argc, char* argv[]){
	
	
	for(int i = 0; i < argc; i++){
		printf("Arg %d: %s\n",i,argv[i]);
	}
	
	// Load reference positions from file if flagged. Otherwise, get them
	if(argc < 2){
		std::cout << "Need to get reference positions..." << std::endl;
		bot.getReferences();
		bot.saveReferences();
	}
	else if(std::string(argv[1]) == "-L"){
		std::cout << "Loading reference positions from file..." << std::endl;
		bot.loadReferences();
	}
	else if(std::string(argv[1]) == "--get-pos"){
		bot.printPose();
		return 1;
	}
	else if(std::string(argv[1]) == "--go-to"){
		bot.goTo(stod(argv[2]),stod(argv[3]),stod(argv[4]));
		return 1;
	}
	else if(std::string(argv[1]) == "--suction-off"){
		bot.suctionOff();
		return 1;
	}
	else if(std::string(argv[1]) == "--suction-on"){
		bot.suctionOn();
		return 1;
	}
	
	//~ std::vector<Target> targets = cam.get_targets(min_radius,max_radius,color_index,method_index,pos_index);
	
	std::cout << "Getting red balls..." << std::endl;
	std::vector<sp::Target> targets = cam.get_targets(10,30,RED,CIRC,POS_2);
	//~ std::cout << "Getting red balls..." << std::endl;
	//~ std::vector<sp::Target> targets2 = cam.get_targets(10,30,RED,BLOB,POS_1);
	
	// Go places.
	//~ bot.goHome();
	//~ bot.goRamp1();
	//~ bot.goHome();
	//~ bot.goRamp2();
	
}   







