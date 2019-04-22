#ifndef SERVO_HAT_H
#define SERVO_HAT_H
/*********************************************************************************************************
**********************************************************************************************************
							   ServoHat.h
**********************************************************************************************************
**********************************************************************************************************
	_filename: ServoHat.h
	_description: This class controls the communication protocols with the Adafruit PCA9685 
			PWM / Servo motor raspberry pi 3B hat. This class uses I2C to communicate
			 with the RPi and  includes the basic functionality to actuate up to 16 servo motors.


***************************************Info About HS-425BB**********************************************
Pulse Cycle: 20 ms 
Neutral (90 deg ): 1.5 ms
Minimum( 0 deg): 1
Maximum( 180 deg): 2
Degree per microsecond : 0.096


***************************************References******************************************************

************************************************************************************************************
	@author		Dylan DeSantis
	@date 		4/19/2018
	@version	1.0.0
************************************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
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
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <deque>
#ifndef WPi
#define WPi
#include <wiringPiI2C.h>
#endif
#include <iostream>

// Servo Hat constants
#define SERVO_ADDR				0x40
#define SERVO_MODE1				0x00
#define SERVO_MODE2				0x01
#define SERVO_SUBADR1				0x02
#define SERVO_SUBADR2				0x03
#define SERVO_SUBADR3				0x04
#define SERVO_PRESCALE			0xFE
#define SERVO_LED0_ON_L			0x06
#define SERVO_LED0_ON_H			0x07
#define SERVO_LED0_OFF_L			0x08
#define SERVO_LED0_OFF_H			0x09
#define SERVO_ALL_LED_ON_L			0xFA
#define SERVO_ALL_LED_ON_H		0xFB
#define SERVO_ALL_LED_OFF_L		0xFC
#define SERVO_ALL_LED_OFF_H		0xFD

#define SERVO_RESTART				0x80
#define SERVO_SLEEP				0x10
#define SERVO_ALLCALL				0x01
#define SERVO_INVRT				0x10
#define SERVO_OUTDRV				0x04

// HS-425BB Servo 
#define HS425_CYC_MS				20.0	// ms
#define HS425_NEUTRAL				1.50	// ms
#define HS425_MAX					2.0	// ms			
#define HS425_MIN					1.0	// ms

/*
#define HS425_NEUTRAL				1.50	// ms
#define HS425_MAX					2.520	// ms			
#define HS425_MIN					0.553	// ms
*/



class ServoHat
{
public:

	/****************************************************************************************************
	 								Memebers 
	 ***************************************************************************************************/
	uint8_t	servo_i2c_addr = SERVO_ADDR;
	int		fd_i2c;
	float	max_pwidth_ms;
	float	min_pwidth_ms;
	float	neutral_pwidth_ms;
	float	cycle_time_ms;
	
	
public:
	/****************************************************************************************************
	 								Methods 
	 ***************************************************************************************************/
	int read8(int reg);
	int write8(int reg, int data);
	int takeI2CBus();

	int setupServo(float, float, float,float);
	int setupServo();
	int setPWMFreq(float);
	int setPWMFreq();
	int setPWM(int, int, int);
	int setAllPWM(int, int);
	int setAngle(int, float);
	int reset();


	/****************************************************************************************************
	 								Constructors/Destructors 
	 ***************************************************************************************************/

	ServoHat(int fd):
		fd_i2c(fd){}
	~ServoHat() {}

};

#endif
