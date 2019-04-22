/*********************************************************************************************************
**********************************************************************************************************
							 DCMotor.cpp
**********************************************************************************************************
**********************************************************************************************************
	@author		Dylan DeSantis
	@date 		3/25/2018
	@version	1.0.0
**********************************************************************************************************/
#include "GPIO.h"
#include "SPI_Slave.h"
#include "DCMotor.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include <chrono>
#include <ctime>

int DCMotor::setupDCMotor(PLATE_ADDR addr, DC_MOTOR mtr_in, DC_MOTOR_DIR mtr_dir, float start_speed, float accel, int A_Encoder_Pin, int B_Encoder_Pin)
{
	this->pa = addr;
	this->mtr = mtr_in;
	this->dir = mtr_dir;
	this->pin_enA = A_Encoder_Pin;
	this->pin_enB = B_Encoder_Pin;
	MotorPlate::setup();
	MotorPlate::configDC( addr, mtr_in,  mtr_dir, start_speed, accel);
	//MotorPlate::GPIO::pinMode(A_Encoder_Pin, INPUTX);
	//MotorPlate::GPIO::pinMode(B_Encoder_Pin, INPUT);
	//pFile = fopen(file_name.c_str(),"w");
	//fprintf(pFile,"Time (s), Ref (deg/s), Ctrl (deg/s), Error (deg/s), OUTPUTX (deg/s) \n");
	return 0;
}
int DCMotor::setupController(float Kp, float Ki, float Kd, float Ts)
{
	this->K1= Kp + Ki*Ts/2.0 + Kd/Ts;
	this->K2 = Ki*Ts/2.0-Kp-2.0*Kd/Ts;
	this->K3 = Kd/Ts;
	this->Ts = Ts;
	return 0;
}
int DCMotor::closeLogger()
{
	fclose(pFile);
	return 0;
}

float DCMotor::time()
{
	std::chrono::steady_clock::time_point time_now = std::chrono::steady_clock::now();
	std::chrono::duration<float, std::micro> time_span = time_now-this->start_time ;
	float duration =(1.0/1000000.0)* ((float)(time_span.count()));
	return duration;
}

int DCMotor::sampleHold(int delay, timeScale ts)
{
	MotorPlate::delay(delay, ts);
	return 0;
}

int DCMotor::startDCMotor()
{
	MotorPlate::startDC(this->pa, this->mtr);
	this->start_time = std::chrono::steady_clock::now();
	return 0;
}

int DCMotor::stopDCMotor()
{
	MotorPlate::stopDC(this->pa, this->mtr);
	this->end_time = std::chrono::steady_clock::now();
	return 0;
}

int DCMotor::updateSpeed(float speed)
{
	float duty_cycle = (speed/((float)MAX_DPS))*100.0;
	MotorPlate::putDCSpeed(this->pa, this->mtr, duty_cycle);
	return 0;
}

int DCMotor::logSignals(float time, float ref, float ctrl, float err, float actual)
{
	fprintf(pFile,"%f, %f, %f, %f, %f \n",time, ref, ctrl, err, actual);
	return 0;
}
int DCMotor::update_error_hist(float err)
{
	this->error_sig_km2 = this->error_sig_km1;// error for the k-2 sample
	this->error_sig_km1 =this->error_sig_km0 ;// error for the k-1 sample
	this->error_sig_km0 = err; // error for the k-0 sample
	return 0;
}

int DCMotor::update_control_hist(float ctrl)
{
	this->ctrl_sig_km1 = this->ctrl_sig_km0;// control for the k-1 sample
	this->ctrl_sig_km0 = ctrl;// control for the k-0 sample
	return 0;
}

float DCMotor::saturation(float speed)
{
	if(speed > ((float)MAX_DPS)) {speed = ((float)MAX_DPS-10);}
	if(speed<0) {speed =0;}
	return speed;
}

float DCMotor::reference(float time)
{
	float speed = 0.0;
	float steady_speed = 300.0;
	float t_1 = 20.0;
	float t_2 = 25.0;
	float t_3 = 2*t_1+t_2;
	if(time <8)
	{
		speed = (steady_speed/t_1)*time;
	}else if (time>=t_1 && time<t_2+t_1)
	{
		speed = steady_speed;
	} else if(time>=t_2+t_1&& time<t_3)
	{
		speed = steady_speed-(steady_speed/t_1)*(time-t_2-t_1);
	}
	return speed;
}

float DCMotor::readSpeed()
{
	auto SA = MotorPlate::GPIO::digitalRead(this->pin_enA );
	auto SB = MotorPlate::GPIO::digitalRead(this->pin_enB );
	//std::cout<< SA<<SB<<std::endl;
	float t_begin_to = DCMotor::time();
	while( (MotorPlate::GPIO::digitalRead(this->pin_enA ) == SA) || (MotorPlate::GPIO::digitalRead(this->pin_enB ) == SB))
		 {
		 	//std::cout<<"StateA: "<<MotorPlate::GPIO::digitalRead(this->pin_enA ) <<std::endl;
		 	//std::cout<<"StateB: "<<MotorPlate::GPIO::digitalRead(this->pin_enB ) <<std::endl;
		 	if ((DCMotor::time()-t_begin_to)>this->Ts) {return 0.0;}
		 }
	float t_initial = DCMotor::time();
	SA = MotorPlate::GPIO::digitalRead(this->pin_enA );
	SB= MotorPlate::GPIO::digitalRead(this->pin_enB );
	while( (MotorPlate::GPIO::digitalRead(this->pin_enA ) == SA) && (MotorPlate::GPIO::digitalRead(this->pin_enB ) ==SB)) {}
	float t_final = DCMotor::time();
	float dt = t_final-t_initial; // time for one pulse  .... s/pulse
	// w = deg/s     PPR = pulse/rev    dt  = s/pulse     PPR*dt = s/rev
	// 1/(PPR*dt) = rev/s      360/(PPR*dt) = deg/s
	float speed = 360.0/(((float)PPR)*dt);
	printf("Speed: %f \n", speed);
	return speed;
}

int DCMotor::controlSpeed()
{
	float actual = readSpeed();
	float t_now = DCMotor::time();
	//float ref = reference(t_now);
	float ref = 500.00;
	float err = ref-actual;
	//std::cout<<"error: "<<err<<std::endl;
	//std::cout<<"ref: "<<ref<<std::endl;
	DCMotor::update_error_hist(err);
	float ctrl_sig = ctrl_sig_km1 + K1*error_sig_km0+K2*error_sig_km1+ K3*error_sig_km2;
	//std::cout<<"ctrl "<<ctrl_sig<<std::endl;
	ctrl_sig = DCMotor::saturation(ctrl_sig);
	DCMotor::update_control_hist(ctrl_sig);
	DCMotor::updateSpeed(ctrl_sig);
	DCMotor::logSignals(t_now, ref, ctrl_sig, err, actual);
	return 0;
}

