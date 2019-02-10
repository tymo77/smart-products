#ifndef SMART_PRODUCTS_mySPGPIO_H
#define SMART_PRODUCTS_mySPGPIO_H

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <exception>
#include <thread>
#include <chrono>
#include <bitset>
#include <iomanip>
#include <vector>
#include <stdio.h>

#include "SPmath.h"
#include "SPmatrix.h"
#include "SPGPIO.h"

namespace sp {

	class MyGPIO : public GPIO {

	private:
		uint32_t readRegBits(void*);

		int setRegBits(void*, uint32_t);
		
		// Left shift for the function select bits for each GPIO pin in
		// its function select register.
		static int FUNC_SEL_SHIFT[];
		static int RW_SHIFT[];
		
		static GPIOregisters FUNC_SEL_REG[];
		static GPIOregisters R_REG[];
		static GPIOregisters W_H_REG[];
		static GPIOregisters W_L_REG[];

	public:
		//GPIOregisters getPinModeReg(int);

		//GPIOregisters getPinReadReg(int);

		//GPIOregisters getPinHighReg(int);

		//GPIOregisters getPinLowReg(int);
		
		/*
			Reads the current mode of the given pin.
			
			@param pin_number The pin to read the mode of.
			@returns An integer 0-7 denoting the pin mode code.
			Returns -1 if pin_number is out of range.
			
		*/
		int readPinMode(int);

		/*
			Sets the mode of the given pin.
			
			@param pin_number The pin to read the mode of.
			@param p_mode The mode to set the pin to.
			@returns 1 if successful, -1 if failed.
			
		*/
		int pinMode(int, PinModes) override;
		
		/*
			Reads the state of given pin.
			
			@param pin_number The pin to read the state of.
			@returns 1 for high and 0 for low, -1 if failed.
			
		*/
		int digitalRead(int) override;
		
		/*
			Sets the state of given pin.
			
			@param pin_number The pin to read the state of.
			@param out_value  What to set the pin to: HIGH/LOW
			@returns 1 if successful, -1 if failed.
			
		*/
		int digitalWrite(int, DigitalOut) override;
		
		/*
			Gets the PinSettings struct of a given pin.
			
			@param pin_number The pin to read the state of.
			@returns a PinSettings struct with all the pin info.
			
		*/
		PinSettings getPinSettings(int);
		
		/*
			Updates the PinSettings struct of a given pin.
			
			@param pin_number The pin to read the state of.
			@param pinSet The current pin settings.
			
		*/
		void updatePinSettings(int, PinSettings&);
		
		/*
			Displays the mode and state of all the pins to std::cout.
			
		*/
		void showPins();
		
		/*
			Displays the mode and state of select pins to std::cout.
			 
			@param pins A vector of all the pins to be displayed.
			
		*/
		void showPins(const std::vector<int> &);
	};
}

#endif
