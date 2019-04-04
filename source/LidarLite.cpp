#include "../include/LidarLite.h"
#include "../include/I2C.h"
#include <wiringPiI2C.h>

#include <stdexcept>
#include <exception>
#include <stdlib.h>
#include <iostream>

#include <thread>         // std::this_thread::sleep_for, std::thread
#include <chrono>         // std::chrono::seconds


namespace sp {

	LidarLite::LidarLite() : I2C::I2C(LidarLite::DEF_I2C_ADDRESS) {

		enable_reciever_bias_correction(true);

	};

	/*
	Set the i2c address of the device 
	@param new_address	- new integer address for the device.

	*/
	void LidarLite::set_i2c_address(int new_address) {
		
		try {
			
			// 1) Read the two-byte serial number.
			int sn_high = this->resilient_read_8bit(LidarLite::REG_UNIT_ID_HIGH);
			int sn_low = this->resilient_read_8bit(LidarLite::REG_UNIT_ID_LOW);

			// 2) Write the serial number high byte.
			this->resilient_write_8bit(LidarLite::REG_I2C_ID_HIGH, sn_high);

			// 3) Write the serial number low byte.
			this->resilient_write_8bit(LidarLite::REG_I2C_ID_LOW, sn_low);

			// 4) Write the desired i2c address.
			this->resilient_write_8bit(LidarLite::REG_I2C_SEC_ADDR, new_address);

			// 5) Disable the default i2c address.
			this->resilient_write_8bit(LidarLite::REG_I2C_CONFIG, 8);

			// Setup new address on the Pi side.
			this->i2c_address = new_address;
			this->setup_i2c();
		}
		catch(const std::exception & e){
			std::cerr << "Failed to set new address: " e.what() << std::endl;
			std::cerr << "You may have to cycle the power to the Lidar module." << std::endl;
		}
	};

	/*
	Measure Lidar range.
	@returns 	- integer distance in cm.

	A receiver bias correction is performed by default. See "enable_receiver_bias_correction()"

	*/
	int LidarLite::measure_dist() {

		// Get start time for measuring time elapsed and timing out.
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		// Wait until it is in ready state.
		while ((this->resilient_read_8bit(REG_STATUS) & 1) > 0) {
			// End program after idle timeout seconds.
			std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
			std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(now - begin);
			if (time_span.count() > (double) this->idle_timeout) {
				throw std::runtime_error("LidarLite: busy flag did not enter ready state before timeout.");
				return -1;
			}
		}

		//int both_byte = this->i2c_read_16bit(REG_FULL_DELAY_BOTH);

		int high_byte = this->resilient_read_8bit(REG_FULL_DELAY_HIGH);
		int low_byte = this->resilient_read_8bit(REG_FULL_DELAY_LOW);

		return low_byte + 2 ^ 8 * high_byte;
	};

	/*
	Enable/disable reciever bias correction before measurement.
	@param 	enable - boolean true/false for whether to enable the receiever bias correction algorithm.

	*/
	void LidarLite::enable_reciever_bias_correction(bool enable) {
		if (enable) {
			this->resilient_write_8bit(LidarLite::REG_ACQ_COMMAND, 4);
		}
		else {
			this->resilient_write_8bit(LidarLite::REG_ACQ_COMMAND, 3);
		}
	};

	/*
	Resets the settings to the default.
	I do not believe this resets the i2c address. I think that can only be reset with a power cycle.
	
	*/
	void LidarLite::reset_settings() {
		this->resilient_write_8bit(LidarLite::REG_ACQ_COMMAND, 0);
	};

}