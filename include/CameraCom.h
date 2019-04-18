#ifndef SP_CAMERACOM
#define SP_CAMERACOM

#include <vector>
#include "wiringPi.h"
#include "wiringSerial.h"
#include <iostream>
#include <iomanip>

#include "SPmath.h"
#include "SPmatrix.h"
#include "../include/Robot.h"
#include "../include/command.h"

#define Mval { { -0.642777679224007, 0.316755371768381,  -0.253984571204965,  83.139646376366443},{ 0.158771887454431,   0.183395601402561,  -0.627224625240108,  48.465928494817007 },{ -0.002796222501437,  -0.001757883663208,  -0.002897949484508,   1.000000000000000 } }

namespace sp{
	
	class Target {
	public:
	
	int x, y;
	int index;
	
	Target(int _index, int _x, int _y){
		index = _index;
		x = _x;
		y = _y;
		
	}
	
	};
	
	
	class CameraCom{
	
	private:
	int timeout;
	int fd;
	double M[3][4] = Mval; // Camera calibration matrix.
	double plane_height;
	
	// Camera frame {C} to link frame {L3} transformation matrix.
	MatF_t TCtoL3_data;
	MatF TCtoL3;
	
	// Position of camera center in {C}.
	MatF_t P_cam_origin_data;
	MatF P_cam_origin;
	
	public:
	
	CameraCom(int timeout, double plane_height);
	std::vector<Target> get_targets(int min_radius, int max_radius, int color_index, int method_index, int pos_index);
	int wait_and_read();
	std::vector<double> cam2world(Target t, Pose p);
	
	~CameraCom(){
		serialClose(this->fd);
	}
	
		
	};
	
}

#endif
