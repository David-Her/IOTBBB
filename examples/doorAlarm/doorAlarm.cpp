#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <vector>

#include "ip_connection.h"
#include "bricklet_distance_us.h"
#include "bricklet_piezo_speaker.h"
#include "bricklet_oled_128x64.h"
#include "bricklet_multi_touch.h"

#define HOST "192.168.178.52"
#define PORT 4223
#define UID_DISTANCE "zqf" // Change XYZ to the UID of your Distance US Bricklet
#define UID_SPEAKER "C84"
#define UID_OLED "Bj4"
#define UID_KEYPAD "zCq"



 OLED128x64 oled;
 PiezoSpeaker ps;
 MultiTouch mt;

 bool presed = false;
 uint16_t key = 0;

 std::vector<int> pass;

// Callback function for touch state callback
void cb_touch_state(uint16_t state, void *user_data) {
	(void)user_data; // avoid unused parameter warning

	if(state & (1 << 12)) {
		printf("In proximity, ");
	}

	if((state & 0xfff) == 0) {
		printf("No electrodes touched\n");
		presed = false;
	} else {
		printf("Electrodes ");
		int i;
		for(i = 0; i < 12; i++) {
			if(state & (1 << i)) {
				printf("%d ", i);
				presed = true;
				key = i;
					
			}
		}
		printf("touched\n");
		
		// Fill the pass vector
		if((int)pass.size()>4){
			pass.erase(pass.begin());
		}
		pass.push_back(key);
		std::cout << "pass: " << pass[0] << " " << pass[1] << " " << pass[2] << " " << pass[3] << std::endl;
	}
}

bool  enterPasword(){
	
	std::cout << "In enterPassword \n ";

	oled_128x64_write_line(&oled, 5, 3, "Insert password:");
	std::stringstream ss;
	for(int i = 0; i < (int)pass.size(); i++){
		ss << pass[i] << " ";
	}
	std::string a;	
	a = ss.str();
	oled_128x64_write_line(&oled, 11, 5, (char*) a.c_str());
	
	if((int)pass.size() == 4){
		std::cout << "password correct! \n";
		return true;
	}

return false;
}

int main(void) {

 time_t now2 = time(NULL);
 char date[80];
// strftime(date, 80, "%Y-%m-%d_%H-%M-%S", localtime(&now2));
 strftime(date, 80, "%H:%M:%S", localtime(&now2));
 std::cout <<"TIME => " <<  date << '\n';
 std::string date1 = date;

    // Create IP connection
    IPConnection ipcon;
    ipcon_create(&ipcon);

    // Create device object
    DistanceUS dus;
    distance_us_create(&dus, UID_DISTANCE, &ipcon);

    piezo_speaker_create(&ps, UID_SPEAKER, &ipcon);

    oled_128x64_create(&oled, UID_OLED, &ipcon);

    multi_touch_create(&mt, UID_KEYPAD, &ipcon);

    // Connect to brickd
    if(ipcon_connect(&ipcon, HOST, PORT) < 0) {
        fprintf(stderr, "Could not connect\n");
        return 1;
    }
    // Don't use device before ipcon is connected

    // Clear display
    oled_128x64_clear_display(&oled);

    // Write "Hello World" starting from upper left corner of the screen
    oled_128x64_write_line(&oled, 0, 3, " Hello David and Anna! ");
    oled_128x64_write_line(&oled, 3, 3, "Init. the system ...");

	multi_touch_register_callback(&mt,
	                              MULTI_TOUCH_CALLBACK_TOUCH_STATE,
	                              (void *)cb_touch_state,
	                              NULL);

   sleep(1);

    while(true){
    	// Get current distance value
    	uint16_t distance;
    	if(distance_us_get_distance_value(&dus, &distance) < 0) {
        	fprintf(stderr, "Could not get distance value, probably timeout\n");
        	return 1;
    	}

    	printf("Distance Value: %u\n", distance);

	if(distance < 500 ){
		// Clear display
		oled_128x64_clear_display(&oled);
		// Write "Hello World" starting from upper left corner of the screen
		oled_128x64_write_line(&oled, 0, 3, "The door have been open!");
        	piezo_speaker_morse_code(&ps, ". - . - . -", 2000);
		bool pasword = false;
		while(!pasword){
			pasword = enterPasword();
			if(pasword){
				oled_128x64_clear_display(&oled);
				oled_128x64_write_line(&oled, 0, 0, " Password Correct! ");
			}
			usleep(500);
		}
		
		distance_us_get_distance_value(&dus, &distance);
    		printf("Distance Value: %u\n", distance);
		while(distance < 501){
			std::cout << "Waiting to close the door to activate the alarm \n ";
			sleep(1);	
		}
	}

	    sleep(1);
    }




    printf("Press key to exit\n");
    getchar();
    distance_us_destroy(&dus);
    ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
    return 0;
}
