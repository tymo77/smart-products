#include "../include/PanTilt.h"
#include "../include/I2C.h"
#include <wiringPiI2C.h>

#include <stdexcept>
#include <exception>
#include <stdlib.h>
#include <cmath>
#include <iostream>

#include <thread>         // std::this_thread::sleep_for, std::thread
#include <chrono>         // std::chrono::seconds

constexpr int W_DELAY = 4000;// Write delay in milliseconds.

namespace sp {
	
	/*
	Constructs a PanTilt object with default settings.
	*/
	PanTilt::PanTilt() : I2C::I2C(PanTilt::I2C_ADDRESS) {

		idle_timeout = 2; // Idle timeout in seconds.

		enable_servo1 = false;
		enable_servo2 = false;

		servo_min[0] = 575;
		servo_min[1] = 575;

		servo_max[0] = 2325;
		servo_max[1] = 2325;
		
		is_setup = false;

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
		this->resilient_write_8bit(PanTilt::REG_CONFIG, config);
	}

	/*
	Convert pulse time (microseconds) to degrees.
	@param angle	- integer pulse-width in microseconds.
	@param us_min	- integer minimum pulse-width allowed.
	@param us_max	- integer maximum pulse-width allowed.

	@returns		- integer angle in degrees.
	*/
	int PanTilt::servo_us_to_deg(int us, int us_min, int us_max) {
		if ((us >= us_min) && (us <= us_max)) {
			int servo_range = us_max - us_min;
			double angle = (double(us - us_min) / double(servo_range))*180.0;
			return int(std::round(angle)) - 90;
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
		if (angle >= -90 && angle <= 90) {
			angle += 90;
			int servo_range = us_max - us_min;
			double us = double(servo_range) / 180.0 * double(angle);

			return int(std::round(us)) + us_min;
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
		int us = this->resilient_read_16bit(read_add);

		try{
			// Try to convert the pulse with to an angle.
			return this->servo_us_to_deg(us,servo_min[index-1],servo_max[index-1]);
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
			angle = std::max(angle, -55);// Don't go below -30 degrees for servo2
		}
		else {
			throw std::runtime_error("Can't command servo. Bad servo index.");
		}
		int us = this->servo_deg_to_us(angle, servo_min[index-1], servo_max[index-1]);
		this->resilient_write_16bit(reg, us);

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
		//~ std::cout << "Stopping servo " << index << std::endl;
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
	
	PanTilt::~PanTilt(){
		this->stop_servo(1);
		this->stop_servo(2);
	}
}
