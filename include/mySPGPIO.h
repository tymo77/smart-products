#ifndef SMART_PRODUCTS_myGPIO_H
#define SMART_PRODUCTS_myGPIO_H

#include<iostream>
#include<stdlib.h>
#include<vector>
#include<exception>
#include<thread>
#include<chrono>
#include<bitset>
#include<iomanip>
#include<vector>
#include<stdio.h>

#include"SPmath.h"
#include"SPmatrix.h"
#include"SPGPIO.h"

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

		int readPinMode(int);

		int pinMode(int, PinModes) override;

		int digitalRead(int) override;

		int digitalWrite(int, DigitalOut) override;

		PinSettings getPinSettings(int);
		
		PinSettings updatePinSettings(int);

		void showPins();

		void showPins(const std::vector<int> &);
	};
}

#endif
