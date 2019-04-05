#include "../../include/PanTilt.h"
#include "../../include/LidarLite.h"
#include <wiringPiI2C.h>

#include <iostream>
#include <iomanip>
#include <fstream>

#include <thread>
#include <chrono>

#include <vector>

#include <cmath>

#define TWIST_START 	-90
#define TWIST_END		0
#define TWIST_INC 	10
#define ELEV_START 	-55
#define ELEV_END		35
#define ELEV_INC 		10

sp::PanTilt pantilt;
sp::LidarLite lidar;

double deg2rad(double x){
	return x/180.0*3.1415926536;
}

void move(int angle1, int angle2){
	//~ std::cout << "Moving to (" << angle1 << "," << angle2 << ")" << std::endl;
	pantilt.set_angle(1,angle1);
	pantilt.set_angle(2,angle2);
	std::this_thread::sleep_for(std::chrono::milliseconds(1100));
	
}

void readstate(){
	int angle1 = pantilt.get_angle(1);
	int angle2 = pantilt.get_angle(2);
	
	std::cout << "Current state: (" << angle1 << "," << angle2 << ")" << std::endl;
}

std::vector<double> fwd_kin(int dist, int a1, int a2){
	
	std::vector<double> p(3);
	
	
	double d1 = 4.0;
	double ax1 = 3.0;
	double ay2 = 1.5;
	double ax2 = 6.5;
	double d2 = 2.0;
	
	
	double _dist = double(dist);
	double _a1 = deg2rad(double(a1));
	double _a2 = deg2rad(double(a2));
	double _d35 = deg2rad(35.0);
	double _t2 = _d35 - _a2;
	
	double temp_1 = ax1 + ax2 * cos(_d35 - _a2) + ay2*sin(_d35 - _a2);
	
	
	p[0] = -d2 * sin(_a1) + cos(_a1) * temp_1 + cos(_a1) * cos(_t2) * _dist;
	p[1] = d2 * cos(_a1) + sin(_a1) * temp_1 + cos(_t2) * sin(_a1) * _dist;
	p[2] = d1 - ay2 * cos(_t2) + ax2 * sin(_t2) + sin(_t2) * _dist;
	
	return p;
	
}
	




int main(int argc, char * argv[]) {
	
	std::vector<double> p(3);
	
	//output file
	std::ofstream logfile;
	logfile.open("lidar_data.log");
	
	
	std::cout << " pausing " << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	lidar.enable_free_running(true);
	pantilt.set_idle_timeout(0);
	
	if (argc == 3) {
		move(std::stoi(argv[1]),std::stoi(argv[2]));
	}
	else{
		int angle1 = TWIST_START;
		int angle2 = ELEV_START;
		for(; angle1 < TWIST_END; angle1+=TWIST_INC){
			angle2 = ELEV_START;
			for(; angle2 < ELEV_END; angle2+=ELEV_INC){
				move(angle1,angle2);
				int dist = lidar.measure_dist();
				int vel = lidar.measure_velocity();
				
				
				p = fwd_kin(dist, angle1, angle2);
				
				
				// Append to log file
				logfile << angle1 << "," << angle2 << "," << dist << "," << vel << "," << p[0] << "," << p[1] << "," << p[2] << "\n";
				//~ logfile << angle1 << "," << angle2 << "," << dist << "," << vel << "\n";
				
				// Output to terminal
				std::cout << dist << " cm " << vel << " cm/s" << std::endl; // Read distance
				
				
			}
		}
	}
	
	logfile.close();
	// We always want to wait a few extra seconds for the timeout on the servo enable/disable.
	std::this_thread::sleep_for(std::chrono::seconds(5));
	return 0;
}
