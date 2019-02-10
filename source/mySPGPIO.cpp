#include "../include/mySPGPIO.h"

namespace sp {
	
	int MyGPIO::FUNC_SEL_SHIFT[] = {
		0,3,6,9,12,15,18,21,24,27,
		0,3,6,9,12,15,18,21,24,27,
		0,3,6,9,12,15,18,21,24,27,
		0,3,6,9,12,15,18,21,24,27,
		0,3,6,9,12,15,18,21,24,27,
		0,3,6,9,12,15,18,21,24,27
	};
		
	int MyGPIO::RW_SHIFT[] = {
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
	};
		
	GPIO::GPIOregisters MyGPIO::FUNC_SEL_REG[] = {
		GPIOregisters::SR0, GPIOregisters::SR0, GPIOregisters::SR0, GPIOregisters::SR0, GPIOregisters::SR0, GPIOregisters::SR0, GPIOregisters::SR0, GPIOregisters::SR0, GPIOregisters::SR0, GPIOregisters::SR0,
		GPIOregisters::SR1, GPIOregisters::SR1, GPIOregisters::SR1, GPIOregisters::SR1, GPIOregisters::SR1, GPIOregisters::SR1, GPIOregisters::SR1, GPIOregisters::SR1, GPIOregisters::SR1, GPIOregisters::SR1,
		GPIOregisters::SR2, GPIOregisters::SR2, GPIOregisters::SR2, GPIOregisters::SR2, GPIOregisters::SR2, GPIOregisters::SR2, GPIOregisters::SR2, GPIOregisters::SR2, GPIOregisters::SR2, GPIOregisters::SR2,
		GPIOregisters::SR3, GPIOregisters::SR3, GPIOregisters::SR3, GPIOregisters::SR3, GPIOregisters::SR3, GPIOregisters::SR3, GPIOregisters::SR3, GPIOregisters::SR3, GPIOregisters::SR3, GPIOregisters::SR3,
		GPIOregisters::SR4, GPIOregisters::SR4, GPIOregisters::SR4, GPIOregisters::SR4, GPIOregisters::SR4, GPIOregisters::SR4, GPIOregisters::SR4, GPIOregisters::SR4, GPIOregisters::SR4, GPIOregisters::SR4,
		GPIOregisters::SR5, GPIOregisters::SR5, GPIOregisters::SR5, GPIOregisters::SR5, GPIOregisters::SR5, GPIOregisters::SR5, GPIOregisters::SR5, GPIOregisters::SR5, GPIOregisters::SR5, GPIOregisters::SR5
	};
	
	GPIO::GPIOregisters MyGPIO::R_REG[] = {
		GPIOregisters::Read0, GPIOregisters::Read0, GPIOregisters::Read0, GPIOregisters::Read0, GPIOregisters::Read0, GPIOregisters::Read0, GPIOregisters::Read0, GPIOregisters::Read0,
		GPIOregisters::Read0, GPIOregisters::Read0, GPIOregisters::Read0, GPIOregisters::Read0, GPIOregisters::Read0, GPIOregisters::Read0, GPIOregisters::Read0, GPIOregisters::Read0,
		GPIOregisters::Read0, GPIOregisters::Read0, GPIOregisters::Read0, GPIOregisters::Read0, GPIOregisters::Read0, GPIOregisters::Read0, GPIOregisters::Read0, GPIOregisters::Read0,
		GPIOregisters::Read0, GPIOregisters::Read0, GPIOregisters::Read0, GPIOregisters::Read0, GPIOregisters::Read0, GPIOregisters::Read0, GPIOregisters::Read0, GPIOregisters::Read0,
		GPIOregisters::Read1, GPIOregisters::Read1, GPIOregisters::Read1, GPIOregisters::Read1, GPIOregisters::Read1, GPIOregisters::Read1, GPIOregisters::Read1, GPIOregisters::Read1,
		GPIOregisters::Read1, GPIOregisters::Read1, GPIOregisters::Read1, GPIOregisters::Read1, GPIOregisters::Read1, GPIOregisters::Read1, GPIOregisters::Read1, GPIOregisters::Read1,
		GPIOregisters::Read1, GPIOregisters::Read1, GPIOregisters::Read1, GPIOregisters::Read1, GPIOregisters::Read1, GPIOregisters::Read1, GPIOregisters::Read1, GPIOregisters::Read1,
		GPIOregisters::Read1, GPIOregisters::Read1, GPIOregisters::Read1, GPIOregisters::Read1, GPIOregisters::Read1, GPIOregisters::Read1, GPIOregisters::Read1, GPIOregisters::Read1
	};
	
	GPIO::GPIOregisters MyGPIO::W_H_REG[] = {
		GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0,
		GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0,
		GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0,
		GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0, GPIOregisters::WriteHigh0,
		GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1,
		GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1,
		GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1,
		GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1, GPIOregisters::WriteHigh1
	};
	
	GPIO::GPIOregisters MyGPIO::W_L_REG[] = {
		GPIOregisters::WriteLow0, GPIOregisters::WriteLow0, GPIOregisters::WriteLow0, GPIOregisters::WriteLow0, GPIOregisters::WriteLow0, GPIOregisters::WriteLow0, GPIOregisters::WriteLow0, GPIOregisters::WriteLow0,
		GPIOregisters::WriteLow0, GPIOregisters::WriteLow0, GPIOregisters::WriteLow0, GPIOregisters::WriteLow0, GPIOregisters::WriteLow0, GPIOregisters::WriteLow0, GPIOregisters::WriteLow0, GPIOregisters::WriteLow0,
		GPIOregisters::WriteLow0, GPIOregisters::WriteLow0, GPIOregisters::WriteLow0, GPIOregisters::WriteLow0, GPIOregisters::WriteLow0, GPIOregisters::WriteLow0, GPIOregisters::WriteLow0, GPIOregisters::WriteLow0,
		GPIOregisters::WriteLow0, GPIOregisters::WriteLow0, GPIOregisters::WriteLow0, GPIOregisters::WriteLow0, GPIOregisters::WriteLow0, GPIOregisters::WriteLow0, GPIOregisters::WriteLow0, GPIOregisters::WriteLow0,
		GPIOregisters::WriteLow1, GPIOregisters::WriteLow1, GPIOregisters::WriteLow1, GPIOregisters::WriteLow1, GPIOregisters::WriteLow1, GPIOregisters::WriteLow1, GPIOregisters::WriteLow1, GPIOregisters::WriteLow1,
		GPIOregisters::WriteLow1, GPIOregisters::WriteLow1, GPIOregisters::WriteLow1, GPIOregisters::WriteLow1, GPIOregisters::WriteLow1, GPIOregisters::WriteLow1, GPIOregisters::WriteLow1, GPIOregisters::WriteLow1,
		GPIOregisters::WriteLow1, GPIOregisters::WriteLow1, GPIOregisters::WriteLow1, GPIOregisters::WriteLow1, GPIOregisters::WriteLow1, GPIOregisters::WriteLow1, GPIOregisters::WriteLow1, GPIOregisters::WriteLow1,
		GPIOregisters::WriteLow1, GPIOregisters::WriteLow1, GPIOregisters::WriteLow1, GPIOregisters::WriteLow1, GPIOregisters::WriteLow1, GPIOregisters::WriteLow1, GPIOregisters::WriteLow1, GPIOregisters::WriteLow1
	};
	
	
	uint32_t MyGPIO::readRegBits(void* ptr) {
		return *(reinterpret_cast<std::uint32_t *>(ptr));
	}

	int MyGPIO::setRegBits(void* ptr, uint32_t bits) {
		*(reinterpret_cast<std::uint32_t *>(ptr)) = bits;
		return 1;
	}

	//~ GPIO::GPIOregisters MyGPIO::getPinModeReg(int pin_number) {
		//~ GPIOregisters pin_reg;
		//~ switch (pin_number / 10) {
		//~ case 0:
			//~ pin_reg = GPIOregisters::SR0;
			//~ break;
		//~ case 1:
			//~ pin_reg = GPIOregisters::SR1;
			//~ break;
		//~ case 2:
			//~ pin_reg = GPIOregisters::SR2;
			//~ break;
		//~ case 3:
			//~ pin_reg = GPIOregisters::SR3;
			//~ break;
		//~ case 4:
			//~ pin_reg = GPIOregisters::SR4;
			//~ break;
		//~ case 5:
			//~ pin_reg = GPIOregisters::SR5;
			//~ break;
		//~ default:
			//~ throw std::runtime_error("getPinModeReg(): pin # out of range");
		//~ }
		//~ return pin_reg;
	//~ }

	//~ GPIO::GPIOregisters MyGPIO::getPinReadReg(int pin_number) {
		//~ GPIOregisters pin_reg;

		//~ switch (pin_number / 32) {
		//~ case 0:
			//~ pin_reg = GPIOregisters::Read0;
			//~ break;
		//~ case 1:
			//~ pin_reg = GPIOregisters::Read1;
			//~ break;
		//~ default:
			//~ throw std::runtime_error("getPinReadReg(): pin # out of range");
		//~ }

		//~ return pin_reg;
	//~ }

	//~ GPIO::GPIOregisters MyGPIO::getPinHighReg(int pin_number) {
		//~ GPIOregisters pin_reg;
		//~ switch (pin_number / 32) {
		//~ case 0:
			//~ pin_reg = GPIOregisters::WriteHigh0;
			//~ break;
		//~ case 1:
			//~ pin_reg = GPIOregisters::WriteHigh1;
		//~ default:
			//~ throw std::runtime_error("getPinHighReg(): pin # out of range");
		//~ }
		//~ return pin_reg;
	//~ }

	//~ GPIO::GPIOregisters MyGPIO::getPinLowReg(int pin_number) {
		//~ GPIOregisters pin_reg;
		//~ switch (pin_number / 32) {
		//~ case 0:
			//~ pin_reg = GPIOregisters::WriteLow0;
			//~ break;
		//~ case 1:
			//~ pin_reg = GPIOregisters::WriteLow1;
			//~ break;
		//~ default:
			//~ throw std::runtime_error("getPinLowReg(): pin # out of range");
		//~ }
		//~ return pin_reg;
	//~ }

	int MyGPIO::readPinMode(int pin_number) {
		if (pin_number < 0 || pin_number > 54){
			std::cout << "readPinMode() failed. Bad pin\n";
			return -1;			
		}
		void* pin_ptr = getPtr(FUNC_SEL_REG[pin_number]);
		uint32_t pinmode_now = readRegBits(pin_ptr);
		pinmode_now = (pinmode_now >> FUNC_SEL_SHIFT[pin_number]) & (0x7);
		return static_cast<int>(pinmode_now);
	}

	int MyGPIO::pinMode(int pin_number, PinModes p_mode) {
		
		if (pin_number < 0 || pin_number > 54){
			std::cout << "pinMode() failed. Bad pin\n";
			return -1;			
		}

		// Get pointer to memory register.
		void* pin_ptr = getPtr(FUNC_SEL_REG[pin_number]);

		// Get current state of target register.
		uint32_t pinmode_now = readRegBits(pin_ptr);

		// Get pinmode code to set target bits in target register.
		uint32_t p_mode_code = static_cast<std::uint32_t>(p_mode);

		// Shift the target pin over to the correct bits in target register.
		int pin_number_shift = FUNC_SEL_SHIFT[pin_number];
		uint32_t des_output = p_mode_code << pin_number_shift;

		// Mask 000's over the target bits in target register.
		uint32_t mask = (~(0x7 << pin_number_shift));

		// Get new state for the target register.
		des_output = (pinmode_now & mask) | (des_output & ~mask);

		// Set target register.
		setRegBits(pin_ptr, des_output);

		return 1;//Return 1 if mode didn't fail.
	}

	int MyGPIO::digitalRead(int pin_number) {
		
		if (pin_number < 0 || pin_number > 54){
			std::cout << "digitalRead() failed. Bad pin\n";
			return -1;			
		}

		// Get pointer to memory register.
		void* pin_ptr = getPtr(R_REG[pin_number]);

		// Get current state of target register.
		uint32_t pinmode_now = readRegBits(pin_ptr);

		// Mask "...010..." over the target bit in target register.
		int pin_number_shift = RW_SHIFT[pin_number];
		uint32_t mask = (0x1 << pin_number_shift);
		uint32_t result = (mask & pinmode_now) >> pin_number_shift;

		return static_cast<int>(result);
	}

	int MyGPIO::digitalWrite(int pin_number, DigitalOut out_value) {
		
		if (pin_number < 0 || pin_number > 54 || readPinMode(pin_number) != 1){
			std::cout << "digitalWrite() failed. Bad or non-writable pin\n";
			return -1;			
		}

		// Get pointer to memory register.
		void* pin_ptr;
		if (out_value == DigitalOut::HIGH) {
			pin_ptr = getPtr(W_H_REG[pin_number]);
		}
		else if (out_value == DigitalOut::LOW) {
			pin_ptr = getPtr(W_L_REG[pin_number]);
		}
		else {
			throw std::runtime_error("digitalWrite(): bad DigitalOut");
		}

		// Get current state of target register.
		uint32_t pinmode_now = readRegBits(pin_ptr);

		// Shift the target pin over to the correct bits in target register.
		uint32_t des_output = (0x1 << RW_SHIFT[pin_number]);

		// Set target register.
		setRegBits(pin_ptr, des_output);

		return 1;//Return 1 if mode didn't fail.
	}

	GPIO::PinSettings MyGPIO::getPinSettings(int pin_number) {

		GPIO::PinSettings pinSet;

		pinSet.PinLevel = digitalRead(pin_number);
		pinSet.PinMode = readPinMode(pin_number);
		pinSet.ModePtr = getPtr(FUNC_SEL_REG[pin_number]);
		pinSet.WrPtrH = getPtr(W_H_REG[pin_number]);
		pinSet.WrPtrL = getPtr(W_L_REG[pin_number]);
		pinSet.RdPtr = getPtr(R_REG[pin_number]);
		return pinSet;
	}

	void MyGPIO::updatePinSettings(int pin_number, GPIO::PinSettings& pinSet) {

		pinSet.PinLevel = digitalRead(pin_number);
		pinSet.PinMode = readPinMode(pin_number);

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
}
