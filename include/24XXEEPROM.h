#ifndef SMART_PRODUCTS_24XXEEPROM_H
#define SMART_PRODUCTS_24XXEEPROM_H

namespace sp {

	class MC24XX{

	private:
		static int MEM_SIZE;
		static int DEF_ADDR;
		int fd;
		
	public:
		void write(int, int);
		int read(int);
		MC24XX();

	};

}

#endif
