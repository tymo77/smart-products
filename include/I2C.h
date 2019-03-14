#ifndef SMART_PRODUCTS_I2C_H
#define SMART_PRODUCTS_I2C_H

namespace sp {
	/*
	C++ class that wraps the wiringPi I2C functions.
	*/
	class I2C {
	private:
		int write_delay;// Write delay in milliseconds.
		int fd;
		int i2c_address;

	public:
		// Constructor
		I2C(int);
		void set_write_delay(int);
		int get_write_delay();

		// Communication
		int read_8bit(int);
		int read_16bit(int);
		void write_8bit(int, int);
		void write_16bit(int, int);

	};
}
#endif
