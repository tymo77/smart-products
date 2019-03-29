/***
 * Midterm Exam - Smart Products
 * Pi as an arbitrator between agents. Minimum working example.
 * 
 * Tyler Morrison and Yuan Gao
 * March 2019
 * 
 * ME 5194 Spring 2019
 * 
 * Anyone can use this however they like.
***/

#include "../../include/I2C.h"	// For my i2c class. (My Library)
#include <iostream> 	// For printing to console.
#include <chrono>		// For measuring elapsed time, and giving durations.
#include <thread>		// For pausing execution to wait a while.
#include <vector>
#include <set>
#include <algorithm> // For "find"

#define TIMEOUT 10.0				// (seconds) The time this program will run until it quits (prevents infite loops).
#define ARDUINO1_ADDRESS 4		// The i2c address of the arduino. This should be set to match in the arduino code.
#define NORMAL_WAIT 1000		// (milliseconds) The time to wait before asking to move again after a successful communication.
#define NOTREADY_WAIT 500		// (milliseconds) The time to wait before asking to move again if the arduino was not ready.
#define DO_NOT_MOVE 0
#define OK_MOVE 1

int timeout;

std::vector<int> proposed_move = {0,0};	// This is the move that the agent is proposing.
std::vector<int> proposed_pos = {0,0};		// This is the position that the agent is proposing to move to.
std::vector<int> current_pos = {0,0};		// This is the current position of the agent.
char msg;						// This is the raw byte receievd from the agent.
bool valid_move = false;	// This is whether or not the move is OK.
bool valid_msg = false;		// This is whether or not the msg makes sense.


// This is the vector of positions that the agent is not allowed to go to.
// It is dynamic. Points can be added or removed during runtime.
std::set<std::vector<int>> set_bad_pos = {{1,0},{1,1},{0,3},{3,1},{3,2},{3,3},{3,4},{3,5}};
std::set<std::vector<int>>::iterator it;

int main(int argc, char* argv[]) {
	
	// Get custom timeout if wanted.
	if (argc == 2){
		timeout = std::stoi(argv[1]);
	}
	else {
		timeout = TIMEOUT;
	}
	
	// Add a boundary to the set of bad positions so it doesn't leave the domain.
	for(int i = -1; i <= 8; i++){
		set_bad_pos.insert({i,-1});	// Southern boundary.
		set_bad_pos.insert({i,8});		// Northern boundary.
	}
	for(int j = -1; j <= 8; j++){
		set_bad_pos.insert({-1,j});	// Western boundary.
		set_bad_pos.insert({8,j});		// Eastern boundary.
	}
	
	// Setup i2c device for Arduino.
	sp::I2C arduino(ARDUINO1_ADDRESS);
	arduino.set_read_delay(1); // One millsecond delay.
	
	int msg_int;
	// Get start time for measuring time elapsed and timing out.
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	while(true){
		
		// Ask if ready to move -- if so, what move.
		msg_int = arduino.read_8bit();
		msg = char(msg_int);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		
		if (msg == 'n'){
			
			// If the message is "not ready", wait a while.
			std::cerr << "Arduino not ready: " << msg << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(NOTREADY_WAIT));
		}
		else if (msg_int == -1){
			
			// If there is no response, wait a while.
			std::cerr << "Arduino not available: " << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(NOTREADY_WAIT));
			
		}
		else {// Everything that we do if we get a message is not -1 or 'not ready'
			// Ready to move, sent back a move candidate.
			switch(msg){
				case 'E': // Requests east.
					proposed_move = {1,0}; 
					valid_msg = true;
					break;
				case 'N': // Requests north.
					proposed_move = {0,1};
					valid_msg = true;
					break;
				case 'W': // Requests west.
					proposed_move = {-1,0};
					valid_msg = true;
					break;
				case 'S': // Requests south.
					proposed_move = {0,-1};
					valid_msg = true;
					break;
				case 'O': // Requests to stay.
					proposed_move = {0,0};
					valid_msg = true;
					break;
				default :
					std::cerr << "Cannot interpret response from Arduino: " << msg << std::endl;
					valid_msg = false;
					valid_move = false;
			}
			
			std::cout << "Received request: " << msg_int << " = " << msg << std::endl;
			
			// If the message was a valid candidate move, check it.
			if(valid_msg){
			
				// Check position against list.
				proposed_pos[0] = current_pos[0] + proposed_move[0];
				proposed_pos[1] = current_pos[1] + proposed_move[1];
				
				it = find(set_bad_pos.begin(),set_bad_pos.end(), proposed_pos);
				if(it != set_bad_pos.end()){
					// It is in the set of prohibited moves.
					valid_move = false;
				}
				else {
					// It is not in the set of prohibited moves.
					valid_move = true;
				}
			}
			
			// Respond OK if the move and message are good. Respond NO otherwise.
			if(valid_msg && valid_move){
				std::cout << "Telling the Arduino it CAN move" << std::endl;
				arduino.write_8bit(OK_MOVE);
				
				// Assume it moved.
				current_pos[0] = current_pos[0] + proposed_move[0];
				current_pos[1] = current_pos[1] + proposed_move[1];
			}
			else{
				std::cout << "Telling the Arduino it CANNOT move" << std::endl;
				arduino.write_8bit(DO_NOT_MOVE);
			}
			
			// Wait after successful communication.
			std::this_thread::sleep_for(std::chrono::milliseconds(NORMAL_WAIT));
		}
		
		// End program after TIMEOUT seconds.
		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(now - begin);
		if (time_span.count() > timeout){
			std::cout << "Program ended after timeout: " << time_span.count() << " seconds." << std::endl;
			return 0;
		}
	}
}
