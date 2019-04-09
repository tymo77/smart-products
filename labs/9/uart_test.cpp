#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <errno.h> 
#include <math.h> 
#include <signal.h> 
#include <unistd.h> 
#include <wiringPi.h> 
#include <wiringSerial.h> 
#include	<sstream> 
#include <fstream> 
#include <string.h>

#include <thread>
#include <chrono>

#include <exception>
#include <stdexcept>

#include <vector>


#define CODE_START_RQST	0x01
#define CODE_ACK			0x02
#define CODE_GET_CIRCLES 0x05

class Circle {
	public:
	
	int x, y, r;
	int rgb[3];
	int index;
	
	Circle(int _index, int _x, int _y, int _r, int _color_r, int _color_g, int _color_b){
		index = _index;
		x = _x;
		y = _y;
		r = _r;
		rgb[0] = _color_r;
		rgb[1] = _color_g;
		rgb[2] = _color_b;
	}
	
};

int wait_and_read(int fd, int timeout){
	std::chrono::duration<double> wctduration;
	auto wcts = std::chrono::system_clock::now();
	while(0==serialDataAvail(fd)){
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		wctduration = (std::chrono::system_clock::now() - wcts);
		if(wctduration.count() > 2){
			return -1;
		}
	}
	
	int msg = serialGetchar(fd);
	//~ std::cout << int(msg) << std::endl;
	return msg;
}

std::vector<Circle> get_circles(int fd){
	// Check if it is ready
	int attempt = 0;
	while(true){
		serialPutchar(fd, CODE_START_RQST);
		
		if(wait_and_read(fd,1.0) == CODE_ACK){ break;}
		if(attempt > 3){throw std::runtime_error("Failed to comm with camera in 3 tries!");}
		attempt++;
	}
	
	// Send request for circles and get the number back.
	serialPutchar(fd,CODE_GET_CIRCLES);
	int num_circs = wait_and_read(fd,1.0);
	if(num_circs < 0){throw std::runtime_error("Bad number of circles!");}
	std::cout << "Getting " << num_circs << " circles..." << std::endl;
	
	// Get all the circles
	int last_index, index, x, y, r, color_r, color_g, color_b;
	last_index = -1;
	
	std::vector<Circle> circles;
	
	if(num_circs > 0){
		for(int i = 0; i < num_circs; i++){
			index = wait_and_read(fd, 1.0);
			x = wait_and_read(fd, 1.0);
			y = wait_and_read(fd, 1.0);
			r = wait_and_read(fd, 1.0);
			color_r = wait_and_read(fd, 1.0);
			color_g = wait_and_read(fd, 1.0);
			color_b = wait_and_read(fd, 1.0);
			
			if(!(index == last_index + 1) ){throw std::runtime_error("I think we missed an index.");}
			
			Circle circle (index,x,y,r,color_r,color_g,color_b);
			circles.push_back(circle);
			last_index = index;
		} 
		
		// Print all the circles out
	
		for(auto it = circles.begin(); it != circles.end(); it++){
			Circle c = *it;
			printf("Circle %3d at (%3d,%3d), r = %3d, rgb = (%d,%d,%d)\n",c.index,c.x,c.y,c.r,c.rgb[0],c.rgb[1],c.rgb[2]);
		}
	}
	
	return circles;
}

int main (void){
	int fd1 = serialOpen("/dev/ttyUSB0",9600);
	wiringPiSetup();
	fflush(stdout);
	std::vector<Circle> circles = get_circles(fd1);
	
	serialClose(fd1);
	return 0;
}
