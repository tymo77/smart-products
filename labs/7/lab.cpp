#include "../../include/PanTilt.h"
#include <iostream>

int main() {
	sp::PanTilt ptilt;
	std::cout << ptilt.get_angle(1) << std::endl;
	std::cout << ptilt.get_angle(2) << std::endl;
	return 0;
}
