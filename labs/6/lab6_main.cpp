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
#include<string.h>        
#include<errno.h>
#include<math.h>
#include<signal.h>
#include<unistd.h>
#include "wiringPi.h"
#include "wiringSerial.h"

#include "Dobot.h"
//Set Serial TX&RX Buffer Size
#define SERIAL_TX_BUFFER_SIZE 64
#define SERIAL_RX_BUFFER_SIZE 256
extern int fd;

int fd=serialOpen( UART_PATH, BAUD_RATE);


int main() 
{
	//	Setup
	int fd = serialOpen( UART_PATH, BAUD_RATE);
	Dobot bot;
	bot.setup();
	bot.mRoutine = RobotRoutine;// defined in my header function
        printf("\r\n======Enter Main Routine ======\r\n");
        /**
          Here you will program the dobot to pick up the
          grid of table tennis balls and place them onto the
          grid. You may use the provided functions in
          the Robot class. The pick and place operation should
          navigate to the location of the ball, drop down onto it
          to acquire a secure seal and then turn on the pump.
          The robot should wait for a moment to let the pump
          to obtain a vacuum. Then you you should navigate to a
          position over the rails and turn off the pump. Repeat this
          until all the balls are placed into the box.
         **/
    	//
	bot.mRoutine(bot);
	
}   






