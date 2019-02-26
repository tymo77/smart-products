#ifndef SP_DOTBOT
#define SP_DOTBOT

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

#include "Protocol.h"
#include "command.h"
#include "Robot.h"
#include<functional>

class Dobot : public Robot
{
public:
	std::function<void(Dobot&)> mRoutine;
};

void RobotRoutine(Dobot &bot)
{

		return;	
}

#endif
