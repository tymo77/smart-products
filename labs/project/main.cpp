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
#include "../../include/command.h"

#define RED 	1
#define GREEN  2

#define BLOB 	1
#define CIRC	2

#define POS_START 1
#define POS_1		2
#define POS_2		3
#define R_ball 	21.0185

//Set Serial TX&RX Buffer Size
#define SERIAL_TX_BUFFER_SIZE 64
#define SERIAL_RX_BUFFER_SIZE 256

int fd = serialOpen( UART_PATH, BAUD_RATE);// Dobot

sp::Dobot bot;
sp::CameraCom cam(2,-8.8 - R_ball);//2 Second timeout.



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
	
	
	
	// Go places.
	bot.goHome();
	//~ bot.goRamp1();
	//~ bot.goHome();
	//~ bot.goRamp2();
	//~ bot.goHome();
	std::cout << "Getting red balls..." << std::endl;
	std::vector<sp::Target> targets;
	std::vector<double> target_pos;
	for(int i = 0; i < 5; i++){
		targets = cam.get_targets(10,30,RED,BLOB,POS_START);
		if(targets.size() > 0){
			target_pos = cam.cam2world(targets[0],bot.getPose());
			std::cout << "(" << targets[0].x << "," << targets[0].y << ")" << "->" << "(" << target_pos[0] << "," << target_pos[1] << "," << target_pos[2] << ")" << std::endl;
			
			std::cout << "Moving to target..." << std::endl;
			bot.goTo(target_pos[0],target_pos[1],target_pos[2] + 40 + R_ball);//Position 2
		}
		else {
		std::cout << "None found" << std::endl;
		}
	}
	
	bot.goTo(target_pos[0] - 8.0,target_pos[1],target_pos[2] + R_ball - 14.0);//Pick
	bot.suctionOn();
	bot.goHome();
	bot.goRamp1();
	bot.suctionOff();
	bot.goHome();
}
