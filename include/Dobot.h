#ifndef SP_DOTBOT
#define SP_DOTBOT

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

#include "Protocol.h"
#include "command.h"
#include "Robot.h"
#include <functional>

namespace sp{

	class Dobot : public Robot{
		
	public:
		Dobot();
		void goTo(double x, double y, double z);
		void printPose();
		void setRamp1();
		void setRamp2();
		void setHome();
		void goHome();
		void goRamp1();
		void goRamp2();
		void getReferences();
		void saveReferences();
		void loadReferences();
		
	private:
		int fd;
		double p_home[3];
		double p_ramp1[3];
		double p_ramp2[3];
	};

}
#endif
