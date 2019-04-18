#include "wiringPi.h"
#include "wiringSerial.h"
#include <chrono>
#include <thread>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <math.h>

#include "../include/CameraCom.h"
#include "../include/SPmath.h"
#include "../include/SPmatrix.h"
#include "../include/Robot.h"
#include "../include/command.h"

#define CODE_START_RQST		0x01
#define CODE_ACK				0x02
#define CODE_RED				0x03
#define CODE_GREEN			0x04
#define CODE_BLOB_METH		0x05
#define CODE_CIRC_METH		0x06
#define CODE_POS_START		0x07
#define CODE_POS_CHECK1		0x08
#define CODE_POS_CHECK2		0x09

#define TCtoL3_val { { -0.2067262659881,  -0.1259735299686,   0.9702550802227,  -13.4027918953717},{ 0.6968225094874,  -0.7150834271410,  0.0556244775255, -14.9523189507702},{ 0.6868061161800,0.6875946203800,0.2356077180445,-110.9046610928553 }, {0,0,0,1.0} }
#define P_cam_origin_val {{131.0813957486520}, {120.2907553493345}, {145.6236902265848}, {1.0}}

namespace sp{
	
	CameraCom::CameraCom(int timeout, double plane_height){
		this->fd = serialOpen("/dev/ttyUSB1",9600);
		fflush(stdout);
		this->plane_height = plane_height;
		this->timeout = timeout;
		this->TCtoL3_data = TCtoL3_val;
		MatF temp(TCtoL3_data);
		this->TCtoL3 = temp;
		
		this->P_cam_origin_data = P_cam_origin_val;
		MatF temp2(P_cam_origin_data);
		this->P_cam_origin = temp2;
	}
	
	int CameraCom::wait_and_read(){
		std::chrono::duration<double> wctduration;
		auto wcts = std::chrono::system_clock::now();
		while(0==serialDataAvail(this->fd)){
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			wctduration = (std::chrono::system_clock::now() - wcts);
			if(wctduration.count() > this->timeout){
				return -1;
			}
		}
		
		int msg = serialGetchar(fd);
		//~ std::cout << int(msg) << std::endl;
		return msg;
	}
	
	std::vector<double> CameraCom::cam2world(Target t, Pose p){
		
		double xrob = p.x;
		double yrob = p.y;
		double zrob = p.z;
		
		double u = double(t.x); double v = double(t.y);
		
		
		// Plane equation coefficients
		double a1 = u*this->M[2][0] - this->M[0][0];
		double b1 = u*this->M[2][1] - this->M[0][1];
		double c1 = u*this->M[2][2] - this->M[0][2];
		
		double a2 = v*this->M[2][0] - this->M[1][0];
		double b2 = v*this->M[2][1] - this->M[1][1];
		double c2 = v*this->M[2][2] - this->M[1][2];
		
		// Plane intersection direction vector components.
		double v1 = (b2*c1)/(a2*b1-a1*b2) - (b1*c2)/(a2*b1-a1*b2);
		double v2 = -(a2*c1)/(a2*b1-a1*b2) + (a1*c2)/(a2*b1-a1*b2);
		double v3 = 1.0;
		MatF_t v_C_data = {{float(v1)},{float(v2)},{float(v3)}};
		MatF v_C(v_C_data);
		
		// {C} -> {L3}
		
		MatF_t RCtoL3_data = initIdent(3);
		for(int i = 0; i < 3; i++){
			for(int j = 0; j < 3; j++){
				RCtoL3_data[i][j] = this->TCtoL3_data[i][j];
			}
		}
		
		MatF RCtoL3(RCtoL3_data);
		MatF v_L3 = RCtoL3*v_C;
		MatF a_L3 = this->TCtoL3*this->P_cam_origin;
		
		// {L3} -> {W}
		double theta = atan2(yrob,xrob); // Get the rotation of L3;
		
		MatF_t RL3toW_data = {{float(cos(theta)),float(-sin(theta)),0.0},{float(sin(theta)),float(cos(theta)),0.0},{0.0,0.0,float(1.0)}};
		MatF RL3toW(RL3toW_data);
		
		MatF_t TL3toW_data = {{float(cos(theta)),float(-sin(theta)),0.0,float(xrob)},{float(sin(theta)),float(cos(theta)),0.0,float(yrob)},{0.0,0.0,1.0,float(zrob)},{0.0,0.0,0.0,1.0}};
		MatF TL3toW(TL3toW_data);
		
		MatF v_W = RL3toW*v_L3;
		MatF a_W = TL3toW*a_L3;
		
		
		double tt = (this->plane_height - a_W.getMatData()[2][0])/v_W.getMatData()[2][0];
		
		double x_W = tt*v_W.getMatData()[0][0] + a_W.getMatData()[0][0];
		double y_W = tt*v_W.getMatData()[1][0] + a_W.getMatData()[1][0];

		vector<double> P(3);
		P[0] = x_W; P[1] =y_W; P[2] = plane_height;
		
		return P;
	}
	
	std::vector<Target> CameraCom::get_targets(int min_radius, int max_radius, int color_index, int method_index, int pos_index){
		
		int color_code, method_code, pos_code;
		
		switch(color_index){
			case 1:
				color_code = CODE_RED;
				break;
			case 2:
				color_code = CODE_GREEN;
				break;
			default:
				throw std::runtime_error("Bad color index.");
		}
		
		switch(method_index){
			case 1:
				method_code = CODE_BLOB_METH;
				std::cout << "Searching using blob..." << std::endl;
				break;
			case 2:
				method_code = CODE_CIRC_METH;
				std::cout << "Searching using circ..." << std::endl;
				break;
			default:
				throw std::runtime_error("Bad method index.");
		}
		
		switch(pos_index){
			case 1:
				pos_code = CODE_POS_START;
				break;
			case 2:
				pos_code = CODE_POS_CHECK1;
				break;
			case 3:
				pos_code = CODE_POS_CHECK2;
				break;
			default:
				throw std::runtime_error("Bad position check index.");
		}
		
		// Check if it is ready
		int attempt = 0;
		while(true){
			serialPutchar(this->fd, CODE_START_RQST);
			
			if(this->wait_and_read() == CODE_ACK){ break;}
			if(attempt > 5){throw std::runtime_error("Failed to comm with camera!");}
			attempt++;
		}
		
		// Send request parameters.
		serialPutchar(this->fd,color_code);
		serialPutchar(this->fd,min_radius);
		serialPutchar(this->fd,max_radius);
		serialPutchar(this->fd,method_code);
		serialPutchar(this->fd,pos_code);

		int num_targets = this->wait_and_read();
		if(num_targets < 0){throw std::runtime_error("Bad number of targets!");}
		std::cout << "Getting " << num_targets << " targets..." << std::endl;
		
		// Get all the targets
		int last_index, index, x, y;
		last_index = -1;
		
		std::vector<Target> targets;
		
		if(num_targets > 0){
			for(int i = 0; i < num_targets; i++){
				index = this->wait_and_read();
				x = this->wait_and_read();
				y = this->wait_and_read();
				
				if(!(index == last_index + 1) ){throw std::runtime_error("I think we missed an index.");}
				
				Target target (index,x,y);
				targets.push_back(target);
				last_index = index;
			} 
			
			// Print all the circles out
			for(auto it = targets.begin(); it != targets.end(); it++){
				Target t = *it;
				printf("Target %3d at (%3d,%3d)\n",t.index,t.x,t.y);
			}
		}
		
		return targets;
	}
	
}
