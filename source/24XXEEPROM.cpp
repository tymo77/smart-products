#include "../include/24XXEEPROM.h"
#include <stdlib.h>
#include <iostream>

#include <bitset>
#include <errno.h>
#include <wiringPiI2C.h>

#include <chrono>
#include <thread>

#include <sys/ioctl.h>

#include <stdexcept>
#include <exception>

#define I2C_SLAVE	0x0703

constexpr int W_DELAY = 4000;
constexpr int R_DELAY = 0;

namespace sp {

	int MC24XX::DEF_ADDR = 0x50;
	int MC24XX::MEM_SIZE = 256;

	void MC24XX::write(int add, int data) {
		if(add >= MC24XX::MEM_SIZE) throw std::runtime_error("write(): address out of range");
		wiringPiI2CWriteReg8(MC24XX::fd, add, data);
		std::this_thread::sleep_for(std::chrono::microseconds(W_DELAY));
	};

	int MC24XX::read(int add) {
		if(add >= MC24XX::MEM_SIZE) throw std::runtime_error("read(): address out of range");
		int r = wiringPiI2CReadReg8(MC24XX::fd, add);
		//~ std::this_thread::sleep_for(std::chrono::microseconds(R_DELAY));
		return r;
	};

	MC24XX::MC24XX() {

		std::cout << "Setting up i2c for EEPROMs" << std::endl;
		MC24XX::fd = wiringPiI2CSetup(DEF_ADDR);
		std::cout << "Result: " << MC24XX::fd << std::endl;

		if (ioctl(MC24XX::fd, I2C_SLAVE, DEF_ADDR) < 0) {
			std::cout << "problem setting slave\n";
		}
	};

}
