#include "wiringPi.h"
#include "wiringSerial.h"
#include <chrono>
#include <thread>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "../include/CameraCom.h"

#define CODE_START_RQST		0x01
#define CODE_ACK				0x02
#define CODE_RED				0x03
#define CODE_GREEN			0x04
#define CODE_BLOB_METH		0x05
#define CODE_CIRC_METH		0x06
#define CODE_POS_START		0x07
#define CODE_POS_CHECK1		0x08
#define CODE_POS_CHECK2		0x09

namespace sp{
	
	CameraCom::CameraCom(int timeout){
		this->fd = serialOpen("/dev/ttyUSB1",9600);
		fflush(stdout);
		this->timeout = timeout;
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
	
	std::vector<double> cam2world(int xc, int yc){
		
		
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
