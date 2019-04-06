#include "../../include/PanTilt.h"
#include "../../include/LidarLite.h"
#include <wiringPiI2C.h>

#include <iostream>
#include <iomanip>
#include <fstream>

#include <thread>
#include <chrono>

#include <exception>
#include <stdexcept>

#include <vector>
#include <algorithm>

#include <cmath>

#define TWIST_START 	-90
#define TWIST_END		0
#define TWIST_INC 	3
#define ELEV_START 	-55
#define ELEV_END		35
#define ELEV_INC 		3

#define NUM_SAMPLES 5 // Number of samples to take
#define NUM_DISCARD 1 // Number of outliers to discard

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

void remove_outliers(std::vector<int> & v){
	
	std::vector<int>::iterator min_el, max_el;
	
	// Erase a certain number of the highest and lowest elements
	for(int i = 0; i < NUM_DISCARD; i++){
		max_el = std::max_element(v.begin(),v.end());
		v.erase(max_el);
		
		min_el = std::min_element(v.begin(),v.end());
		v.erase(min_el);
	}
}

int average(std::vector<int> & v){
	
	std::vector<int>::iterator min_el, max_el;
	int avg = 0;
	int count = 0;
	
	// Compute averages
	for(std::vector<int>::iterator it = v.begin(); it != v.end(); ++it){
			avg += *it;
			count++;
	}
	avg /= count;
	return avg;
}


int main(int argc, char * argv[]) {
	
	// Check validity of setup.
	if (2*NUM_DISCARD > NUM_SAMPLES) {
		throw std::runtime_error("Bad setup. No samples leftover after discarding outliers!!"); 
	}
	
	int dist, vel, count;
	std::vector<double> p(3);
	std::vector<int> dists(NUM_SAMPLES);
	std::vector<int> vels(NUM_SAMPLES);
	
	std::vector<int> dists_avg;
	std::vector<int> vels_avg;
	
	// Prepare output file.
	std::ofstream logfile;
	logfile.open("lidar_data.log");//Open file, clear contents, and write headers
	logfile << "Angle 1, Angle 2, Distance (cm), Velocity (cm/s), x, y, z\n";
	
	// Pause.
	std::cout << " pausing " << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	
	// Setup.
	lidar.enable_free_running(true);
	pantilt.set_idle_timeout(0);
	
	// Move to position if arguments provided (for testing).
	if (argc == 3) {
		move(std::stoi(argv[1]),std::stoi(argv[2]));
	}
	else{
		int angle1 = TWIST_START;
		int angle2 = ELEV_START;
		for(; angle1 < TWIST_END; angle1+=TWIST_INC){
			angle2 = ELEV_START;
			for(; angle2 < ELEV_END; angle2+=ELEV_INC){
				logfile.open("lidar_data.log", std::ios::out | std::ios::app);//Open file as append output
				move(angle1,angle2);
				
				dists_avg.clear(); vels_avg.clear();
				// Get a certain number of samples
				for(int i = 0; i < NUM_SAMPLES; i++){
					dists[i] = lidar.measure_dist();
					vels[i] = lidar.measure_velocity();
					std::this_thread::sleep_for(std::chrono::milliseconds(150));
				}
				dists_avg = dists;
				vels_avg = vels;
					
				remove_outliers(dists_avg);
				remove_outliers(vels_avg);
				
				dist = average(dists_avg);
				vel = average(vels_avg);
				
				p = fwd_kin(dist, angle1, angle2);
				
				
				// Append to log file
				logfile << angle1 << "," << angle2 << "," << dist << "," << vel << "," << p[0] << "," << p[1] << "," << p[2] << "\n";
				//~ logfile << angle1 << "," << angle2 << "," << dist << "," << vel << "\n";
				
				// Output to terminal
				std::cout << dist << " cm " << vel << " cm/s" << std::endl; // Read distance
				logfile.close();// Close and write file.
				
			}
		}
	}
	
	logfile.close();
	// We always want to wait a few extra seconds for the timeout on the servo enable/disable.
	std::this_thread::sleep_for(std::chrono::seconds(5));
	return 0;
}
