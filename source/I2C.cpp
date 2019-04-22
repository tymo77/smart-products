#include "../include/I2C.h"
#include <wiringPiI2C.h>

#include <exception>
#include <stdexcept>

#include <iostream>
#include <iomanip>

#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#include <sys/ioctl.h>
#include <stdlib.h>

#define I2C_SLAVE	0x0703 // Code to set slave for ioctl.

namespace sp {

	/*
	Construct an I2C object.
	@param device_address	- integer i2c address of the device.
	*/
	I2C::I2C(int device_address) {
		i2c_address = device_address;
		write_delay = 4000;//Milliseconds
		read_delay = 0;//Milliseconds
		fd = 0;

		i2c_retries = 10; // Number of attempts to send i2c before it fails.
		i2c_retry_time = 10;// Time in milliseconds between i2c retry attempts.

		setup_i2c();
	};

	int I2C::get_fd() {
		return this->fd;
	}

	void I2C::setup_i2c() {
		std::cout << "Setting up i2c device with device id: " << std::setbase(16);
		std::cout << this->i2c_address << std::endl << std::setbase(10);

		// Using wiringPi to setup the device i2c connection and get hardware id.
		this->fd = wiringPiI2CSetup(this->i2c_address);
		std::cout << "New device hardware id: " << fd << std::endl;

		// Check that everything went okay.
		if (ioctl(fd, I2C_SLAVE, this->i2c_address) < 0) {
			std::cout << "Problem setting slave." << std::endl;
		}
		else {
			std::cout << "Device setup succeeded." << std::endl;
		}
	};

	/*
	Attempt to write a byte of int data to device.
	@param data		- integer data to write.
	*/
	void I2C::write_8bit(int data) {

		try {
			wiringPiI2CWrite(this->fd, data);
			std::this_thread::sleep_for(std::chrono::microseconds(this->write_delay));
			return;
		}
		catch (const std::exception& e) {
			std::cerr << "Error writing byte. " << e.what() << std::endl;
			return;
		}
	};

	/*
	Attempt to write a byte of int data to device.
	@param address	- integer address of register on device to write to.
	@param data		- integer data to write.
	*/
	void I2C::write_8bit(int address, int data) {

		try {
			wiringPiI2CWriteReg8(this->fd, address, data);
			std::this_thread::sleep_for(std::chrono::microseconds(this->write_delay));
			return;
		}
		catch (const std::exception& e) {
			std::cerr << "Error writing byte. " << e.what() << std::endl;
			return;
		}
	};

	/*
	Attempt to write a word of int data to device.
	@param address	- integer address of register on device to write to.
	@param data		- integer data to write.
	*/
	void I2C::write_16bit(int address, int data) {
		try {
			wiringPiI2CWriteReg16(this->fd, address, data);
			std::this_thread::sleep_for(std::chrono::microseconds(this->write_delay));
			return;
		}
		catch (const std::exception& e) {
			std::cerr << "Error writing byte. " << e.what() << std::endl;
			return;
		}
	};

	/*
	Attempt to read a byte of data from the device.
	@param address	- integer register address to read from.
	@returns		- an integer of the data.
	*/
	int I2C::read_8bit(int address) {
		try {
			int r = wiringPiI2CReadReg8(this->fd, address);
			std::this_thread::sleep_for(std::chrono::microseconds(this->read_delay));
			return r;
		}
		catch (const std::exception& e) {
			std::cerr << "Error reading byte. " << e.what() << std::endl;
		}
		return 0;
	}

	/*
	Attempt to read a byte of data from the device.
	@returns		- an integer of the data.
	*/
	int I2C::read_8bit() {
		try {
			int r = wiringPiI2CRead(this->fd);
			std::this_thread::sleep_for(std::chrono::microseconds(this->read_delay));
			return r;
		}
		catch (const std::exception& e) {
			std::cerr << "Error reading byte. " << e.what() << std::endl;
		}
		return 0;
	}

	/*
	Attempt to read a word of data from the device.
	@param address	- integer register address to read from.
	@returns		- an integer of the data.
	*/
	int I2C::read_16bit(int address) {
		try {
			int r = wiringPiI2CReadReg16(this->fd, address);
			std::this_thread::sleep_for(std::chrono::microseconds(this->read_delay));
			return r;
		}
		catch (const std::exception& e) {
			std::cerr << "Error reading word. " << e.what() << std::endl;
		}
		return 0;
	}

	/*
	Write a byte of int data to int address on the LidarLite.
	@param address	- integer address of register to write to
	@param data		- integer data to write
	i2c_resilient_write_8bit will attempt to write this->i2c_retries times before throwing an error.
	*/
	void I2C::resilient_write_8bit(int address, int data) {
		// Try up to a certain number of times to write to device.
		for (int i = 0; i < this->i2c_retries; ++i) {
			try {
				this->write_8bit(address, data);
				return;
			}
			catch (const std::exception& e) {
				std::this_thread::sleep_for(std::chrono::milliseconds(this->i2c_retry_time));
				std::cerr << "Error writing byte attempt " << i << ": " << e.what() << std::endl;
				continue;
			}
		}

		throw std::runtime_error("Failed to write byte via i2c.");
	};

	/*
	Write a word (16bit) of int data to int address on the LidarLite.
	@param address	- integer address of register to write to.
	@param data		- integer data to write.
	i2c_resilient_write_16bit will attempt to write this->i2c_retries times before throwing an error.
	*/
	void I2C::resilient_write_16bit(int address, int data) {
		// Try up to a certain number of times to write to device.
		for (int i = 0; i < this->i2c_retries; i++) {
			try {
				this->write_16bit(address, data);
				return;
			}
			catch (const std::exception& e) {
				std::this_thread::sleep_for(std::chrono::milliseconds(this->i2c_retry_time));
				std::cerr << "Error writing word attempt " << i << ": " << e.what() << std::endl;
				continue;
			}
		}

		throw std::runtime_error("Failed to write word via i2c.");
	};

	/*
	Read a byte of data from the LidarLite.
	@param address	- integer register address to read from.
	@returns		- an integer of the data.
	Will try again a certain number of times if there is an error.
	*/
	int I2C::resilient_read_8bit(int address) {
		// Try up to a certain number of times to read from device.
		for (int i = 0; i < this->i2c_retries; i++) {
			try {
				int r = this->read_8bit(address);
				return r;
			}
			catch (const std::exception& e) {
				std::this_thread::sleep_for(std::chrono::milliseconds(this->i2c_retry_time));
				std::cerr << "Error reading byte attempt " << i << ": " << e.what() << std::endl;
				continue;
			}
		}

		throw std::runtime_error("Failed to read byte via i2c.");
		return 0;
	};

	/*
	Read a word of data from the LidarLite.
	@param address	- integer register address to read from.
	@returns		- an integer of the data.
	Will try again a certain number of times if there is an error.
	*/
	int I2C::resilient_read_16bit(int address) {
		// Try up to a certain number of times to read from device.
		for (int i = 0; i < this->i2c_retries; i++) {
			try {
				int r = this->read_16bit(address);
				return r;
			}
			catch (const std::exception& e) {
				std::this_thread::sleep_for(std::chrono::milliseconds(this->i2c_retry_time));
				std::cerr << "Error reading word attempt " << i << ": " << e.what() << std::endl;
				continue;
			}
		}

		throw std::runtime_error("Failed to read word via i2c.");
		return 0;
	};
}
