#include<iostream>
#include<stdlib.h>
#include<vector>
#include"../../include/SPmath.h"
#include"../../include/SPmatrix.h"
#include"../../include/SPGPIO.h"
#include<exception>
#include<thread>
#include<chrono>
#include<bitset>
#include<iomanip>
#include<vector>
#include<stdio.h>

int main(int argc, char* argv[]) {

	sp::MyGPIO gpio; // enables the constructor
	int pin_num;

	if (argc == 1) {
		// starting GPIO session
		pin_num = 27; // pin to control	
		std::cout << "\ndefault pin set to " << pin_num << std::endl;
	}
	else if (argc == 2) {
		try {
			pin_num = atoi(argv[1]);
			std::cout << "\ntesting pin " << pin_num << std::endl;
		}
		catch (std::exception e) {
			std::cout << ("\nonly takes an int argument\n");
			return 0;
		}
		if ((pin_num > 27) || (pin_num < 0)) {
			std::cout << ("\npin no's valid for range 0-27\n");
			return 0;
		}
	}
	else {
		std::cout << ("\nmain.out: too many arguments\n");
		return 0;
	}


	//gpio.showPins();
	std::cout << "Toggling Pin Mode ..." << std::endl;
	gpio.pinMode(pin_num, sp::GPIO::PinModes::INPUT);
	//gpio.showPins();
	gpio.pinMode(pin_num, sp::GPIO::PinModes::OUTPUT);// set the pin mode
	std::cout << "toggle complete" << std::endl;
	//gpio.showPins();

	std::cout << "Writing Low : " << std::endl;
	gpio.digitalWrite(pin_num, sp::GPIO::DigitalOut::LOW); // set the pin to low
	int pin_val = gpio.digitalRead(pin_num); // read the value of the pin
	std::cout << "Value of pin " << pin_num << " is " << pin_val << std::endl;
	//gpio.showPins();

	std::cout << "Writing High : " << std::endl;
	gpio.digitalWrite(pin_num, sp::GPIO::DigitalOut::HIGH); //write high to pin
	pin_val = gpio.digitalRead(pin_num);// read the pin
	std::cout << "Value of pin " << pin_num << " is " << pin_val << std::endl;
	//gpio.showPins();

	return 0;
}
