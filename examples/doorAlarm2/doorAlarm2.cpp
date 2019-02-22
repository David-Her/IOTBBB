/***************************************************************************/
// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#define IPCON_EXPOSE_MILLISLEEP

//#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include "ip_connection.h"
#include "brick_master.h"
#include "bricklet_industrial_quad_relay.h"
#include "bricklet_distance_us.h"

#define HOST "localhost"
#define PORT 4223
#define UIDmaster "6JpovQ" // Change XXYYZZ to the UID of your Master Brick
#define UIDrelay "BvA" // Change XYZ to the UID of your Industrial Quad Relay Bricklet
#define UIDdistance "zqf"

// To reboot the BB
#include <linux/reboot.h>
#include <sys/reboot.h>

int main()
{
	// Create IP connection
	IPConnection ipcon;
	ipcon_create(&ipcon);

	// Create device object
	IndustrialQuadRelay iqr;
	industrial_quad_relay_create(&iqr, UIDrelay, &ipcon);

	DistanceUS dus;
	distance_us_create(&dus, UIDdistance, &ipcon);

	// Connect to brickd
	if (ipcon_connect(&ipcon, HOST, PORT) < 0) {
		fprintf(stderr, "Could not connect\n");
		return 1;
	}
	// Don't use device before ipcon is connected

	/*********************************************************************************************************/

	bool led_on = FALSE;

	while (true) {
		// Get current distance value
		uint16_t distance;


		if (distance_us_get_distance_value(&dus, &distance) < 0) {
			printf("NO Distance Value: %u\n", distance);
			//sync();
			// reboot(LINUX_REBOOT_CMD_RESTART);
			return 1;
		}
		else {
			printf("Distance Value: %u\n", distance);

			if (led_on) {

				led_on = !led_on;
				printf(" => Led_on: %u\n", distance);
				industrial_quad_relay_set_value(&iqr, 1 << 1);
				millisleep(100);
				industrial_quad_relay_set_value(&iqr, 0 << 1);
			}
			else
			{
				led_on = !led_on;
//				printf("Led_off: %u\n", distance);
//				industrial_quad_relay_set_value(&iqr, 1 << 1);
			}

			if ((distance < 800) || (distance > 1200)) {
				printf("Alguien entro: %u\n", distance);
				int i, j;
				for (j = 0; j < 2; ++j) {
					for (i = 0; i < 2; ++i) {
						industrial_quad_relay_set_value(&iqr, 1 << 0);
						millisleep(600);
						industrial_quad_relay_set_value(&iqr, 0 << 0);
						millisleep(300);
//						industrial_quad_relay_set_value(&iqr, 1 << 2);
//						millisleep(100);
//						industrial_quad_relay_set_value(&iqr, 1 << 3);
					}
				millisleep(2000); // 2 seconds
				}
			}

		}



		millisleep(500);
	}
	/**********************************************************************************************************/

/*
	// Turn relays alternating on/off 10 times with 100 ms delay
	

*/
	printf("Press key to exit\n");
	getchar();
	industrial_quad_relay_destroy(&iqr);
	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
	return 0;
}

/***************************************************************************/