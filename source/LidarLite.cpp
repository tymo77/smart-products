#include "../include/LidarLite.h"
#include "../include/I2C.h"
#include <wiringPiI2C.h>

#include <stdexcept>
#include <exception>
#include <stdlib.h>
#include <iostream>
#include <iomanip>			// set base cstream

#include <thread>         // std::this_thread::sleep_for, std::thread
#include <chrono>         // std::chrono::seconds
#include <cmath>


namespace sp {

	LidarLite::LidarLite() : I2C::I2C(LidarLite::DEF_I2C_ADDRESS) {

		enable_receiver_bias_correction(true);
		wait_timeout = 200;//Milliseconds?
		this->read_delay = 25;
		this->write_delay = 0;
		this->reset_settings();

	};

	/*
	Set the i2c address of the device 
	@param new_address	- new integer address for the device.

	*/
	void LidarLite::set_i2c_address(int new_address) {
		
		std::cerr << "CAUTION: CHANGING THE I2C ADDRESS IS CURRENTLY UNSTABLE" << std::endl;
		std::cout << "Changing i2c address of LidarLite device to 0x" << std::setbase(16) << new_address << std::setbase(10) << std::endl;
		takeI2CBus();
		
		try {
			
			
			if(new_address % 2 == 1){
				throw std::runtime_error("Least significant bit for this device must be 0.");
			}
			if(new_address > 0x77 || new_address < 0x04){
				throw std::runtime_error("Address must be a valid i2c address.");
			}
			
			// 1) Read the two-byte serial number.
			this->write_8bit(LidarLite::REG_UNIT_ID_HIGH);
			int sn_high = this->read_8bit();
			
			this->write_8bit(LidarLite::REG_UNIT_ID_LOW);
			int sn_low = this->read_8bit();

			// 2) Write the serial number high byte.
			this->resilient_write_8bit(LidarLite::REG_I2C_ID_HIGH, sn_high);

			// 3) Write the serial number low byte.
			this->resilient_write_8bit(LidarLite::REG_I2C_ID_LOW, sn_low);

			// 4) Write the desired i2c address.
			this->resilient_write_8bit(LidarLite::REG_I2C_SEC_ADDR, new_address);

			// 5) Disable the default i2c address.
			this->resilient_write_8bit(LidarLite::REG_I2C_CONFIG, 0x08);

			// Setup new address on the Pi side.
			this->i2c_address = new_address;
			this->setup_i2c();
		}
		catch(const std::exception & e){
			std::cerr << "Failed to set new address: " << e.what() << std::endl;
			std::cerr << "You may have to cycle the power to the Lidar module." << std::endl;
		}
	};

	/*
	Measure Lidar range.
	@returns 	- integer distance in cm.

	A receiver bias correction is performed by default. See "enable_receiver_bias_correction()"

	*/
	int LidarLite::measure_dist() {

		//~ // Get start time for measuring time elapsed and timing out.
		//~ std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		takeI2CBus();
		if(not free_running){
			if(bias_correct){
				this->write_8bit(LidarLite::REG_ACQ_COMMAND,0x04);
				}
			else{
				this->write_8bit(LidarLite::REG_ACQ_COMMAND,0x03);
			}
					
		}


		// Read high byte
		this->write_8bit(REG_FULL_DELAY_HIGH);
		uint16_t high_byte = this->read_8bit();
		//~ std::cout << "High = " << high_byte;
		
		// Read low bye
		this->write_8bit(REG_FULL_DELAY_LOW);
		uint16_t low_byte = this->read_8bit();
		//~ std::cout << ", Low = " << low_byte << std::endl;
		
		
		//~ return both_byte;
		return low_byte | (high_byte << 8);
	};
	
	/*
	Measure Lidar velocity.
	@returns 	- integer velocity in cm/s.

	*/
	int LidarLite::measure_velocity() {

		takeI2CBus();
		// Read low bye
		this->write_8bit(REG_VELOCITY);
		int8_t vel = this->read_8bit();
		
		//convert from .1m/s to cm/s;
		return int(vel)*100;
	};

	/*
	Enable/disable reciever bias correction before measurement.
	@param 	enable - boolean true/false for whether to enable the receiever bias correction algorithm.

	*/
	void LidarLite::enable_receiver_bias_correction(bool enable) {
		takeI2CBus();
		if (enable) {
			this->write_8bit(LidarLite::REG_ACQ_COMMAND, 0x04);
		}
		else {
			this->write_8bit(LidarLite::REG_ACQ_COMMAND, 0x03);
		}
		this->bias_correct = enable;
	};
	
	/*
	Enable/disable free running measurement mode at 10hz.
	@param 	enable - boolean true/false.

	*/
	void LidarLite::enable_free_running(bool enable){
		takeI2CBus();
		if (enable) {
			this->write_8bit(LidarLite::REG_OUTER_LOOP_COUNT, 0xff);
		}
		else {
			this->write_8bit(LidarLite::REG_OUTER_LOOP_COUNT, this->measurements_per_cmd);
		}
		free_running = enable;
		if (this->bias_correct) {
			this->write_8bit(LidarLite::REG_ACQ_COMMAND, 0x04);
		}
		else {
			this->write_8bit(LidarLite::REG_ACQ_COMMAND, 0x03);
		}
	}
	
	/*
		Set the number of measurements to be taken per command
		@param count - new integer number of measurements taken per command 0 corresponds to 1.
	*/
	void LidarLite::set_measurements_per_cmd(int count){
		takeI2CBus();
		this->write_8bit(LidarLite::REG_OUTER_LOOP_COUNT,count);
		this->measurements_per_cmd = count;
	};

	/*
	Resets the settings to the default.
	I do not believe this resets the i2c address. I think that can only be reset with a power cycle.
	
	*/
	void LidarLite::reset_settings() {
		takeI2CBus();
		this->resilient_write_8bit(LidarLite::REG_ACQ_COMMAND, 0);
	};
	
	int LidarLite::takeI2CBus()
{
	/*
	Sets the I2C to communicate with this device
	*/
	if (ioctl(this->fd, I2C_SLAVE, this->DEF_I2C_ADDRESS) < 0)
	{
		printf("problem setting slave\n");
		return -1;
	}
	return 0;
}

}
