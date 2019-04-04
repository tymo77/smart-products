#include "../../include/PanTilt.h"
#include "../../include/LidarLite.h"

#include <iostream>

int main() {
	sp::PanTilt ptilt;
	sp::LidarLite lidar;

	std::cout << ptilt.get_angle(1) << std::endl;
	std::cout << ptilt.get_angle(2) << std::endl;
	return 0;
}
