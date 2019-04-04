#ifndef SMART_PRODUCTS_LIDARLITE_H
#define SMART_PRODUCTS_LIDARLITE_H

#include "I2C.h"

namespace sp {

	class LidarLite : public I2C {
		/**
		LidarLite Driver

		Communicates with LidarLite over i2c
		Based on the doc from Garmin
		**/

	private:

		const static int DEF_I2C_ADDRESS		= 0x62;

		// Table of registers from the doc
		const static int REG_ACQ_COMMAND		= 0x00;
		const static int REG_STATUS				= 0x01;
		const static int REG_SIG_COUNT_VAL		= 0x02;
		const static int REG_ACQ_CONFIG_REG		= 0x04;
		const static int REG_VELOCITY			= 0x09;
		const static int REG_PEAK_CORR			= 0x0c;
		const static int REG_NOISE_PEAK			= 0x0d;
		const static int REG_SIGNAL_STRENGTH	= 0x0e;
		const static int REG_FULL_DELAY_HIGH	= 0x0f;
		const static int REG_FULL_DELAY_LOW		= 0x10;
		const static int REG_OUTER_LOOP_COUNT	= 0x11;
		const static int REG_REF_COUNT_VAL		= 0x12;
		const static int REG_LAST_DELAY_HIGH	= 0x14;
		const static int REG_LAST_DELAY_LOW		= 0x15;
		const static int REG_UNIT_ID_HIGH		= 0x16;
		const static int REG_UNIT_ID_LOW		= 0x17;
		const static int REG_I2C_ID_HIGH		= 0x18;
		const static int REG_I2C_ID_LOW			= 0x19;
		const static int REG_I2C_SEC_ADDR		= 0x1a;
		const static int REG_THRESHOLD_BYPASS	= 0x1c;
		const static int REG_I2C_CONFIG			= 0x1e;
		const static int REG_COMMAND			= 0x40;
		const static int REG_MEASURE_DELAY		= 0x45;
		const static int REG_PEAK_BCK			= 0x4c;
		const static int REG_CORR_DATA			= 0x52;
		const static int REG_CORR_DATA_SIGN		= 0x53;
		const static int REG_ACQ_SETTINGS		= 0x5d;
		const static int REG_POWER_CONTROL		= 0x65;
		
		const static int REG_FULL_DELAY_BOTH	= 0x8f

	public:
		// Configuration

		/*
		Set the i2c address of the device
		@param new_address	- new integer address for the device.
		*/
		void set_i2c_address(int new_address);

		/*
		Measure Lidar range.
		@returns 	- integer distance in cm.

		A receiver bias correction is performed by default. See "enable_receiver_bias_correction()"

		*/
		int measure_dist();

		/*
		Enable/disable reciever bias correction before measurement.
		@param 	enable - boolean true/false for whether to enable the receiever bias correction algorithm.

		*/
		void enable_reciever_bias_correction(bool);

		/*
		Resets the settings to the default.
		I do not believe this resets the i2c address. I think that can only be reset with a power cycle.

		*/
		void reset_settings();
		
	};

}
#endif
