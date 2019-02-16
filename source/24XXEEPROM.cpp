#include "../include/24XXEEPROM.h"
#include <stdlib.h>
#include <bitset>
#include <errno.h>
#include <wiringPiI2C.h>

#include <sys/ioctl.h>

namespace sp {

	std::uint8_t MC24XX::CNTRL_W_BYTE = 0b10100000;
	std::uint8_t MC24XX::CNTRL_R_BYTE = 0b10100001;
	int MC24XX::DEF_ADDR = 0x50;
	int MC24XX::MEM_SIZE = 255;
	int MC24XX::fd;

	std::uint8_t MC24XX::castData(int i) {
		return static_cast<std::uint32_t>(i);
	}

	std::uint8_t MC24XX::castAddress(int i) {
		if (i <= MC24XX::MEM_SIZE) return E24XX::castData(i);
	};

	void MC24XX::write(int add, int data) {
		wiringPiI2CWrite8(MC24XX::fd, add, data);
	};

	int MC24XX::read(int add) {
		return wiringPiI2CRead8(MC24XX::fd, add);
	};

	MC24XX::MC24XX() {
		std::cout << "Setting up i2c for EEPROMs" << std::endl;
		MC24XX::fd = wiringPiI2CSetup(DEF_ADDR);
		std::cout << "Result: " << MC24XX::fd << std::endl;

		if (ioctl(MC24XX::fd, I2C_SLAVE, DEF_ADDR) < 0) {
			cout << "problem setting slave\n";
		}
	};

}