#ifndef SP_CAMERACOM
#define SP_CAMERACOM

#include <vector>
#include "wiringPi.h"
#include "wiringSerial.h"
#include <iostream>
#include <iomanip>

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
	
	public:
	
	CameraCom(int timeout);
	std::vector<Target> get_targets(int min_radius, int max_radius, int color_index, int method_index, int pos_index);
	int wait_and_read();
	
	~CameraCom(){
		serialClose(this->fd);
	}
	
		
	};
	
}

#endif
