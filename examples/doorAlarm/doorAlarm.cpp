#include <stdio.h>
#include <iostream>
#include <unistd.h>

#include "ip_connection.h"
#include "bricklet_distance_us.h"
#include "bricklet_piezo_speaker.h"

#define HOST "192.168.178.52"
#define PORT 4223
#define UID_DISTANCE "zqf" // Change XYZ to the UID of your Distance US Bricklet
#define UID_SPEAKER "C84"

int main(void) {
    // Create IP connection
    IPConnection ipcon;
    ipcon_create(&ipcon);

    // Create device object
    DistanceUS dus;
    distance_us_create(&dus, UID_DISTANCE, &ipcon);

    PiezoSpeaker ps;
    piezo_speaker_create(&ps, UID_SPEAKER, &ipcon);

    // Connect to brickd
    if(ipcon_connect(&ipcon, HOST, PORT) < 0) {
        fprintf(stderr, "Could not connect\n");
        return 1;
    }
    // Don't use device before ipcon is connected

    while(true){
    	// Get current distance value
    	uint16_t distance;
    	if(distance_us_get_distance_value(&dus, &distance) < 0) {
        	fprintf(stderr, "Could not get distance value, probably timeout\n");
        	return 1;
    	}
    
    	printf("Distance Value: %u\n", distance);
	    
	if(distance < 500 ){
		piezo_speaker_morse_code(&ps, ". - . - . -", 2000);
	}

	    sleep(1);
    }




    printf("Press key to exit\n");
    getchar();
    distance_us_destroy(&dus);
    ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
    return 0;
}
