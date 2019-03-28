#include "../../include/I2C.h"
#include <iostream> 	// For printing to console.
#include <chrono>		// For measuring elapsed time.

#define TIMEOUT 10.0
#define ARDUINO1_ADDRESS 4;
int main(int argc, char* argv[]) {
	
	// Setup i2c device for Arduino.
	sp::I2C arduino1(ARDUINO1_ADDRESS);
	
	
	// Get start time for measuring time elapsed and timing out.
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	while(true){
		
		// Ask if ready to move.
		
		
		// If ready to move, ask position.
		
		
		
		// Check position against list.
		
		
		// Report if there is any conflict "Y/N"
		
		
		
		// End program after TIMEOUT seconds.
		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(now - begin);
		if (time_span.count() > TIMEOUT){
			std::cout << "Program ended after timeout: " << time_span.count() << " seconds." << std::endl;
			return 0;
		}
	}
}
