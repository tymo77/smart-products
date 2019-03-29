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

		std::cout << "Setting up i2c device with device id: " << std::setbase(16);
		std::cout << i2c_address << std::endl << std::setbase(10);

		// Using wiringPi to setup the device i2c connection and get hardware id.
		fd = wiringPiI2CSetup(i2c_address);
		std::cout << "New device hardware id: " << fd << std::endl;

		// Check that everything went okay.
		if (ioctl(fd, I2C_SLAVE, i2c_address) < 0) {
			std::cout << "Problem setting slave." << std::endl;
		}
		else {
			std::cout << "Device setup succeeded." << std::endl;
		}
		
	};

	/*
	Set write delay (delays thread execution while waiting for write command to finish).
	@param		- integer delay in microseconds.
	*/
	void I2C::set_write_delay(int delay) {
		this->write_delay = delay;
	}

	/*
	Get write delay (delays thread execution while waiting for write command to finish).
	@returns	- integer delay in microseconds.
	*/
	int I2C::get_write_delay() {
		return this->write_delay;
	}
	
	/*
	Set read delay (delays thread execution while waiting for read command to finish).
	@param		- integer delay in microseconds.
	*/
	void I2C::set_read_delay(int delay) {
		this->read_delay = delay;
	}

	/*
	Get read delay (delays thread execution while waiting for read command to finish).
	@returns	- integer delay in microseconds.
	*/
	int I2C::get_read_delay() {
		return this->read_delay;
	}
	
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
}
