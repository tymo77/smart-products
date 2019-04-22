#ifndef MOTORPLATE_H
#define	MOTORPLATE_H
/*********************************************************************************************************
**********************************************************************************************************
							   MotorPlate.h
**********************************************************************************************************
**********************************************************************************************************
	_filename: MotorPlate.h
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
#include "mySPGPIO.h"
#include "SPGPIO.h"

//	Board Absolutes
#define MAX_PLATE_ADDR		8
#define MAX_RPM				2000
#define MAX_DC_MOTORS		4
#define MAX_STEP_MOTORS		2
#define MAX_STEP_RES			3

//	Board Configurations
#define MOTOR_BASE_ADDR		16
#define PIN_FRAME				25		//	 OUTPUT
#define PIN_INT				22		//	INPUT

//	Communication Settings
#define SPI_DEV				"/dev/spidev0.1"	//	SPI0_CS1
#define STEP_RATE_MASK		0x00FF
#define STEP_STEP_MASK		0xFF
#define STEP_RES_SHIFT		4		//	shift resolution left (res <<4)
#define STEP_RATE_SHIFT		8		//	shift rate right (rate>>8)
#define RESET					0x0F

#define STEP_CONFIG_CMD		0x10
#define STEP_MOVE_CMD		0x12
#define STEP_JOG_CMD			0x14
#define STEP_STOP_CMD		0x16
#define STEP_RATE_CMD		0x18
#define STEP_OFF_CMD			0x1E

#define DC_CONFIG1_CMD		0x30
#define DC_CONFIG2_CMD		0x3A
#define DC_SPEED_CMD			0x33
#define DC_START_CMD			0x31
#define DC_STOP_CMD			0x32

#define GET_SENSOR_CMD		0x20
#define GET_TACHC_CMD		0x22
#define GET_TACHF_CMD		0x23

typedef enum{
	A0 = 0,
	A1 = 1,
	A2 = 2,
	A3 = 3,
	A4 = 4,
	A5 = 5,
	A6 = 6,
	A7 = 7
} PLATE_ADDR;

typedef enum{
	STEP_A = 0,
	STEP_B = 1
} STEP_MOTOR;

typedef enum{
	DC_1 = 0,
	DC_2 = 1,
	DC_3 = 2,
	DC_4 = 3
} DC_MOTOR;

typedef enum{
	TACH_1 = 0,
	TACH_2 = 1,
	TACH_3 = 2,
	TACH_4 = 3
} TACH_NUM;

typedef enum{
	CCW_STEP =	0x00,
	CC_STEP = 	0x40
} STEP_MOTOR_DIR;

typedef enum{
	CCW_DC = 0x00,
	CC_DC  =  0x10
} DC_MOTOR_DIR;

typedef enum{
	RES_0 = 0,
	RES_1 = 1,
	RES_2 = 2,
	RES_3 = 3
} STEP_RES;

class MotorPlate: public SPI_Slave, public sp::MyGPIO
{
public:

	/****************************************************************************************************
	 								Members 
	 ***************************************************************************************************/
	int fd_spi;
	
	/****************************************************************************************************
	 								Methods 
	 ***************************************************************************************************/
public:
	int setup();
	int reset(PLATE_ADDR addr);
	int sendCMD(PLATE_ADDR , uint8_t , uint8_t , uint8_t , int , int* );
	int delay( int delay, timeScale ts);
	
	int configStepper(PLATE_ADDR addr, STEP_MOTOR mtr, STEP_MOTOR_DIR mtr_dir, STEP_RES mtr_res, float step_rate, float step_accel);
	int moveStepper(PLATE_ADDR addr, STEP_MOTOR mtr, int steps);
	int jogStepper(PLATE_ADDR addr, STEP_MOTOR mtr);
	int stopStepper(PLATE_ADDR addr, STEP_MOTOR mtr);
	int putStepRate(PLATE_ADDR addr, STEP_MOTOR mtr, float rate);
	int turnStepperOff(PLATE_ADDR addr, STEP_MOTOR mtr);
	
	int configDC(PLATE_ADDR addr, DC_MOTOR mtr, DC_MOTOR_DIR mtr_dir, float speed, float accel);
	int putDCSpeed(PLATE_ADDR addr, DC_MOTOR mtr, float speed);
	int startDC(PLATE_ADDR addr, DC_MOTOR mtr);
	int stopDC(PLATE_ADDR addr, DC_MOTOR mtr);
	
	int getSensors(PLATE_ADDR addr);
	int getTachCoarse(PLATE_ADDR addr, TACH_NUM tnum);
	int getTachFine(PLATE_ADDR addr, TACH_NUM tnum);
//private:
	/****************************************************************************************************
	 								Constructors/Destructors 
	 ***************************************************************************************************/

	MotorPlate(int fd):
		fd_spi(fd),
		SPI_Slave(fd, -1, 0,  500000, 8),
		MyGPIO()
		{}
	~MotorPlate() {}

};

#endif
