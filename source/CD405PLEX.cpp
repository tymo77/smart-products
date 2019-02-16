#include "../include/mySPGPIO.h"

namespace sp {

	vector<vector<GPIO::PinModes>> CD405::PIN_TT = { 
	{GPIO::DigitalOut::LOW,		GPIO::DigitalOut::LOW	},
	{GPIO::DigitalOut::LOW,		GPIO::DigitalOut::HIGH	},
	{GPIO::DigitalOut::HIGH,	GPIO::DigitalOut::LOW	},
	{GPIO::DigitalOut::HIGH,	GPIO::DigitalOut::HIGH	}
	};

	CD405::CD405(int pinA, int pinB) {
		MyGPIO gpio;

		// set the pin modes
		gpio.pinMode(pinA, GPIO::PinModes::OUTPUT);
		gpio.pinMode(pinB, GPIO::PinModes::OUTPUT);

		// default to multiplexer config 0
		CD405::setPlex(0);
	};

	void CD405::setPlex(int path) {
		gpio.digitalWrite(pinA, CD405::PIN_TT[path][0]);
		gpio.digitalWrite(pinA, CD405::PIN_TT[path][1]);
	};
}