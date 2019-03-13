#pragma once

namespace sp {
	/*
	C++ class that wraps the wiringPi I2C functions.
	*/
	class I2C {
	private:
		int write_delay;// Write delay in milliseconds.
		int fd;

	public:
		// Constructor
		I2C(int);
		set_write_delay(int);
		int get_write_delay();

		// Communication
		read_8bit();
		read_16bit();
		write_8bit(int);
		write_16bit(int);

		const int i2c_address;

	};
}