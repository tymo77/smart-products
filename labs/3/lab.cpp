#include<iostream>
#include<stdlib.h>
#include<vector>
#include"../../include/SPmath.h"
#include"../../include/SPmatrix.h"
#include"../../include/SPGPIO.h"

namespace sp {

	class MyGPIO : public GPIO {

		int pinMode(int pin_number, PinModes p_mode) override {
			// pin_number -> pin we want to set.
			// p_mode -> the mode we want to set this pin to.
			// sets pin_number to p_mode

			// Get pointer to memory register.
			void* pin_ptr;
			switch (pin_number / 10) {
			case 0:
				pin_ptr = getPtr(GPIOregisters::SR0);
				break;
			case 1:
				pin_ptr = getPtr(GPIOregisters::SR1);
				break;
			case 2:
				pin_ptr = getPtr(GPIOregisters::SR2);
				break;
			case 3:
				pin_ptr = getPtr(GPIOregisters::SR3);
				break;
			case 4:
				pin_ptr = getPtr(GPIOregisters::SR4);
				break;
			case 5:
				pin_ptr = getPtr(GPIOregisters::SR4);
				break;
			default:
				return -1;//Return if out of range.
			}

			// Get current state of target register.
			uint32_t pinmode_now = *(reinterpret_cast<std::uint32_t *>(pin_ptr));

			// Get pinmode code to set target bits in target register.
			uint32_t p_mode_code = static_cast<std::uint32_t>(p_mode);

			// Shift the target pin over to the correct bits in target register.
			int pin_number_shift = (pin_number % 10) * 3;
			uint32_t des_output = p_mode_code << pin_number_shift

				// Mask 000's over the target bits in target register.
				uint32_t mask = (~(0x7 << pin_number_shift));

			// Get new state for the target register.
			des_output = (pinmode_now & mask) | (des_output & ~mask);

			// Set target register.
			*(reinterpret_cast<std::uint32_t *>(pin_ptr)) = des_output;

			return 1;//Return 1 if mode didn't fail.
		}
		int digitalRead(int pin_number) override {

			// Get pointer to memory register.
			void* pin_ptr;
			switch (pin_number / 32) {
			case 0:
				pin_ptr = getPtr(GPIOregisters::Read0);
				break;
			case 1:
				pin_ptr = getPtr(GPIOregisters::Read1);
				break;
			default:
				return -1;//Return if out of range.
			}

			// Get current state of target register.
			uint32_t pinmode_now = *(reinterpret_cast<std::uint32_t *>(pin_ptr));

			// Mask "...010..." over the target bit in target register.
			int pin_number_shift = pin_number % 32;
			uint32_t mask = (0x1 << pin_number_shift);

			int result = static_cast<int>(mask & pinmode_now);

			return result;
		}
		int digitalWrite(int pin_number, DigitalOut out_value) override {

			// Get pointer to memory register.
			void* pin_ptr;
			switch (pin_number / 32) {
			case 0:
				if (out_value == DigitalOut::HIGH) {
					register_type = GPIOregisters::WriteHigh0;
				}
				else if (out_value == DigitalOut::LOW) {
					register_type = GPIOregisters::WriteLow0;
				}
				else { return -1; }
				break;
			case 1:
				if (out_value == DigitalOut::HIGH) {
					register_type = GPIOregisters::WriteHigh1;
				}
				else if (out_value == DigitalOut::LOW) {
					register_type = GPIOregisters::WriteLow1;
				}
				else { return -1; }
				break;
			default:
				return -1;//Return if out of range.
			}
			pin_ptr = getPtr(register_type);

			// Get current state of target register.
			uint32_t pinmode_now = *(reinterpret_cast<std::uint32_t *>(pin_ptr));

			// Shift the target pin over to the correct bits in target register.
			int pin_number_shift = (pin_number % 32);
			uint32_t des_output = (0x1 << pin_number_shift;

			// Mask "...11011..." over the target bit in target register.
			uint32_t mask = (~(0x1 << pin_number_shift));

			// Get new state for the target register.
			des_output = (pinmode_now & mask) | (des_output & ~mask);

			// Set target register.
			*(reinterpret_cast<std::uint32_t *>(pin_ptr)) = des_output;

			return 1;//Return 1 if mode didn't fail.
		}
		//PinSettings getPinSettings(int pinNumber, DigitalOut outValue = DigitalOut::LOW) { return PinSettings(); }
	};
}

int main(){
	// starting GPIO session
	sp::MyGPIO gpio; // enables the constructor
	int pin_num = 27; // pin to control

	std::cout << "Toggling Pin Mode ...";
	gpio.pinMode(pin_num, sp::GPIO::PinModes::INPUT);
	gpio.pinMode(pin_num, sp::GPIO::PinModes::OUTPUT);// set the pin mode
	std::cout << "toggle complete" << std::endl;


	std::cout << "Writing Low : " << std::endl;
	gpio.digitalWrite(pin_num, sp::GPIO::DigitalOut::LOW); // set the pin to low
	int pin_val = gpio.digitalRead(pin_num); // read the value of the pin
	std::cout << "Value of pin " << pin_num << " is " << pin_val << std::endl;


	std::cout << "Writing High : " << std::endl;
	gpio.digitalWrite(pin_num, sp::GPIO::DigitalOut::HIGH); //write high to pin 27
	pin_val = gpio.digitalRead(pin_num);// read the pin
	std::cout << "Value of pin " << pin_num << " is " << pin_val << std::endl;

	return 0;
}