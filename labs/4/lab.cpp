#include <iostream>
#include <iomanip>

#include <chrono>  // for high_resolution_clock

#include <stdlib.h>
#include <time.h>

#include "../../include/mySPGPIO.h"
#include "../../include/24XXEEPROM.h"
#include "../../include/CD405PLEX.h"

constexpr int PIN_A = 17;
constexpr int PIN_B = 27;

int main() {
	int val;
	// generate random integers
	int data[128];
	
	srand (time(NULL));
	for (int i = 0; i < 128; i++){
		data[i] = rand() % 256;// random numbers 0-256 (2^8)
	}
	
	//~ sp::MyGPIO gpio;
	// Record start time
	auto start = std::chrono::high_resolution_clock::now();
	
	// initialize objects
	sp::CD405	cd405(PIN_A, PIN_B);
	sp::MC24XX	mc24xx;
	
	std::cout << "Writing Tests" << std::endl;
	for( int i = 1; i <= 3; i++){
		
		std::cout << "Testing EEPROM " << i << std::endl;
		cd405.setPlex(i);
		
		for (int j = 0; j < 128; j++){
		
		// Write
		std::cout << "Writing " << std::setw(5) << data[j];
		std::cout << " to EEPROM " << std::setw(3) << i << " register " << j << std::endl;
		mc24xx.write(j, data[j]);
		}
	}
	
	auto w_finish = std::chrono::high_resolution_clock::now();
	
	std::cout << "Reading Tests" << std::endl;
	int err_ct = 0;
	for( int i = 1; i <= 3; i++){
		cd405.setPlex(i);
		
		for (int j = 0; j < 128; j++){
		
		// Read
		val = mc24xx.read(j);
		
		std::cout << i << ", " << std::setw(5) << j << ":";
		std::cout << std::setw(5) << data[j] << " -> " << val << std::endl;
		
		err_ct += (data[j] == val ? 0 : 1);
		
		}
	}
	
	// Record end time
	auto r_finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> w_elapsed = w_finish - start;
	std::chrono::duration<double> r_elapsed = r_finish - w_finish;
	
	double rate_w = 3*128*8 / w_elapsed.count() / 1000; // 3 EEPROMS, 128 registers, 8 bits
	double rate_r = 3*128*8 / r_elapsed.count() / 1000; // 3 EEPROMS, 128 registers, 8 bits
	
	std::cout << err_ct << " errors detected in r/w test" << std::endl;
	std::cout << "Elapsed time for writes: " << w_elapsed.count() << " s\n";
	std::cout << "Estimated write speed: " << rate_w << " kbps\n";
	std::cout << "Elapsed time for reads: " << r_elapsed.count() << " s\n";
	std::cout << "Estimated read speed: " << rate_r << " kbps\n";

	return 0;
}
