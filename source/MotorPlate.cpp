/*********************************************************************************************************
**********************************************************************************************************
							 MotorPlate.cpp
**********************************************************************************************************
**********************************************************************************************************
	@author		Dylan DeSantis
	@date 		3/25/2018
	@version	1.0.0
**********************************************************************************************************/
#include "MotorPlate.h"
#include "SPI_Slave.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include<math.h>
int MotorPlate::setup()
{
	SPI_Slave::spiBegin();
	GPIO::pinMode(PIN_FRAME, GPIO::PinModes::OUTPUT);
	GPIO::pinMode(PIN_INT, GPIO::PinModes::INPUT);
	return 0;
}

int MotorPlate::reset(PLATE_ADDR addr)
{
	int resp[1] = {0};
	MotorPlate::sendCMD(addr, RESET,0,0,0, resp);
	return 0;
}

int MotorPlate::sendCMD(PLATE_ADDR addr, uint8_t cmd, uint8_t param1, uint8_t param2, int bytes2return, int resp[])
{
	unsigned char cmds[4], dummy[bytes2return];
	memset(cmds, 0, sizeof(cmds));
	if (bytes2return>0) {memset(dummy, 0, sizeof(dummy));}
	//memset(resp, 0, sizeof(resp));
	
	
	cmds[0] = (addr | MOTOR_BASE_ADDR);
	cmds[1] = cmd;
	cmds[2] = param1;
	cmds[3] = param2;
	unsigned int tx_lens_1[1] = {4}; 
	unsigned int rx_lens_1[1] = {0};
	
	SPI_Slave::spiPutBlkCmds(rx_lens_1, tx_lens_1, cmds, 4, 1);
	GPIO::digitalWrite(PIN_FRAME, GPIO::DigitalOut::HIGH);
	SPI_Slave::spiTransfer(32, 32, SEQ, 300000, 60);
	SPI_Slave::spiDelay(100, uS);
	SPI_Slave::spiClearQueues();
	
	unsigned int tx_lens_2[1] = {(unsigned int)bytes2return}; 
	unsigned int rx_lens_2[1] = {(unsigned int)bytes2return};	
	SPI_Slave::spiPutBlkCmds(rx_lens_2, tx_lens_2, dummy, bytes2return, 1);
	SPI_Slave::spiDelay(1, mS);
	SPI_Slave::spiTransfer(32, 32, SYNC, 500000, 20);
	SPI_Slave::spiDelay(1, mS);
	for( int i = 0 ; i<SPI_Slave::stream_in.size(); i++)
	{
		resp[i] = (int)SPI_Slave::stream_in[i];
	}
	SPI_Slave::spiClearQueues();
	SPI_Slave::spiDelay(1, mS);
	GPIO::digitalWrite(PIN_FRAME, GPIO::DigitalOut::LOW);
	return 0;
}

int MotorPlate::delay(int delay, timeScale ts)
{
	SPI_Slave::spiDelay(delay,ts);
	return 0;
}

int MotorPlate::configStepper(PLATE_ADDR addr, STEP_MOTOR mtr, STEP_MOTOR_DIR mtr_dir, STEP_RES mtr_res, float step_rate, float step_accel)
{
	if (step_rate >MAX_RPM)
	{
		printf("Error: step rate cannot be larger than %d \n", MAX_RPM);
		return -1;
	}
	 int resp[1] = {0};
	uint8_t  param1, param2, cmd;
	uint32_t increm;
	param1 = mtr_dir | (mtr_res<<STEP_RES_SHIFT) | (  (uint32_t)step_rate>>STEP_RATE_SHIFT);
	param2 = (uint32_t)step_rate & STEP_RATE_MASK;
	cmd = (uint32_t)STEP_CONFIG_CMD + mtr;
	MotorPlate::sendCMD(addr, cmd, param1, param2, 0, resp);
	SPI_Slave::spiDelay(1, mS);
	if (step_accel ==0) {increm = 0;}
	else {increm = (uint32_t)(1024.0*(step_rate/(step_accel*(float)MAX_RPM))+0.5);}
	param1 = 0x80 | (increm >>8);
	param2 = increm & (0x00FF);
	cmd = STEP_CONFIG_CMD + mtr;
	MotorPlate::sendCMD(addr, cmd, param1, param2, 0,resp);
	return 0;			
}

int MotorPlate::moveStepper(PLATE_ADDR addr, STEP_MOTOR mtr, int steps)
{
	if (steps >65535)
	{
		printf("Error: step counter greater than 65,535\n");
		return -1;
	}
	int resp[1] = {0};
	uint8_t cmd =(uint32_t)STEP_MOVE_CMD + mtr;
	uint8_t param1 = steps>>8;
	uint8_t param2 = steps & 0xFF;
	MotorPlate::sendCMD(addr, cmd, param1, param2, 0, resp);
	return 0;		
}

int MotorPlate::jogStepper(PLATE_ADDR addr, STEP_MOTOR mtr)
{
	int resp[1] = {0};
	uint8_t cmd = (uint32_t)STEP_JOG_CMD + mtr;
	uint8_t param1 = 0;
	uint8_t param2 = 0;
	MotorPlate::sendCMD(addr, cmd, param1, param2, 0, resp);
	return 0;	
}

int MotorPlate::stopStepper(PLATE_ADDR addr, STEP_MOTOR mtr)
{
	int resp[1] = {0};
	uint8_t cmd = (uint32_t)STEP_STOP_CMD + mtr;
	uint8_t param1 = 0;
	uint8_t param2 = 0;
	MotorPlate::sendCMD(addr, cmd, param1, param2, 0,resp);
	return 0;	
}
int MotorPlate::putStepRate(PLATE_ADDR addr, STEP_MOTOR mtr, float rate)
{
	if (rate >MAX_RPM)
	{
		printf("Error: step rate cannot be larger than %d \n", MAX_RPM);
		return -1;
	}
	int resp[1] = {0};
	uint8_t cmd =  (uint32_t)STEP_RATE_CMD + mtr;
	uint8_t param1 = (uint32_t)rate>>8;
	uint8_t param2 = (uint32_t)rate & 0x00FF;
	MotorPlate::sendCMD(addr, cmd, param1, param2, 0,resp);
	return 0;	
}

int MotorPlate::turnStepperOff(PLATE_ADDR addr, STEP_MOTOR mtr)
{
	int resp[1] = {0};
	uint8_t cmd =  (uint32_t)STEP_OFF_CMD + mtr;
	uint8_t param1 = 0;
	uint8_t param2 = 0;
	MotorPlate::sendCMD(addr, cmd, param1, param2, 0, resp);
	return 0;
}
	
int MotorPlate::configDC(PLATE_ADDR addr, DC_MOTOR mtr, DC_MOTOR_DIR mtr_dir, float speed, float accel)
{
	if (speed>100 || speed<0)
	{
		printf("Error: Speed out of range. Must be between 0 and 100 \n");
		return -1;
	}
	if (accel>10 || accel <0)
	{
		printf("Error: Acceleration time out of range. Must be between 0 and 10 \n");
		return -1;
	}
	int resp[1] = {0};
	uint32_t speeda =  (uint32_t)((speed*1023.0/100)+0.5);
	if (mtr == DC_1 || mtr ==DC_2) {speeda = (speeda*5)>>3;}
	uint8_t param1 = (((mtr)<<6) | mtr_dir) | (speeda>>8);
	uint8_t param2 = speeda & 0x00FF;
	uint8_t cmd = DC_CONFIG1_CMD;
	MotorPlate::sendCMD(addr, cmd, param1, param2, 0,resp);
	SPI_Slave::spiDelay(1, mS);
	 uint32_t increm;
	if (accel ==0) {increm = 0;}
	else {increm =  (uint32_t)(1024.0*(speed/(accel*(float)MAX_RPM))+0.5);}
	param1 = (increm >>8);
	param2 = increm & (0x00FF);
	cmd =  (uint32_t)DC_CONFIG2_CMD + (mtr);
	MotorPlate::sendCMD(addr, cmd, param1, param2, 0,resp);
	SPI_Slave::spiDelay(1, mS);
	return 0;
}

int MotorPlate::putDCSpeed(PLATE_ADDR addr, DC_MOTOR mtr, float speed)
{
	if (speed>100 || speed<0)
	{
		printf("Error: Speed out of range. Must be between 0 and 100 \n");
		return -1;
	}
	int resp[1] = {0};
	unsigned int speeda =(unsigned int)((speed*1023.0/100)+0.5);
	if (mtr == DC_1 || mtr ==DC_2) {speeda = (speeda*5)>>3;}
	uint8_t param1 = (((mtr)<<6) ) | (speeda>>8);
	uint8_t param2 = speeda & 0x00FF;
	uint8_t cmd = DC_SPEED_CMD;
	MotorPlate::sendCMD(addr, cmd, param1, param2, 0, resp);
	SPI_Slave::spiDelay(1, mS);
	return 0;
}

int MotorPlate::startDC(PLATE_ADDR addr, DC_MOTOR mtr)
{
	int resp[1] = {0};
	uint8_t cmd = (uint8_t)DC_START_CMD;
	uint8_t param1 = mtr;
	uint8_t param2 = 0;
	int j = MotorPlate::sendCMD(addr, cmd, param1, param2, 0,resp);
	SPI_Slave::spiDelay(10, mS);
	return 0;
}


int MotorPlate::stopDC(PLATE_ADDR addr, DC_MOTOR mtr)
{
	int resp[1] = {0};
	uint8_t cmd = (uint8_t)DC_STOP_CMD;
	uint8_t param1 = mtr;
	uint8_t param2 = 0;
	int j = MotorPlate::sendCMD(addr, cmd, param1, param2, 0,resp);
	SPI_Slave::spiDelay(10, mS);
	return 0;	
}

	
int MotorPlate::getSensors(PLATE_ADDR addr)
{
	int resp[1] = {0};
	 MotorPlate::sendCMD(addr, GET_SENSOR_CMD, 0, 0, 1, resp);
	return resp[0];
}

int MotorPlate::getTachCoarse(PLATE_ADDR addr, TACH_NUM tnum)
{
	int resp[2] = {0,0};
	MotorPlate::sendCMD(addr, GET_TACHC_CMD, tnum, 0, 2,resp);
	return (resp[0]*256+resp[1]);
}

int MotorPlate::getTachFine(PLATE_ADDR addr, TACH_NUM tnum)
{
	int resp[2] = {0,0};
	MotorPlate::sendCMD(addr, GET_TACHF_CMD, tnum, 0, 2, resp);
	return (resp[0]*256+resp[1]);
}



