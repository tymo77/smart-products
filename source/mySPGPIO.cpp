#include "../include/mySPGPIO.h"

namespace sp {
	uint32_t MyGPIO::readRegBits(void* ptr) {
		return *(reinterpret_cast<std::uint32_t *>(ptr));
	}

	int MyGPIO::setRegBits(void* ptr, uint32_t bits) {
		*(reinterpret_cast<std::uint32_t *>(ptr)) = bits;
		return 1;
	}

	GPIO::GPIOregisters MyGPIO::getPinModeReg(int pin_number) {
		GPIOregisters pin_reg;
		switch (pin_number / 10) {
		case 0:
			pin_reg = GPIOregisters::SR0;
			break;
		case 1:
			pin_reg = GPIOregisters::SR1;
			break;
		case 2:
			pin_reg = GPIOregisters::SR2;
			break;
		case 3:
			pin_reg = GPIOregisters::SR3;
			break;
		case 4:
			pin_reg = GPIOregisters::SR4;
			break;
		case 5:
			pin_reg = GPIOregisters::SR4;
			break;
		default:
			throw std::runtime_error("getPinModeReg(): pin # out of range");
		}
		return pin_reg;
	}

	GPIO::GPIOregisters MyGPIO::getPinReadReg(int pin_number) {
		GPIOregisters pin_reg;

		switch (pin_number / 32) {
		case 0:
			pin_reg = GPIOregisters::Read0;
			break;
		case 1:
			pin_reg = GPIOregisters::Read1;
			break;
		default:
			throw std::runtime_error("getPinReadReg(): pin # out of range");
		}

		return pin_reg;
	}

	GPIO::GPIOregisters MyGPIO::getPinHighReg(int pin_number) {
		GPIOregisters pin_reg;
		switch (pin_number / 32) {
		case 0:
			pin_reg = GPIOregisters::WriteHigh0;
			break;
		case 1:
			pin_reg = GPIOregisters::WriteHigh1;
		default:
			throw std::runtime_error("getPinHighReg(): pin # out of range");
		}
		return pin_reg;
	}

	GPIO::GPIOregisters MyGPIO::getPinLowReg(int pin_number) {
		GPIOregisters pin_reg;
		switch (pin_number / 32) {
		case 0:
			pin_reg = GPIOregisters::WriteLow0;
			break;
		case 1:
			pin_reg = GPIOregisters::WriteLow1;
			break;
		default:
			throw std::runtime_error("getPinLowReg(): pin # out of range");
		}
		return pin_reg;
	}

	int MyGPIO::readPinMode(int pin_number) {
		void* pin_ptr = getPtr(getPinModeReg(pin_number));
		uint32_t pinmode_now = readRegBits(pin_ptr);
		int pin_number_shift = (pin_number % 10) * 3;
		pinmode_now = (pinmode_now >> pin_number_shift) & (0x7);

		return static_cast<int>(pinmode_now);
	}

	int MyGPIO::pinMode(int pin_number, PinModes p_mode) {
		// pin_number -> pin we want to set.
		// p_mode -> the mode we want to set this pin to.
		// sets pin_number to p_mode

		// Get pointer to memory register.
		void* pin_ptr = getPtr(getPinModeReg(pin_number));

		// Get current state of target register.
		uint32_t pinmode_now = readRegBits(pin_ptr);
		//std::cout << "initial pins  :" << std::bitset<32>(pinmode_now) << std::endl;

		// Get pinmode code to set target bits in target register.
		uint32_t p_mode_code = static_cast<std::uint32_t>(p_mode);

		// Shift the target pin over to the correct bits in target register.
		int pin_number_shift = (pin_number % 10) * 3;
		uint32_t des_output = p_mode_code << pin_number_shift;

		// Mask 000's over the target bits in target register.
		uint32_t mask = (~(0x7 << pin_number_shift));
		//std::cout << "desired change:" << std::bitset<32>(des_output) << std::endl;
		//std::cout << "bit mask:      " << std::bitset<32>(mask) << std::endl;

		// Get new state for the target register.
		des_output = (pinmode_now & mask) | (des_output & ~mask);
		//std::cout << "output:        " << std::bitset<32>(des_output) << std::endl;

		// Set target register.
		setRegBits(pin_ptr, des_output);
		//std::cout << "after :        " << std::bitset<32>(readRegBits(pin_ptr)) << std::endl;

		return 1;//Return 1 if mode didn't fail.
	}

	int MyGPIO::digitalRead(int pin_number) {

		// Get pointer to memory register.
		void* pin_ptr = getPtr(getPinReadReg(pin_number));

		// Get current state of target register.
		uint32_t pinmode_now = readRegBits(pin_ptr);

		// Mask "...010..." over the target bit in target register.
		int pin_number_shift = pin_number % 32;
		uint32_t mask = (0x1 << pin_number_shift);
		uint32_t result = (mask & pinmode_now) >> pin_number_shift;

		return static_cast<int>(result);
	}

	int MyGPIO::digitalWrite(int pin_number, DigitalOut out_value) {

		// Get pointer to memory register.
		void* pin_ptr;
		if (out_value == DigitalOut::HIGH) {
			pin_ptr = getPtr(getPinHighReg(pin_number));
		}
		else if (out_value == DigitalOut::LOW) {
			pin_ptr = getPtr(getPinLowReg(pin_number));
		}
		else {
			throw std::runtime_error("digitalWrite(): bad DigitalOut");
		}

		// Get current state of target register.
		uint32_t pinmode_now = readRegBits(pin_ptr);
		//std::cout << "initial pins  :" << std::bitset<32>(pinmode_now) << std::endl;

		// Shift the target pin over to the correct bits in target register.
		int pin_number_shift = (pin_number % 32);
		uint32_t des_output = (0x1 << pin_number_shift);

		// Mask "...11011..." over the target bit in target register.
		uint32_t mask = (~(0x1 << pin_number_shift));
		//std::cout << "desired change:" << std::bitset<32>(des_output) << std::endl;
		//std::cout << "bit mask:      " << std::bitset<32>(mask) << std::endl;
		// Get new state for the target register.
		des_output = (pinmode_now & mask) | (des_output & ~mask);
		//std::cout << "output:        " << std::bitset<32>(des_output) << std::endl;
		// Set target register.
		setRegBits(pin_ptr, des_output);

		//std::cout << "after :        " << std::bitset<32>(readRegBits(pin_ptr)) << std::endl;
		return 1;//Return 1 if mode didn't fail.
	}

	GPIO::PinSettings MyGPIO::getPinSettings(int pin_number) {

		GPIO::PinSettings pinSet;

		pinSet.PinLevel = digitalRead(pin_number);
		pinSet.PinMode = readPinMode(pin_number);
		pinSet.ModePtr = getPtr(getPinModeReg(pin_number));
		pinSet.WrPtrH = getPtr(getPinHighReg(pin_number));
		pinSet.WrPtrL = getPtr(getPinLowReg(pin_number));
		pinSet.RdPtr = getPtr(getPinReadReg(pin_number));
		return pinSet;
	}

	GPIO::PinSettings MyGPIO::updatePinSettings(int pin_number) {

		GPIO::PinSettings pinSet;

		pinSet.PinLevel = digitalRead(pin_number);
		pinSet.PinMode = readPinMode(pin_number);
		return pinSet;
	}

	void MyGPIO::showPins() {


		using namespace std;
		cout << setbase(10);
		cout << '\n' << "RPi GPIO STATES\nID:  ";
		for (int i = 0; i <= 27; i++) {
			cout << setw(4) << setfill(' ') << i;
		}
		cout << "\nMODE:";

		for (int i = 0; i <= 27; i++) {
			cout << setw(4) << setfill(' ') << readPinMode(i);
		}

		cout << "\nLVL: ";
		for (int i = 0; i <= 27; i++) {
			cout << setw(4) << setfill(' ') << digitalRead(i);
		}
		cout << endl;
	}

	void MyGPIO::showPins(const std::vector<int> &pins) {

		using namespace std;
		cout << setbase(10);
		cout << '\n' << "RPi GPIO STATES\nID:  ";
		for (auto elem : pins) {
			cout << setw(4) << setfill(' ') << elem;
		}
		cout << "\nMODE:";

		for (auto elem : pins) {
			cout << setw(4) << setfill(' ') << readPinMode(elem);
		}

		cout << "\nLVL: ";
		for (auto elem : pins) {
			cout << setw(4) << setfill(' ') << digitalRead(elem);
		}
		cout << endl;
	}

	//void showRegister
}
