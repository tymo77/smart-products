#ifndef DOBOT_H
#define DOBOT_H
/**
//====================================================================
//                                  Robot.h
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
	
========================References===============================
	https://www.dobot.cc/dobot-magician/specification.html
	https://www.dobot.cc/download/dobot-communication-protocol-v1-0-4/
	https://www.dobot.cc/download/dobot-magician-api-v1-1-1/
=================================================================
	@author 		Dylan DeSantis
	@date		2/16/2018
	@version	1.0.0
*/

#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<errno.h>
#include<math.h>
#include<signal.h>
#include<unistd.h>
#include "Protocol.h"
#include "command.h"
#ifndef WPi
#define WPi
#include <wiringPi.h>
#include<wiringSerial.h>
#endif

//	Serial Communication (USB)
#define UART_PATH "/dev/ttyUSB0"
#define BAUD_RATE 115200
extern int fd; // file descriptor for USB

using namespace std;


class Robot
{
public:
	//==========================================
        //                  MEMBERS
	//==========================================
	// ==========================================
	//==========================================
	
	//	Dobot Communication Protocols Memebers 
	//==========================================
	EndEffectorParams 		gEndEffectorParams;
	PTPCoordinateParams 	gPTPCoordinateParams;
	PTPCommonParams 		gPTPCommonParams;
	PTPCmd          		gPTPCmd;
	uint64_t 				gQueuedCmdIndex;
	HOMEParams				gHOMEParams;
	HOMECmd					gHOMECmd;
	Pose					gPose;
	Kinematics				gKinematics;
	WAITCmd					gWAITCmd;
	bool					suction;
	//==========================================
	//				METHODS
	//==========================================
	
	
	//	Initialization Methods
	//==========================================
	void initRAM(void);
	void setInitParams();
	void setup();
	
	//	Serial Methods
	//==========================================
	void setRxInterrupt(int usecs);
	static void serialRead(int sig_num);
	uint8_t serialRead();
	static void serialWrite(uint8_t);
	
	//	Robot Commands 
	//==========================================
	void setHome(float x, float y, float z, float q4, bool waitEnd);
	void goToHome(void);	
	void goToXYZ(float x, float y, float z, bool waitEnd);			// Go to position in cart. space
	void goToQ(float q1, float q2, float q3, float q4);	// Go to  position in joint space
	void setEndEffector(float x, float y, float z, float q4, bool waitEnd); // Set end effector
	Pose getPose(); 								// Get end effector config: returns x,y,z,q4
	void setPump( bool on_off, bool waitEnd);					// turn on (0x01) or turn off (0x00) suction
	uint8_t getPump();								// get the current state of suction
	void wait(uint32_t timeout_ms);					// have the robot wait in a position
	
	//	Robot feedback Commands
	//==========================================
	void			setupVision();
	
	//	Robot constructor/ destructor
	//==========================================
	Robot()
	{
		wiringPiSetup();
	}
	~Robot() { }
};
#endif
