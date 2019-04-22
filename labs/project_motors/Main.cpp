/**
//====================================================================
//                  	lab4_main.cpp
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
sudo g++ -o test2  main_test2.cpp Message.h Message.cpp  Packet.h Packet.cpp ProtocolDef.h Protocol.h Protocol.cpp ProtocolID.h RingBuffer.h RingBuffer.cpp Robot.h Robot.cpp command.h command.cpp -lwiringPi
========================References===============================
	https://www.dobot.cc/dobot-magician/specification.html
	https://www.dobot.cc/download/dobot-communication-protocol-v1-0-4/
	https://www.dobot.cc/download/dobot-magician-api-v1-1-1/
=================================================================
	@author 		Dylan DeSantis
	@date		2/16/2018
	@version	1.0.0
*/

//#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>
//#include<string.h>        
//#include<errno.h>
//#include<math.h>
//#include<signal.h>
//#include<unistd.h>
//#ifndef WPi
//#define WPi
//#include <wiringPiI2C.h>
//#include <wiringPi.h>
//#include <wiringSerial.h>
//#endif
//#include"ServoHat.h"
//#include "GPIO.h"
//#include "LidarLite.h"
//#include "MotorPlate.h"
//#include "SPI_Slave.h"
//#include "DCMotor.h"
//#include "Camera.h"
//#include "Protocol.h"
//#include "command.h"
//#include "Robot.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#ifndef WPi
#define WPi
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#endif
#include "ServoHat.h"
#include "myGPIO.h"
#include "LidarLite.h"
#include "SPI_Slave.h"
#include "DCMotor.h"
#include "Robot.h"
#include "Camera.h"

//Set Serial TX&RX Buffer Size
#define SERIAL_TX_BUFFER_SIZE 64
#define SERIAL_RX_BUFFER_SIZE 256

//extern int fd;

//int fd=serialOpen( "/dev/ttyUSB0",BAUD_RATE);

void UpToLevel(float, DCMotor&, LidarLite&);
void DownToLevel(float, DCMotor&, LidarLite&);

void openStageGates(ServoHat &SH,float , float);
void closeStageGates(ServoHat &SH,float , float);

void openLeftGate(ServoHat&, float );
void openRightGate(ServoHat&, float );

void closeLeftGate(ServoHat&, float );
void closeRightGate(ServoHat&, float );



int main(int argc,char *argv[] ) {

	// Setup
	
	int spi1_fd = open(SPI_DEV, O_RDWR );
	//int fd = serialOpen( "/dev/ttyUSB0", BAUD_RATE);
	//int fd1 = serialOpen ("/dev/ttyUSB1", 57600);
	//fflush (stdout);

	// ROBOT

	//Robot Dobot;
	//Dobot.setRxInterrupt(100);
	//Dobot.initRAM();
	//ProtocolInit();
	//Dobot.setInitParams();
	
	// CAMERA

	//Camera  camMV(fd1, Dobot);
	//wiringPiSetup (); 
	//// Robot pick and place

	/*Dobot.setRxInterrupt(0);
	usleep(1000000);
	serialClose(fd1);
	serialClose(fd);
	Dobot.~Robot();
	camMV.~Camera();*/
	
	// Motor
	//int spi2_fd = open(SPI_ENCODER, O_RDWR);
	//int spi1_fd = open(SPI_DEV, O_RDWR );
	float sample_time = 0.01;
	std::string fn = "Signal_History.txt";
	DCMotor motor(spi1_fd, fn);
	motor.setupDCMotor(A0, DC_1, CCW_DC, 0, 0, 16, 12);
	motor.setupController(0.2,0.1,0,0.01);
	motor.startDCMotor();
	//motor.closeLogger();
	
	// LIDAR
	//int i2c1_fd = wiringPiI2CSetup(LL_I2C_ADDR);// Setting up shared I2C Bus
	//LidarLite LL(i2c1_fd);
	//LL.connect();
	//LL.reset();
	//usleep(1000);
	sp::LidarLite LL;
	int i2c1_fd = LL.get_fd();
	usleep(1000);

	//Servos
	ServoHat SH(i2c1_fd);//using same fd to share i2c bus
	SH.setupServo();
	usleep(1000);
	
    //printf("\r\n======End of ROBOT Main Routine ======\r\n");		
    printf("\r\n======Entering  Elevator Main Routine ======\r\n");	
    float platform_height = 20; //cm set these
    float level1_height = 30; //cm set these
    float level2_height = 40; //cm set these
    float ang00, ang01, ang10,ang11,ang20,ang21,ang30,ang31;
    ang00 = 90; ang01 = 0; ang10 =  -90; ang11 =  0; ang20 =  90; ang21 =  0; ang30 =  -90; ang31 =  0;
	SH.setAngle(0, 0);
	SH.setAngle(1, 0);
	SH.setAngle(2, 0);
	SH.setAngle(3, 0);
      	//setupSwitch(motor);
      	usleep(4000000);
      	printf("\r\n======Opening Bottom Gates ======\r\n");
      	openStageGates(SH,ang00,ang10);
      	usleep(4000000);
      	closeStageGates(SH,ang01,ang11);
      	UpToLevel( level1_height, motor, LL);
      	openLeftGate(SH,ang20);
      	usleep(4000000);
      	closeLeftGate(SH,ang21);
      	UpToLevel( level2_height, motor, LL);
      	openRightGate(SH,ang30);
      	usleep(4000000);
      	closeRightGate(SH,ang31);
      	DownToLevel( platform_height, motor, LL);
	motor.stopDCMotor();
      	
	usleep(1000000);
	close(i2c1_fd);
	return 0;
}   




void UpToLevel(float height, DCMotor &motor, sp::LidarLite &LL)
{
	motor.reset(motor.pa);
	usleep(10000);
	motor.setupDCMotor(A0, DC_1, CC_DC, 0, 0, 12, 16);
	motor.startDCMotor();
	float dist= (float)LL.measure_dist();
	motor.updateSpeed(250);
	deque<float> moving_avg (10);
	float avg = 0;
	int i = 0;
	for (int k = 0; k<10; k++)
	{
		dist = ((float)LL.measure_dist());
		moving_avg.push_front(dist);
		moving_avg.pop_back();
		usleep(1000);
	}
	float temp=0;
	for(int j = 0; j<10; j++)
	{
		temp += moving_avg[j];
	}
	avg = temp/10.0;
	while(avg<height)
	{
		dist = ((float)LL.measure_dist());
		moving_avg.push_front(dist);
		moving_avg.pop_back();	
		temp = 0;
		for(int j = 0; j<10; j++)
		{
			temp += moving_avg[j];
		}
		avg = temp/10.0;
		//std::cout<<" Distance and Moving average is : (" << dist<< ", "<<avg<<")"<< std::endl;
		usleep(1000);
	}
	motor.updateSpeed(0);
	motor.stopDCMotor();
	return;
}
	 
void DownToLevel(float height, DCMotor &motor, sp::LidarLite &LL)
{
	motor.reset(motor.pa);
	usleep(10000);
	motor.setupDCMotor(A0, DC_1, CCW_DC, 0, 0, 12, 16);
	motor.startDCMotor();
	float dist= (float)LL.measure_dist();
	motor.updateSpeed(120);
	deque<float> moving_avg (10);
	float avg = 0;
	int i = 0;
	for (int k = 0; k<10; k++)
	{
		dist = ((float)LL.measure_dist());
		moving_avg.push_front(dist);
		moving_avg.pop_back();
		usleep(1000);
	}
	float temp=0;
	for(int j = 0; j<10; j++)
	{
		temp += moving_avg[j];
	}
	avg = temp/10.0;
	while(avg>height)
	{
		dist = ((float)LL.measure_dist());
		moving_avg.push_front(dist);
		moving_avg.pop_back();	
		temp = 0;
		for(int j = 0; j<10; j++)
		{
			temp += moving_avg[j];
		}
		avg = temp/10.0;
		//std::cout<<" Distance and Moving average is : (" << dist<< ", "<<avg<<")"<< std::endl;
		usleep(1000);
	}
	motor.updateSpeed(0);
	motor.stopDCMotor();
	return;
}


void openStageGates(ServoHat &SH,float angle1, float angle2)
{
	SH.setAngle(0, angle1);
	SH.setAngle(1, angle2);
	return;
}
void closeStageGates(ServoHat &SH,float angle1, float angle2)
{
	SH.setAngle(0, angle1);
	SH.setAngle(1, angle2);
	return;
}

void openLeftGate(ServoHat &SH,float angle1)
{
	SH.setAngle(2, angle1);
	return;
}
void openRightGate(ServoHat &SH,float angle1)
{
	SH.setAngle(3, angle1);
	return;
}
void closeLeftGate(ServoHat &SH,float angle1)
{
	SH.setAngle(2, angle1);
	return;
}
void closeRightGate(ServoHat &SH,float angle1)
{
	
	SH.setAngle(3, angle1);
	return;
}

