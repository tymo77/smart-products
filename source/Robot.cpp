
 /**
 //====================================================================
//          		Robot.cpp
//====================================================================

=================================================================
	@author 		Dylan DeSantis
	@date		2/16/2018
	@version	1.0.0
*/

#include "Robot.h"
#include "command.h"
#include "Protocol.h"


extern int fd;


//	Serial Methods
//==========================================
/*
	void 			initRAM(void);
	void				setInitParams();
	void 			setRxInterrupt(int usecs);
	static void 		serialRead(int sig_num);
	uint8_t 			serialRead();
	static void 		serialWrite(uint8_t);
*/
//==========================================

void 	Robot::initRAM(void)
{
    /**
      This function initiates the parameters for the
      robot including the maximum end effector velocity
      the mode used in the point to point (ptp) routine,
      and the location of the home position.

      **/
	//Set PTP Model
    	gPTPCoordinateParams.xyzVelocity = 220;
    	gPTPCoordinateParams.rVelocity = 100;
    	gPTPCoordinateParams.xyzAcceleration = 100;
    	gPTPCoordinateParams.rAcceleration = 80;
    	gPTPCommonParams.velocityRatio = 80;
	gPTPCommonParams.accelerationRatio = 80;

	//	Setup the mode of movement
	gPTPCmd.ptpMode = MOVL_XYZ;
	
	//	Setup  Parameters
	gPTPCmd.x = 200;
	gPTPCmd.y = 0;
	gPTPCmd.z = 0;
	gPTPCmd.r = 0;
	
	gQueuedCmdIndex = 0;
	
	//	Set Home Position
	gHOMEParams.x = 200;
	gHOMEParams.y = 0;
	gHOMEParams.z = 0;
	gHOMEParams.r = 90;
	
	//	Robot Pose
	gPose.x	= 0;
	gPose.y  = 0;
	gPose.z  = 0;

	
	//	Robot Kinematics
	gKinematics.velocity = 0;
	gKinematics.acceleration = 0;
	
	//	Robot Wait
	gWAITCmd.timeout = 0; 
	
	//	Robot Suction
	suction = false;
	
	return;
}

void	Robot::setInitParams()
{

    SetPTPCoordinateParams(&gPTPCoordinateParams, true, &gQueuedCmdIndex);
    SetPTPCommonParams(&gPTPCommonParams, true, &gQueuedCmdIndex);
	SetHOMEParams(&gHOMEParams, true, &gQueuedCmdIndex);	
	ProtocolProcess();  
	return;
}

 void 	Robot::setRxInterrupt(int usecs)
{

        signal(SIGALRM, Robot::serialRead);
	ualarm((useconds_t)usecs,(useconds_t)usecs);
	return;
}


 void 	Robot::setup()
 {

	 setRxInterrupt(100);
	 initRAM();
	 ProtocolInit();
	 setInitParams();
	 return;
 }


void	Robot::serialRead(int sig_num)
{
	if (sig_num == SIGALRM){
		while(serialDataAvail(fd)) {
			uint8_t data = serialGetchar(fd);
        		if (RingBufferIsFull(&gSerialProtocolHandler.rxRawByteQueue) == false) {
            			RingBufferEnqueue(&gSerialProtocolHandler.rxRawByteQueue, &data);
        		}
  		}
  	}
  	return;
}

uint8_t	Robot::serialRead()
{
	return serialGetchar(fd);
}

void	Robot::serialWrite(uint8_t data)
{
	serialPutchar(fd, data);
	return;
}

//	Robot Commands 
//==========================================
/*
	void				setHome(float x, float y, float z, float q4, bool waitEnd);
	void 			goToHome(void);	
	void 			goToXYZ(float x, float y, float z, bool waitEnd);			
	void 			goToQ(float q1, float q2, float q3, float q4);
	void 			setEndEffector(float x, float y, float z, float q4, bool waitEnd); 
	Pose		 	getPose(); 							
	void 			setPump( bool on_off, bool waitEnd);					
	uint8_t 			getPump();						
	void				wait(uint32_t timeout_ms);
*/
//==========================================

void	Robot::setHome(float x, float y, float z, float q4, bool waitEnd)
{
	/**
		Use this function to set the home position given the x,y,z coordinates
		and the rotation of the end effector. The waitEnd is a boolean input
		that allows us to decide if we wish to wait until the Dobot has finished
		executing this command in its queue to continue on with our program.
		**/
	
	gHOMEParams.x = x;
	gHOMEParams.y = y;
	gHOMEParams.z = z;
	gHOMEParams.r  = q4;
	SetHOMEParams(&gHOMEParams, true, &gQueuedCmdIndex);
	ProtocolProcess();   

    	// Check whether the command is finished
    	do {
        	if (waitEnd == false) {
            		break;
       		}
        	uint64_t currentIndex;
       		GetQueuedCmdCurrentIndex(&currentIndex);
        	if (currentIndex >= gQueuedCmdIndex) {
        			break;
        	}
    	} while (1);
	return;
}

void	Robot::goToHome()
{
	
	SetHOMECmd(&gHOMECmd, true, &gQueuedCmdIndex);
	ProtocolProcess(); 
    	do {
        	uint64_t currentIndex;
       		GetQueuedCmdCurrentIndex(&currentIndex);
        	if (currentIndex >= gQueuedCmdIndex) {
        			break;
        	}
    	} while (1);	
	return;
}

void	Robot::goToXYZ(float x, float y, float z, bool waitEnd)
{
	/**
		Use this function to go to the position in space (x,y,z).The waitEnd is a boolean input
		that allows us to decide if we wish to wait until the Dobot has finished
		executing this command in its queue to continue on with our program.
		**/	
	gPTPCmd.x = x;
	gPTPCmd.y = y;
	gPTPCmd.z = z;
	SetPTPCmd(&gPTPCmd, true, &gQueuedCmdIndex);
	ProtocolProcess();  
    	do {
        	if (waitEnd == false) {
            		break;
       		}
        	uint64_t currentIndex;
       		GetQueuedCmdCurrentIndex(&currentIndex);
        	if (currentIndex >= gQueuedCmdIndex) {
        			break;
        	}
    	} while (1);
    	do {
        	if (waitEnd == false) {
            		break;
       		}
        	Pose tpose;
        	Pose tpose_up;
        	Pose tpose_dn;
       		GetPose(&tpose);
       		tpose_up.x =  tpose.x +1;
       		tpose_up.y =  tpose.y +1;
       		tpose_up.z = tpose.z +1;
       		tpose_dn.x =  tpose.x -1;
       		tpose_dn.y =  tpose.y -1;
       		tpose_dn.z = tpose.z -1;
        	if (gPTPCmd.x >tpose_dn.x &&
        		gPTPCmd.y >tpose_dn.y &&
        		gPTPCmd.z >tpose_dn.z &&
        		gPTPCmd.x <tpose_up.x &&
        		gPTPCmd.y <tpose_up.y &&
        		gPTPCmd.z <tpose_up.z 	
        	) {
        			break;
        	}
    	} while (1);    	
	return;
}

void 	Robot::goToQ(float q1, float q2, float q3, float q4)
{
	return;
}

void	Robot::setEndEffector(float x, float y, float z, float q4, bool waitEnd)
{
	gPTPCmd.x = x;
	gPTPCmd.y = y;
	gPTPCmd.z = z;	
	gPTPCmd.r  = q4;
	SetPTPCmd(&gPTPCmd, true, &gQueuedCmdIndex);
	ProtocolProcess();
    	do {
        	if (waitEnd == false) {
            		break;
       		}
        	uint64_t currentIndex;
       		GetQueuedCmdCurrentIndex(&currentIndex);
        	if (currentIndex >= gQueuedCmdIndex) {
        			break;
        	}
    	} while (1);  
	return;
}

Pose Robot::getPose()
{
	GetPose(&gPose);
	ProtocolProcess();   
	return this->gPose;
}
						
void Robot::setPump( bool on_off, bool waitEnd)
{
	/**
		Use this function to set the state of the pump.  The waitEnd is a boolean input
		that allows us to decide if we wish to wait until the Dobot has finished
		executing this command in its queue to continue on with our program.
	**/
	SetEndEffectorSuctionCup(on_off, true, &gQueuedCmdIndex);
	ProtocolProcess();
    	do {
        	if (waitEnd == false) {
            		break;
       		}
        	uint64_t currentIndex;
       		GetQueuedCmdCurrentIndex(&currentIndex);
        	if (currentIndex >= gQueuedCmdIndex) {
        			break;
        	}
    	} while (1);
	return;
}				

uint8_t	Robot::getPump()
{
	return 1;
}

void	Robot::wait(uint32_t timeout_ms)
{
	/**
		Use this function to have the dobot pause for a specified amount
		of time in milliseconds. 
	**/
	gWAITCmd.timeout = timeout_ms;
	SetWAITCmd(&gWAITCmd, true, &gQueuedCmdIndex);
	ProtocolProcess();
    	do {

        	uint64_t currentIndex;
       		GetQueuedCmdCurrentIndex(&currentIndex);
        	if (currentIndex >= gQueuedCmdIndex) {
        			break;
        	}
    	} while (1);  
	return;
}							
















