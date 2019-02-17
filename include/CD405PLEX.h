#ifndef SMART_PRODUCTS_CD405PLEX_H
#define SMART_PRODUCTS_CD405PLEX_H

#include <vector>
#include "../include/mySPGPIO.h"
#include "../include/SPGPIO.h"

namespace sp {

	class CD405 {

	private:
		static std::vector<std::vector<GPIO::DigitalOut>> PIN_TT;
		int pinA;
		int pinB;
		static MyGPIO gpio;

	public:
		CD405(int, int);
		void setPlex(int);

	};
}

#endif
