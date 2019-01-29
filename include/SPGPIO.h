#ifndef SMART_PRODUCTS_GPIO_H
#define SMART_PRODUCTS_GPIO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/mman.h>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <exception>
#include <string>
#include <iostream>


namespace sp {

	// Abstract Class to inherit
	class GPIO
	{
	private:
		void* mMemPtr;// dynamic pointer to the start of the memory space
		int mMemFD; // file driver object, to open and close the gpio memory

	public:
		
		// All the different PinModes that can be set for the GPIO 
		enum class PinModes : std::uint32_t {
			INPUT = 0x0,
			OUTPUT = 0x1,
			ALT0 = 0x4,
			ALT1 = 0x5,
			ALT2 = 0x6,
			ALT3 = 0x7,
			ALT4 = 0x3,
			ALT5 = 0x2
		};

		// Options to write high or low to the GPIO pins
		enum class DigitalOut : std::uint32_t {
			HIGH = 1,
			LOW = 0
		};
		/*
			The offsets from GPIO Base for different memory registers needed
			to:
				change the pin modes (SRx),
				read the digital state of a pin (Readx),
				change the digital output of a pin to 1 (WriteHighx),
				change the digital output of a pin to 0 (WriteLowx)
		*/
		enum class GPIOregisters : std::uint32_t {
			SR0 = 0x0000,
			SR1 = 0x0004,
			SR2 = 0x0008,
			SR3 = 0x000C,
			SR4 = 0x0010,
			SR5 = 0x0014,
			Read0 = 0x0034,
			Read1 = 0x0038,
			WriteHigh0 = 0x001C,
			WriteHigh1 = 0x0020,
			WriteLow0 = 0x0028,
			WriteLow1 = 0x002C
		};

		/*
			Default settings that are needed to work with the memory registers
			with RPi
		*/
		struct RPiSettings {
			std::uint32_t MemBasePeriph = 0x3F000000;//beginning of the peripheral memory registers (AKA Base)
			std::uint32_t MemOffsetGPIO = 0x200000;//offset from Base to reach beginning of GPIO memory registers
			std::uint32_t MemMapSize = 4096UL;//size of the mapped memory space (1 page)
			std::string MemPath = "/dev/mem";//root location of the memory device driver (USE THIS ONE)
			std::string GPIOPath = "/dev/gpiomem";//root location of the GPIO memory device driver
		} mRPiSettings;

		struct PinSettings {
			/*
				the position of the first bit in the memory register
				which corresponds to the value of the pin mode for
				the given pin number 
				- (ranges from 0 to 31) -
			*/			
			int PinPositionPM;
			/*
			the position of the first bit in the memory register
			which corresponds to the state (HIGH OR LOW) of a given pin 
			which will be either written to	or read from
			(ranges from 0 to 31)
			*/
			int PinPositionRW;
			/*
				Pointer to the pin mode register
				use  reinterpret_cast<std::uint32_t *>(PtrPinMode) later on to read and write
			*/
			void* PtrPinMode;
			/*
				Pointer to the digital write register
				use  reinterpret_cast<std::uint32_t *>(PtrWrite) later on to read and write
			*/
			void* PtrWrite;
			/*
				Pointer to the digital read register
				use  reinterpret_cast<std::uint32_t *>(PtrRead) later on to read
			*/
			void* PtrRead;
		};

		// these virtual functions must be overwritten
		virtual int pinMode(int, PinModes) = 0;
		virtual int digitalRead(int) = 0;
		virtual int digitalWrite(int, DigitalOut) = 0;
		virtual PinSettings getPinSettings(int pinNumber, DigitalOut outValue = DigitalOut::LOW) { return PinSettings(); }
		
		void* getPtr()
		{
			return mMemPtr;
		}

		void* getPtr(GPIOregisters gpio_register)
		{
			return (mMemPtr +  static_cast<off_t>(gpio_register));
		}

		void setPtr(void *void_ptr) 
		{
			mMemPtr = void_ptr;
		}

		GPIO()
		{
			off_t gpio_base_address = static_cast<off_t>(mRPiSettings.MemBasePeriph + mRPiSettings.MemOffsetGPIO);// base address for GPIO memory
			mMemFD = open(mRPiSettings.MemPath.c_str(), O_RDWR | O_SYNC);// open driver
			if (mMemFD<0) throw std::runtime_error("Cannot open memory device: try using sudo to run your program");
			setPtr(mmap(NULL, mRPiSettings.MemMapSize, (PROT_READ | PROT_WRITE), MAP_SHARED, mMemFD, gpio_base_address));//memory is mapped and a pointer is returned
		}

		~GPIO()
		{
			munmap(getPtr(), mRPiSettings.MemMapSize);
			close(mMemFD);
		}
	};

}
#endif
