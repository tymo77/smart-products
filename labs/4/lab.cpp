#include <iostream>
#include <iomanip>

#include <stdlib.h>

#include "../../include/mySPGPIO.h"
#include "../../include/24XXEEPROM.h"
#include "../../include/CD405PLEX.h"

constexpr int PIN_A = 17;
constexpr int PIN_B = 27;

int main() {

	sp::CD405	cd405(PIN_A, PIN_B);
	sp::MC24XX	mc24xx;

	cd405.setPlex(1);
	mc24xx.write(0, 69);
	mc24xx.write(1, 68);
	std::cout << mc24xx.read(0) << std::endl;
	std::cout << mc24xx.read(1) << std::endl;

	return 0;
}
