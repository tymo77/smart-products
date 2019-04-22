/*********************************************************************************************************
**********************************************************************************************************
							   ServoHat.cpp
**********************************************************************************************************
**********************************************************************************************************
	@author		Dylan DeSantis
	@date 		2/26/2018
	@version	1.0.0
**********************************************************************************************************/


#include "../include/ServoHat.h"

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
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <wiringPiI2C.h>
#include <math.h>

/****************************************************************************************************
	Function Name:  read8
	Inputs: 
		1. int reg = register to read
	Outputs:
		2. int result = the byte that is returned
	Description:
		reads a byte from a register after taking control of the i2c bus
 ***************************************************************************************************/

int ServoHat::read8(int reg)
{
	this->takeI2CBus();
	int result =wiringPiI2CReadReg8(this->fd_i2c, reg);
	//usleep(1000);
	return result;
}

/****************************************************************************************************
	Function Name:  write8
	Inputs: 
		1. int reg = register to write to.
		2. int data = the byte to write 
	Outputs:

	Description:
		This writes a byte to the desired register after taking control of the i2c bus
 ***************************************************************************************************/
int ServoHat::write8(int reg, int data)
{
	this->takeI2CBus();
	//wiringPiI2CWrite(this->fd_i2c, reg);
	//wiringPiI2CWrite(this->fd_i2c, data);
	wiringPiI2CWriteReg8(this->fd_i2c, reg, data);
	//usleep(1000);
	return 0;
}


/****************************************************************************************************
	Function Name:  configure
	Inputs: 
		1. int config = the configuration to set the type of performance we want
	Outputs:

	Description:
		This function set the configuration of the Lidar's measurement system to output
		a given accuracy and time response.
 ***************************************************************************************************/
int ServoHat::setupServo()
{

	this->takeI2CBus();
	this->setAllPWM(0,0);
	this->write8(SERVO_MODE2, SERVO_OUTDRV);
	this->write8(SERVO_MODE1, SERVO_ALLCALL);
	usleep(5000);
	unsigned int mode1 = this->read8(SERVO_MODE1);
	mode1 = (mode1 & (~SERVO_SLEEP));
	this->write8(SERVO_MODE1, mode1);
	usleep(5000);
	this->max_pwidth_ms = HS425_MAX;
	this->min_pwidth_ms = HS425_MIN;
	this->neutral_pwidth_ms = HS425_NEUTRAL;
	this->cycle_time_ms = HS425_CYC_MS;
	this->setPWMFreq();
	return 0;
}


int ServoHat::setupServo(float max_pulse, float min_pulse, float neutral_pulse, float cycle_time)
{

	this->takeI2CBus();
	this->setAllPWM(0,0);
	this->write8(SERVO_MODE2, SERVO_OUTDRV);
	this->write8(SERVO_MODE1, SERVO_ALLCALL);
	usleep(5000);
	unsigned int mode1 = (unsigned int)(this->read8(SERVO_MODE1));
	mode1 = (mode1 & (~SERVO_SLEEP));
	this->write8(SERVO_MODE1, mode1);
	usleep(5000);
	this->max_pwidth_ms = max_pulse;
	this->min_pwidth_ms = min_pulse;
	this->neutral_pwidth_ms = neutral_pulse;
	this->cycle_time_ms = cycle_time;
	return 0;
}

/****************************************************************************************************
	Function Name:  distance
	Inputs: 
		1. bool bias_correct = whether or not to return a bias correct measurement
	Outputs:
		1. int dist = the distance in centimeters of the lidar measurement
	Description:
		This function reads the distance measurements from the lidar lite  module and 
		checks to make sure that the hardware is in working condition. It waits until 
		the lidar has finished with its current measurements before grabbing the data.
 ***************************************************************************************************/
 
 
int ServoHat::setPWMFreq(float freq_hz)
{
	float prescale;
	int byte_prescale;
	prescale = (25000000.0/(4096.0*freq_hz))-1.0;
	byte_prescale = (int)((floor((prescale+0.5))));
	std::cout<<"Prescale is : "<<byte_prescale<<std::endl;
	this->takeI2CBus();
	unsigned int oldmode = (unsigned int)(this->read8(SERVO_MODE1));
	unsigned int newmode = (oldmode & 0x07F) | 0x10;
	this->write8(SERVO_MODE1, newmode);
	this->write8(SERVO_PRESCALE, byte_prescale);
	this->write8(SERVO_MODE1, oldmode);
	usleep(5000);
	this->write8(SERVO_MODE1, (oldmode |0x80));
	return 0;
}

int ServoHat::setPWMFreq()
{
	float freq_hz = 1000.0/((this->cycle_time_ms));
	float prescale;
	int byte_prescale;
	prescale = (25000000.0/(4096.0*freq_hz))-1.0;
	byte_prescale = (int)((floor((prescale+0.5))));
	std::cout<<"Prescale is : "<<byte_prescale<<std::endl;
	this->takeI2CBus();
	unsigned int oldmode = (unsigned int)(this->read8(SERVO_MODE1));
	unsigned int newmode = (oldmode & 0x07F) | 0x10;
	this->write8(SERVO_MODE1, newmode);
	this->write8(SERVO_PRESCALE, byte_prescale);
	this->write8(SERVO_MODE1, oldmode);
	usleep(5000);
	this->write8(SERVO_MODE1, (oldmode |0x80));
	return 0;
}

int ServoHat::setPWM(int channel, int start_tick, int end_tick)
{
	this->write8(SERVO_LED0_ON_L+4*channel, start_tick & 0xFF);
	this->write8(SERVO_LED0_ON_H+4*channel, start_tick >> 8);
	this->write8(SERVO_LED0_OFF_L+4*channel, end_tick & 0xFF);
	this->write8(SERVO_LED0_OFF_H+4*channel, end_tick >> 8);
	return 0;
}


int ServoHat::setAllPWM(int start_tick, int end_tick)
{
	this->write8(SERVO_ALL_LED_ON_L, start_tick & 0xFF);
	this->write8(SERVO_ALL_LED_ON_H, start_tick >> 8);
	this->write8(SERVO_ALL_LED_OFF_L, end_tick & 0xFF);
	this->write8(SERVO_ALL_LED_OFF_H, end_tick >> 8);
	return 0;	
	
}

int ServoHat::setAngle(int servo_num, float theta_deg)
{
	// Theta is degrees from center with positive to the right and negative to the left
	//  (i.e. max theta = 90.......... min theta = -90
	float range = this->max_pwidth_ms - this->min_pwidth_ms;
	float ms_per_deg = range/180.0;
	float pulse_width = neutral_pwidth_ms+(theta_deg)*ms_per_deg;
	float ticks = (pulse_width/this->cycle_time_ms)*4096.0;
	int end_tick = (int)(floor(ticks));
	std::cout<<"end tick is : "<<end_tick<<std::endl;
	this->setPWM(servo_num, 0, end_tick);
	return 0;	
}


int ServoHat::reset()
{
	
	return 0;
}



/****************************************************************************************************
	Function Name:  takeI2CBus
	Inputs: 
		
	Outputs:
	
	Description:
		 This function takes control of the I2C bus
 ***************************************************************************************************/
int ServoHat::takeI2CBus()
{
	/*
	Sets the I2C to communicate with this device
	*/
	if (ioctl(this->fd_i2c, I2C_SLAVE, this->servo_i2c_addr) < 0)
	{
		printf("problem setting slave\n");
		return -1;
	}
	return 0;
}
