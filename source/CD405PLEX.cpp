#include "../include/mySPGPIO.h"
#include "../include/CD405PLEX.h"
#include "../include/SPGPIO.h"

#include <vector>

#include <stdexcept>
#include <exception>

namespace sp {

	std::vector<std::vector<GPIO::DigitalOut>> CD405::PIN_TT = { 
	{GPIO::DigitalOut::LOW,		GPIO::DigitalOut::LOW	},
	{GPIO::DigitalOut::LOW,		GPIO::DigitalOut::HIGH	},
	{GPIO::DigitalOut::HIGH,	GPIO::DigitalOut::LOW	},
	{GPIO::DigitalOut::HIGH,	GPIO::DigitalOut::HIGH	}
	};
	
	MyGPIO CD405::gpio;

	CD405::CD405(int pinA, int pinB) {
		
		this->pinA = pinA;
		this->pinB = pinB;

		// set the pin modes
		gpio.pinMode(pinA, GPIO::PinModes::OUTPUT);
		gpio.pinMode(pinB, GPIO::PinModes::OUTPUT);

		// default to multiplexer config 0
		CD405::setPlex(0);
	};

	void CD405::setPlex(int c) {
		
		if(c > 3) throw std::runtime_error("read(): multiplexer channel out of range");
		gpio.digitalWrite(pinA, CD405::PIN_TT[c][1]);
		gpio.digitalWrite(pinB, CD405::PIN_TT[c][0]);
	};
}
