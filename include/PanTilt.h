#pragma once

#include "I2C.h"

namespace sp {

	class PanTilt : public I2C {
		/**
		PanTilt HAT Driver

		Communitcats with PanTilt HAT over i2c to control, pan and tilt.
		Ported from Python from: 
		http://docs.pimoroni.com/pantilthat/index.html#pantilthat.PanTilt
		**/

	private:
		int idle_timeout;
		int servo_max[2];
		int servo_min[2];
		bool is_setup;
		bool enable_servo1;
		bool enable_servo2;
		int i2c_retries;
		double i2c_retry_time;
		int fd;

		static int REG_CONFIG = 0x00;
		static int REG_SERVO1 = 0x01;
		static int REG_SERVO2 = 0x03;
		static int REG_UPDATE = 0x4E;
		static int I2C_ADDRESS = 0x15;
		static double UPDATE_WAIT = 0.03

		// Communication
		void timed_stop(double, int);
		void i2c_write_word(int, int);
		void i2c_write_byte(int, int);
		int i2c_read_word(int);
		int i2c_read_byte(int);

	public:
		// Configuration
		PanTilt();
		void setup();
		void set_config();
		void set_idle_timeout(int);

		// Conversion
		int servo_us_to_deg(int, int, int);
		int servo_deg_to_us(int, int, int);

		// Control
		void set_servo_enable(int, bool);
		int get_angle(int);
		void set_angle(int, int);
		void stop_servo(int);
	};

}