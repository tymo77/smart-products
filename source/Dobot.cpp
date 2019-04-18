#include "../include/Dobot.h"
#include "../include/Robot.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

namespace sp{
	
	Dobot::Dobot(){
		this->setup();
	}
	
	void Dobot::goTo(double x, double y, double z){
		this->goToXYZ(x,y,z,true);
	}
	
	void Dobot::suctionOff(){
		this->setPump(false,true);
	}
	
	void Dobot::suctionOn(){
		this->setPump(true,true);
	}
	
	void Dobot::printPose(){
		Pose pose = this->getPose();
		printf("at position x =  % 7.2f y = % 7.2f z = % 7.2f r = % 7.2f\n",pose.x,pose.y,pose.z,pose.r);
	}
	
	void Dobot::setRamp1(){
		Pose pose = this->getPose();
		this->p_ramp1[0] = pose.x;
		this->p_ramp1[1] = pose.y;
		this->p_ramp1[2] = pose.z;
	}
	void Dobot::setRamp2(){
		Pose pose = this->getPose();
		this->p_ramp2[0] = pose.x;
		this->p_ramp2[1] = pose.y;
		this->p_ramp2[2] = pose.z;
	}
	void Dobot::setHome(){
		Pose pose = this->getPose();
		this->p_home[0] = pose.x;
		this->p_home[1] = pose.y;
		this->p_home[2] = pose.z;
	}
	
	void Dobot::getReferences(){
		std::cout << "Record home position. The camera should be able to see the whole grid.\n Move to desired position and press ENTER." << std::endl;
		cin.get();
		this->setHome();
		std::cout << "Record ramp1 position. Move to desired position and press ENTER." << std::endl;
		cin.get();
		this->setRamp1();
		std::cout << "Record ramp2 position. Move to desired position and press ENTER." << std::endl;
		cin.get();
		this->setRamp2();
	}
	
	void Dobot::saveReferences(){
		char delim = ',';
		// Prepare output file.
		std::ofstream logfile;
		std::cout << "Saving reference positions ..." << std::endl;
		logfile.open("reference_positions.log",std::ios::out);//Open file, clear contents.
		logfile << this->p_home[0] << delim << this->p_home[1] << delim << this->p_home[2] << "\n";
		logfile << this->p_ramp1[0] << delim << this->p_ramp1[1] << delim << this->p_ramp1[2] << "\n";
		logfile << this->p_ramp2[0] << delim << this->p_ramp2[1] << delim << this->p_ramp2[2] << "\n";
		logfile.close();
	}
	void Dobot::loadReferences(){
		char delim = ',';
		std::ifstream logfile;
		std::string line;
		std::string temp;
		std::stringstream lineStream(line);
		
		
		logfile.open("reference_positions.log",std::ios::in);
		
		
		std::getline(logfile,line);
		lineStream.str(line);lineStream.clear();
		std::cout << "Home position: ";
		for(int i = 0; i < 3; i++){
			std::getline(lineStream,temp,delim);
			p_home[i] = stod(temp);
			std::cout << p_home[i] << ", ";
		}
		std::cout << endl;
		
		
		std::getline(logfile,line);
		lineStream.str(line);lineStream.clear();
		std::cout << "Ramp1 position: ";
		for(int i = 0; i < 3; i++){
			std::getline(lineStream,temp,delim);
			p_ramp1[i] = stod(temp);
			std::cout << p_ramp1[i] << ", ";
		}
		std::cout << endl;
		
		
		std::getline(logfile,line);
		lineStream.str(line);lineStream.clear();
		std::cout << "Ramp2 position: ";
		for(int i = 0; i < 3; i++){
			std::getline(lineStream,temp,delim);
			p_ramp2[i] = stod(temp);
			std::cout << p_ramp2[i] << ", ";
		}
		std::cout << endl;
		logfile.close();
	}
	
	void Dobot::goHome(){this->goTo(p_home[0],p_home[1],p_home[2]);}
	void Dobot::goRamp1(){this->goTo(p_ramp1[0],p_ramp1[1],p_ramp1[2]);}
	void Dobot::goRamp2(){this->goTo(p_ramp2[0],p_ramp2[1],p_ramp2[2]);}
}


