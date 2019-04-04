#ifndef SMART_PRODUCTS_I2C_H
#define SMART_PRODUCTS_I2C_H

namespace sp {
	/*
	C++ class that wraps the wiringPi I2C functions.
	*/
	class I2C {
	private:
		
		int fd;
		int i2c_address;

	public:

		int write_delay;	// Write delay in milliseconds.
		int read_delay;		// Read delay in milliseconds.

		int i2c_retries;	// Number of attempts to send i2c before it fails for resilient comms.
		int i2c_retry_time; // Time in milliseconds between i2c retry attempts for resilient comms.

		// Constructor
		I2C(int);

		// Setup
		void setup_i2c();

		// Single attempt reads
		int read_8bit();
		int read_8bit(int);
		int read_16bit(int);

		// Single attempt writes
		void write_8bit(int);
		void write_8bit(int, int);
		void write_16bit(int, int);

		// Communication with multiple attempts
		int resilient_read_8bit(int);
		int resilient_read_16bit(int);
		void resilient_write_8bit(int, int);
		void resilient_write_16bit(int, int);

	};
}
#endif
