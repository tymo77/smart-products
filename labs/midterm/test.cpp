#include "../../include/I2C.h"
#include <iostream>

int main(int argc, char* argv[]) {
	int data;
	// Get user input to write to device.
	// Gets the first character of the user input, and sends that.
	if (argc > 1) {
		data = int(char(*argv[1])) ;
	}
	else{
		data = 0;
	}
	
	// Setup i2c device with an address.
	sp::I2C arduinoi2c(4);
	
	// Write the data.
	std::cout << data << std::endl;
	arduinoi2c.write_8bit(data);
	
	// Read data back and print.
	//std::cout << arduinoi2c.read_8bit() << std::endl;
	return 0;
}
