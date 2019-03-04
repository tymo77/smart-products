/**
//====================================================================
//                  	lab6_main.cpp
//====================================================================

	_description: This is the header file to control the robotic arm, Dobot Magician, 
		using the raspberry pi 3. We connect the RPi to the Dobot using the usb cable
		that comes with the Dobot. Utilizing the communication protocol of developed
		by the Dobot company, we will package the different commands and setup
		procedure into a more compact and usable class called Robot. Make sure 
		have installed the wiringPi library. To connect 
		with the Dobot, all you need to do is 
			1. turn off the Dobot
			2. Plug in the USB 3 cable to the Pi and the Dobot.
			3. Turn on the Dobot. 
			4. Wait about 30 seconds perform trying to run any programs
			5. Begin your program.

===============To compile enter the following command ================
sudo g++ -o lab6 lab6_main.cpp Message.h Message.cpp  
Packet.h Packet.cpp ProtocolDef.h Protocol.h Protocol.cpp ProtocolID.h 
RingBuffer.h RingBuffer.cpp Robot.h Robot.cpp Dobot.h
command.h command.cpp -lwiringPi
========================References===============================
	https://www.dobot.cc/dobot-magician/specification.html
	https://www.dobot.cc/download/dobot-communication-protocol-v1-0-4/
	https://www.dobot.cc/download/dobot-magician-api-v1-1-1/
=================================================================
*/

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

//Set Serial TX&RX Buffer Size
#define SERIAL_TX_BUFFER_SIZE 64
#define SERIAL_RX_BUFFER_SIZE 256
extern int fd;

vector<vector<float>> pPICKUP(5,vector<float>(10,0)); // Five points to pickup balls
vector<float> pRAMP = {187, -210, 125}; // One point to drop them.

int fd=serialOpen( UART_PATH, BAUD_RATE);

void gotoppickupfield(Dobot &bot){
	bot.goToXYZ(pPICKUP[0][0],pPICKUP[0][1],pRAMP[2],true);
}

void pickup(Dobot &bot, vector<float> p){
	bot.goToXYZ(p[0], p[1], 20, true);
	bot.goToXYZ(p[0], p[1], p[2]-6, true);
	bot.setPump(true,true);
	bot.wait(500);
	bot.goToXYZ(p[0], p[1], 20, true);
}

void drop(Dobot &bot,vector<float> p){
	bot.goToXYZ(p[0], p[1], 20, true);
	bot.goToXYZ(p[0], p[1], p[2]+2, true);
	bot.setPump(false,true);
	bot.wait(500);
	bot.goToXYZ(p[0], p[1], 20, true);
}

void dropRamp(Dobot &bot){
	gotoppickupfield(bot);
	bot.goToXYZ(pRAMP[0], pRAMP[1], pRAMP[2], true);
	bot.setPump(false,true);
	gotoppickupfield(bot);
}

void recordPose(vector<float> & p, int i, Dobot &bot){
	Pose pose = bot.getPose();
	p[0] = pose.x;
	p[1] = pose.y;
	p[2] = pose.z;
}

void printPose(Dobot &bot){
	Pose pose = bot.getPose();
	printf("at position x =  % 7.2f y = % 7.2f z = % 7.2f r = % 7.2f\n",pose.x,pose.y,pose.z,pose.r);
}


void myRobotRoutine(Dobot &bot) {
	cout << "Initial state\n";
	printPose(bot);
	cout << "Begin recording pickup points...\n";
	
	for(int i = 0; i < 5; i++){
		cin.get();
		recordPose(pPICKUP[i],i,bot);
		cout << "Recorded " << i << endl;
	}
	
	cout << "Record drop-off point...\n";
	cin.get();
	recordPose(pRAMP,0,bot);
	
	cout << "Running.." << endl;
	gotoppickupfield(bot);
	for(int i = 0; i < 5; i++){
		cout << "Getting " << i << endl;
		pickup(bot,pPICKUP[i]);
		dropRamp(bot);
	}
};

int main(int argc, char* argv[]){
	int fd = serialOpen( UART_PATH, BAUD_RATE);
	Dobot bot;
	bot.setup();
	bot.mRoutine = myRobotRoutine;
        printf("\nRunning Main Routine....\n");
	bot.mRoutine(bot);
}   







