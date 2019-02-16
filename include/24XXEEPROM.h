#ifndef SMART_PRODUCTS_24XXEEPROM_H
#define SMART_PRODUCTS_24XXEEPROM_H

namespace sp {

	class MC24XX{

	private:
		static uint8_t CNTRL_W_BYTE;
		static uint8_t CNTRL_R_BYTE;
		static int MEM_SIZE;
		static int DEF_ADDR;
		
	public:
		uint8_t castAddress(int);
		uint8_t castData(int);
		void write(int, int);
		int read(int);
		MC24XX();

	};

}

#endif