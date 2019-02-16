#ifndef SMART_PRODUCTS_CD405PLEX_H
#define SMART_PRODUCTS_CD405PLEX_H

namespace sp {

	class CD405 {

	private:
		static vector<vector<GPIO::PinModes>> CD405::PIN_TT;

	public:
		CD405(int, int);
		void setPlex(int);

	};
}

#endif