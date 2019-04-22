#ifndef MOTOR_H
#define	MOTOR_H
/*********************************************************************************************************
**********************************************************************************************************
							   DCMotor.h
**********************************************************************************************************
**********************************************************************************************************
	_filename: DCMotor.h
	_description: This class provides the C++ functionality to communicate with the Pi-Plate Motor
			      Control Board. It uses SPI to communicate to the board and can control both 
			      stepper and DC motors.
				
***************************************References******************************************************
http://pi-plates.com/motorplate-users-guide/
https://pi-plates.com/wp-content/uploads/2015/04/TB6612FNG.pdf
************************************************************************************************************
	@author		Dylan DeSantis
	@date 		3/25/2018
	@version	1.0.0
************************************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include<sstream>
#include<fstream>
#include<cstring>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <chrono>
#include <ctime>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <deque>
#include "SPI_Slave.h"
#include"GPIO.h"
#include"MotorPlate.h"

//	Motor Params
#define PPR						1120.0
#define MAX_DPS				960.0

//	Constants
#define DEG2REV				0.01745329
#define REV2DEG				57.2957795
#define RPM2DPS				0.00029088209
#define DPS2RPM				3437.74677
				
using namespace std;
using namespace std::chrono;
class DCMotor: public MotorPlate
{
public:

	/****************************************************************************************************
	 								Members 
	 ***************************************************************************************************/
	int fd_spi;
	DC_MOTOR mtr;
	PLATE_ADDR pa;
	DC_MOTOR_DIR dir;
	int pin_enA;
	int pin_enB;
	float speed_k = 0;
	std::chrono::steady_clock::time_point  start_time;
	std::chrono::steady_clock::time_point  end_time;
	float timeout = 0.1;
	string file_name;
	FILE *pFile;
	
	//	Digital Control Members
	float Ts; // in seconds
	float K1, K2, K3;
	float error_sig_km0 = 0.0; // error for the k-0 sample
	float error_sig_km1 = 0.0;// error for the k-1 sample
	float error_sig_km2 = 0.0;// error for the k-2 sample
	float ctrl_sig_km0 = 0.0;// control for the k-0 sample
	float ctrl_sig_km1 = 0.0;// control for the k-1 sample
	
	
	/****************************************************************************************************
	 								Methods 
	 ***************************************************************************************************/
public:
	int setupDCMotor(PLATE_ADDR addr, DC_MOTOR mtr_in, DC_MOTOR_DIR mtr_dir, float start_speed, float accel, int A_Encoder_Pin, int B_Encoder_Pin);
	int setupController(float Kp, float Ki, float Kd, float Ts);
	int closeLogger();
	float time();
	int sampleHold(int delay, timeScale ts);
	int startDCMotor();
	int stopDCMotor();
	int updateSpeed(float);
	float readSpeed();
	float saturation(float speed);
	int logSignals(float time, float ref, float ctrl, float err, float actual);
	int update_error_hist(float err);
	int update_control_hist(float ctrl);
	int controlSpeed();   
	float reference(float time);
	
//private:
	/****************************************************************************************************
	 								Constructors/Destructors 
	 ***************************************************************************************************/

	DCMotor(int fd, string fn):
		fd_spi(fd),
		file_name(fn),
		MotorPlate(fd)
		{}
	~DCMotor() {}

};
#endif
