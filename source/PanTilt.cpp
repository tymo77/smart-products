#include "../include/PanTilt.h"
#include "../include/I2C.h"
#include <wiringPiI2C.h>

#include <stdexcept>
#include <exception>
#include <stdlib.h>
#include <iostream>

#include <thread>         // std::this_thread::sleep_for, std::thread
#include <chrono>         // std::chrono::seconds

#define I2C_SLAVE	0x0703// Code to tell ioctl to set an i2c slave.

constexpr int W_DELAY = 4000;// Write delay in milliseconds.

namespace sp {
	
	/*
	Constructs a PanTilt object with default settings.
	*/
	PanTilt::PanTilt() : I2C::I2C(PanTilt::I2C_ADDRESS) {

		idle_timeout = 2; // Idle timeout in seconds.

		i2c_retries = 10; // Number of attempts to send i2c before it fails.
		i2c_retry_time = 10;// Time in milliseconds between i2c retry attempts.

		enable_servo1 = false;
		enable_servo2 = false;

		servo_min[0] = 575;
		servo_min[1] = 575;

		servo_max[0] = 2325;
		servo_max[1] = 2325;
		
		is_setup = false;
		
		fd = 0;// Master hardware i2c address?

		setup();
	};

	/*
	Sets up the PanTilt HAT.
	*/
	void PanTilt::setup() {
		if (this->is_setup) {
			return;
		}
		
		this->set_config();
		this->is_setup = true;
		return;
	};

	/*
	Set the timeout in seconds.

	The time that the servo has to move before it is shutdown.
	*/
	void PanTilt::set_idle_timeout(int tout) {
		this->idle_timeout = tout;
	}

	/*
	Set the configuration state of the PanTilt HAT.
	
	Updates the configuration state of the HAT to the state of the master program.
	Writes a configuration byte to a configuration register (REG_CONFIG).
	*/
	void PanTilt::set_config() {
		unsigned int config = 0;
		config |= this->enable_servo1;
		config |= (this->enable_servo2 << 1);
		this->i2c_write_byte(PanTilt::REG_CONFIG, config);
	}

	/*
	Convert pulse time (microseconds) to degrees.
	@param angle	- integer pulse-width in microseconds.
	@param us_min	- integer minimum pulse-width allowed.
	@param us_max	- integer maximum pulse-width allowed.

	@returns		- integer angle in degrees.
	*/
	int PanTilt::servo_us_to_deg(int us, int us_min, int us_max) {
		if (us > us_min && us < us_max) {
			int servo_range = us_max - us_min;
			double angle = (double(us - us_min) / double(servo_range))*180.0;
			return int(angle) - 90;
		}
		else {
			throw std::runtime_error("Pulse width out of range.");
			return -1;
		}
	};

	/*
	Convert degrees to pulse time (microseconds).
	@param angle	- integer angle to convert to pulse-width.
	@param us_min	- integer minimum pulse-width allowed.
	@param us_max	- integer maximum pulse-width allowed.

	@returns		- integer pulse-width in microseconds.
	*/
	int PanTilt::servo_deg_to_us(int angle, int us_min, int us_max) {
		if (angle > -90 && angle < 90) {
			angle += 90;
			int servo_range = us_max - us_min;
			double us = double(servo_range) / 180.0 * double(angle);
			return int(us) + us_min;
		}
		else {
			throw std::runtime_error("Angle out of range.");
			return -1;
		}
	};

	/*
	Enable a servo.
	@param index	- integer index of servo to enable (1 or 2).
	@param state	- bool (true/false) for enabled-ness.
	*/
	void PanTilt::set_servo_enable(int index, bool state) {

		// Run setup.
		this->setup();

		if (index == 1) {
			this->enable_servo1 = state;
		}
		else if (index == 2) {
			this->enable_servo2 = state;
		}
		else {
			throw std::runtime_error("Can't enable. Bad servo index.");
		}

		// Send the configuration.
		this->set_config();
		return;
	};
	
	/*
	Get the angle of a servo.
	@ param index	- integer index of the servo to read (1 or 2).
	*/
	int PanTilt::get_angle(int index) {
		
		// Run setup (will short-circuit if already setup).
		this->setup();
		int read_add = 0;

		if (index == 1) {
			// SERVO 1
			read_add = PanTilt::REG_SERVO1;
		}
		else if (index == 2) {
			// SERVO 2
			read_add = PanTilt::REG_SERVO2;
		}
		else {
			throw std::runtime_error("Can't get servo angle. Bad servo index.");
		}
		
		// Read i2c word.
		int us = this->i2c_read_word(read_add);

		try{
			// Try to convert the pulse with to an angle.
			return this->servo_us_to_deg(us,servo_min[index],servo_max[index]);
		}
		catch (const std::exception& e){
			// If there is an error. Report a zero.
			std::cerr << "Warning: " << e.what() << std::endl;
			return 0;
		}
	};

	/*
	Set the angle of a servo.
	@param index	- integer index of servo to command (1 or 2).
	@param angle	- integer angle that the servo should go to.
	*/
	void PanTilt::set_angle(int index, int angle) {
		// Run setup (will short-circuit if already setup).
		this->setup();
		int reg = 0;

		if (index == 1) {
			// SERVO 1
			if (!this->enable_servo1) {
				this->enable_servo1 = true;
				this->set_config();
			}
			reg = PanTilt::REG_SERVO1;
		}
		else if (index == 2) {
			// SERVO 2
			if (!this->enable_servo2) {
				this->enable_servo2 = true;
				this->set_config();
			}
			reg = PanTilt::REG_SERVO2;
			angle = std::min(angle, 20);// Don't go below 20 degrees for servo2
		}
		else {
			throw std::runtime_error("Can't command servo. Bad servo index.");
		}
		int us = this->servo_deg_to_us(angle, servo_min[index], servo_max[index]);
		this->i2c_write_word(reg, us);

		// Build a thread to run the servo movement timeout timer.
		if (this->idle_timeout > 0) {
			std::thread end_thread(&PanTilt::timed_stop,this,this->idle_timeout,index);
			end_thread.detach();// Detach this thread so it can run in the background...
				//... after it goes out of scope.
		}
		return;
	};
	
	/*
	Stop the servo given by index.
	@param index	- integer index of servo to stop (1 or 2);
	*/
	void PanTilt::stop_servo(int index) {
		if (index == 1) {
			this->enable_servo1 = false;
		}
		else if (index == 2) {
			this->enable_servo2 = false;
		}
		else {
			throw std::runtime_error("bad servo index");
		}
		this->set_config();
		return;
	};

	/*
	Write a byte of int data to int address on the PanTilt HAT.
	@param address	- integer address of register to write to
	@param data		- integer data to write

	i2c_write_byte will attempt to write this->i2c_retries times before throwing an error.
	*/
	void PanTilt::i2c_write_byte(int address, int data) {
		// Try up to a certain number of times to write to device.
		for (int i = 0; i < this->i2c_retries; ++i) {
			try{
				this->write_8bit(address, data);
				return;
			}
			catch (const std::exception& e){
				std::this_thread::sleep_for(std::chrono::milliseconds(this->i2c_retry_time));
				std::cerr << "Error writing byte attempt " << i << ": " << e.what() << std::endl;
				continue;
			}
		}

		throw std::runtime_error("Failed to write byte via i2c.");
	};

	/*
	Write a word (16bit) of int data to int address on the PanTilt HAT.
	@param address	- integer address of register to write to.
	@param data		- integer data to write.

	i2c_write_word will attempt to write this->i2c_retries times before throwing an error.
	*/
	void PanTilt::i2c_write_word(int address, int data) {
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
	Read a byte of data from the PanTilt HAT.
	@param address	- integer register address to read from.
	@returns		- an integer of the data.
	*/
	int PanTilt::i2c_read_byte(int address) {
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
	Read a word of data from the PanTilt HAT.
	@param address	- integer register address to read from.
	@returns		- an integer of the data.
	*/
	int PanTilt::i2c_read_word(int address) {
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

	/*
	Function to be run on a separate thread from main execution.
	Waits for a time before stopping the servo.

	@param secs		- integer  time in seconds to wait.
	@param index	- integer index of servo to stop.
	*/
	void PanTilt::timed_stop(int secs, int index) {
		std::this_thread::sleep_for(std::chrono::seconds(secs));
		this->stop_servo(index);
		return;
	};
}
