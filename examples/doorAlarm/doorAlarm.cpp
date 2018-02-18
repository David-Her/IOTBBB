#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <time.h>

// To reboot the BB
#include <linux/reboot.h>

#include <curl/curl.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/forwards.h>

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

 time_t start;

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
				if(i == 0){key = 3;}
				else if(i == 1){key = 2;}
				else if(i == 2){key = 1;}
				else if(i == 3){key = 6;}
				else if(i == 4){key = 5;}
				else if(i == 5){key = 4;}
				else if(i == 6){key = 9;}
				else if(i == 7){key = 8;}
				else if(i == 8){key = 7;}
				else if(i == 9){key = 12;}
				else if(i == 10){key = 11;}
				else if(i == 11){key = 10;}
				else{}
				//key = i;
			}
		}
		printf("touched\n");

		// Fill the pass vector
		if((int)pass.size()>3){
			pass.erase(pass.begin());
		}
		pass.push_back(key);
		std::cout << "pass: " << pass[0] << " " << pass[1] << " " << pass[2] << " " << pass[3] << std::endl;
	}
}

bool sendSMS(){
	bool responseBool = false;
	std::cout << "Sending SMS";
	CURLcode ret; CURL *hnd;
		struct curl_slist *slist1;
		time_t now = time(NULL);
		char date[80];
		strftime(date, 80, "%Y-%m-%d_%H-%M-%S", localtime(&now));
		strftime(date, 80, "UTC", localtime(&now));
		std::string date1 = date;
		std::stringstream ss2;
		ss2 << now;
		srand (time(NULL));
		std::string ss;
		ss = "<stream>\n<protocol>v1</protocol>\n<at>now</at>\n<device>distanceSensor@davidnike18.davidnike18</device>\n<data>\n";
		ss = ss + "<action>sms</action>\n<value>9191</value>\n<from>bbb</from>\n</data>\n</stream>\n";
		std::cout << "\n\n\n ######## payload ######  \n" << ss.c_str()  << "#################### \n\n\n";
		slist1 = NULL;
		slist1 = curl_slist_append(slist1, "carriots.apikey:996367ee330a4ed903e2253780215dba3e72d24556bcc9917dcb6960da207441");
		slist1 = curl_slist_append(slist1, "content-type:application/xml");
		hnd = curl_easy_init();
		curl_easy_setopt(hnd, CURLOPT_URL, "http://api.carriots.com/streams");
		curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, ss.c_str());
		curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist1);
		curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
		ret = curl_easy_perform(hnd);
		// Response
		int http_code;
		curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE, &http_code);
		std::cout << "\n http_code = " << http_code << " \n "; // << str(curl_easy_strerror(ret));
		if (http_code == 200){ 
			std::cout << "Response OK\n"; 
			responseBool = true;
		}else{ 
			std::cout << "Response Fail\n";
			responseBool = false;
		}
		curl_easy_cleanup(hnd);
		hnd = NULL;
		curl_slist_free_all(slist1);
		slist1 = NULL;
		sleep(1);

	return responseBool;

}

bool  enterPasword(){

	std::stringstream ss;

	std::cout << "In enterPassword \n ";

	oled_128x64_write_line(&oled, 3, 3, "Insert password:");

	double seconds_since_start = 30 - difftime( time(0), start);
	ss << seconds_since_start;
	std::string b;
	b = ss.str();
	oled_128x64_write_line(&oled, 4, 15, (char*) b.c_str());
	std::cout << "Time left: " << seconds_since_start;

	if(seconds_since_start < 0){
		oled_128x64_clear_display(&oled);
		oled_128x64_write_line(&oled, 0, 0, "Send SMS");
		if(sendSMS()){
			oled_128x64_write_line(&oled, 3, 3, "===>");
			return true;
		}else{
			oled_128x64_write_line(&oled, 0, 3, " No sent :-(");
		}
	}

	// Clear stringstream
	ss.str(std::string());	
	std::cout << "pass";
	for(int i = 0; i < (int)pass.size(); i++){
		ss << pass[i] << " ";
		std::cout << "["<<  i << "]: " << pass[i] << " ";
	}
	std::cout << std::endl;
	std::string a;
	a = ss.str();
	oled_128x64_write_line(&oled, 5, 3, (char*) a.c_str());
	if((int)pass.size() > 3 ){
		if(pass[0] == 1 && pass[1] == 4 && pass[2] == 1 && pass[3] == 8){
			std::cout << "password correct! \n";
			pass.clear(); //[0] = 0 && pass[1] == 4 && pass[2] == 1 && pass[3] == 8
			return true;
		}
	}
return false;
}

int main(void) {

// To test send a SMS (5 per day / one per minute.)
//	sendSMS();
//	return 0;

/*
 time_t now2 = time(NULL);
 char date[80];
// strftime(date, 80, "%Y-%m-%d_%H-%M-%S", localtime(&now2));
 strftime(date, 80, "%H:%M:%S", localtime(&now2));
 std::cout <<"TIME => " <<  date << '\n';
 std::string date1 = date;
*/
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

    multi_touch_recalibrate(&mt);

    /// 4095 => 0111111111111. 1-11=> ON, 12=> OFF
    multi_touch_set_electrode_config(&mt, 4094);

	multi_touch_register_callback(&mt,
	                              MULTI_TOUCH_CALLBACK_TOUCH_STATE,
	                              (void *)cb_touch_state,
	                              NULL);

	sleep(1);
   oled_128x64_clear_display(&oled);
    while(true){
    	// Get current distance value
    	uint16_t distance;
    	if(distance_us_get_distance_value(&dus, &distance) < 0) {
        	fprintf(stderr, "Could not get distance value, probably timeout\n");
		oled_128x64_clear_display(&oled);
		oled_128x64_write_line(&oled, 3, 1, "No Distance.! Timeout");
		oled_128x64_write_line(&oled, 4, 1, "Rebooting ...");
		// Reboot BB in case of no response from the distance sensor
		reboot(LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, LINUX_REBOOT_CMD_POWER_OFF, 0);
        	return 1;
    	}else{
//		oled_128x64_clear_display(&oled);
                oled_128x64_write_line(&oled, 0, 1, " Alarm: Ok");
		std::stringstream ss;
		ss << distance;
		std::string a;
        	a = ss.str();
		oled_128x64_write_line(&oled, 3, 1, " Dist:");
		oled_128x64_write_line(&oled, 3, 10, (char*) a.c_str());

		oled_128x64_write_line(&oled, 6, 1, " Time: ");

		// Adding one hour (1*60*60) to the UTC time
		 time_t now2 = time(NULL) + (60*60);
		 char date[80];
		// strftime(date, 80, "%Y-%m-%d_%H-%M-%S", localtime(&now2));
		 strftime(date, 80, "%H:%M:%S", localtime(&now2));
		 std::cout <<"TIME => " <<  date << '\n';
		 std::string date1 = date;

		oled_128x64_write_line(&oled, 6, 10, (char*) date1.c_str());
	}

    	printf("Distance Value: %u\n", distance);

	if((distance < 800) || (distance > 1200)){
		// Clear display
		oled_128x64_clear_display(&oled);
		// Write "Hello World" starting from upper left corner of the screen
		oled_128x64_write_line(&oled, 0, 3, "The door have been open!");
		// Start timer
		start = time(0);
        	piezo_speaker_morse_code(&ps, ". - . - . -", 2000);
		bool pasword = false;
		while(!pasword){
			pasword = enterPasword();
			if(pasword){
				oled_128x64_clear_display(&oled);
				oled_128x64_write_line(&oled, 0, 0, " Password Correct! ");
			}
			piezo_speaker_morse_code(&ps, ". - . - . -", 2000);
			usleep(500000);
		}

		distance_us_get_distance_value(&dus, &distance);
    		printf("Distance Value: %u\n", distance);
		while(!((distance > 800) && (distance < 1200))){
			oled_128x64_clear_display(&oled);
                                oled_128x64_write_line(&oled, 0, 1, "waiting to act. alarm! ");
			distance_us_get_distance_value(&dus, &distance);
			std::cout << "Waiting to close the door to activate the alarm \n ";
			sleep(1);
		}
	oled_128x64_clear_display(&oled);
	}

	    sleep(1);
    }




    printf("Press key to exit\n");
    getchar();
    distance_us_destroy(&dus);
    ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
    return 0;
}
