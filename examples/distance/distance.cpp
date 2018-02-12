#include <stdio.h>
#include <curl/curl.h>

#include <time.h>

#include <iostream>
#include <unistd.h>
#include  <math.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sstream>

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/forwards.h>

#define IPCON_EXPOSE_MILLISLEEP

#include "ip_connection.h"
#include "bricklet_distance_us.h"
#include "bricklet_industrial_quad_relay.h"

#define HOST "192.168.178.52" //  "192.168.0.19"
#define PORT 4223
#define UIDdistance "zqf" // Change XYZ to the UID of your Distance US Bricklet
#define UIDrelay "BvA" // Change XYZ to the UID of your Industrial Quad Relay Bricklet
#define UIDOled "BvA"
#define UIDKeyPad "BvA"
#define UIDBuzzer "a"

int distanceCm;
bool sendSMS = true;
//bool openMainDoor;
int distanceCallbackCounter;
int pullDataCounter;

using namespace std;

void doorIsOpen();
void sendDistance(int distanceSensor);
int returnHour(string valueString);
int returnMinute(string valueString);
// Callback function for distance value callback
void cb_distance(uint16_t distance, void *user_data) {
	(void)user_data; // avoid unused parameter warning

	//printf("Distance Value: %d\n", distance);

	distanceCm = distance;  
 //''' DISTANCE SENSOR'''
                if(distanceCm > 500){
                        if(sendSMS){
                            doorIsOpen();
                            sendSMS = false;
                        }
                }else if(distanceCm < 500){
                        sendSMS = true;
                }

	if(distanceCallbackCounter > 1200){
	sendDistance(distanceCm);
	distanceCallbackCounter = 0;
	}else{
	distanceCallbackCounter = distanceCallbackCounter + 1;
	}
}


double string2double(string str)
{
 istringstream buffer(str);
 double temp;
 buffer >> temp;
 return temp;
}

void  readJson(bool &openDoor, int &hour, int &min){

	std::stringstream ss, action, value;
	std::ifstream people_file("fileData.json", std::ifstream::binary);
	string actionString, valueString;
	Json::Value people;
	people_file >> people;

	cout<<people["total_documents"];
	ss << people["total_documents"];
	int numOfDocumentes = string2double(ss.str());

	for(int i = 0; i<numOfDocumentes; i++){
		//cout<<people["result"][i]["_id"];
		//cout<<people["result"][i]["data"];
		action.str(std::string());
		action << people["result"][i]["data"]["action"];
		actionString = action.str(); 
		//cout<<people["result"][i]["data"]["action"] <<" string " << actionString;
		//cout<<people["result"][i]["data"]["value"];
		//cout<<people["result"][i]["data"]["action"];
		value.str(std::string());
		if(actionString.compare("\"set\"\n")==0){
			cout << "set time action \n";
			//value.str(std::string());
                	value << people["result"][i]["data"]["hour"];
                	valueString = value.str();
			hour = returnHour(valueString);
                        cout << " time set to open the door = " << hour;
			// Minute
			value.str(std::string());
			value << people["result"][i]["data"]["minute"];
			valueString = value.str();
                        min = returnMinute(valueString);
                        cout << " : " << min;

		}
		cout<<"\n---\n";
	}

	action.str(std::string());
        action << people["result"][numOfDocumentes]["data"]["action"];
        actionString = action.str(); 
	if(actionString.compare("\"open\"\n")==0){
			cout << "open door action \n";
			value << people["result"][numOfDocumentes]["data"]["value"];
                        valueString = value.str();
			if(valueString.compare("\"2222\"\n")==0){
				openDoor = true;
			}
	}

		//cout << "set 2 " << actionString.compare(" \"set\" ");
	//cout<<"\n---\n";
	//sleep(1);


	//cout<<people["result"][numOfDocumentes-1]["data"]["door"];
//	ss.str(std::string());
//	ss << people["result"][numOfDocumentes-1]["data"]["action"];
	//string action = ss.str();
//	ss.str(std::string());
//	ss << people["result"][numOfDocumentes-1]["data"]["value"];
	//string value = ss.str();

//	std::cout << "dor value in JSON " << value << " " << ss.str() <<"\n";	

	//ss.str(std::string());
	//ss << people["result"][numOfDocumentes-1]["data"]["distance"];
 	//int distance2 = string2double(ss.str());

	//return valueString;
}

void doorIsOpen(){
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
		cout << "\n http_code = " << http_code << " \n "; // << str(curl_easy_strerror(ret));
		if (http_code == 200){ cout << "Response OK\n"; }else{ cout << "Response Fail\n";}
		curl_easy_cleanup(hnd);
		hnd = NULL;
		curl_slist_free_all(slist1);
		slist1 = NULL;
		sleep(10);
}

void sendDistance(int distanceSensor){
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
                ss = "<stream>\n<protocol>v1</protocol>\n";
                ss = ss + "<at>now</at>\n<device>distanceSensor@davidnike18.davidnike18</device>\n<data>\n";
                ss2.str(std::string()); ss2 << distanceSensor;
		ss = ss + "<distance>"+ss2.str()+"</distance>\n</data>\n</stream>\n";
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
                cout << "\n http_code = " << http_code << " \n "; // << str(curl_easy_strerror(ret));
                if (http_code == 200){ cout << "Response OK\n"; }else{ cout << "Response Fail\n";}
                curl_easy_cleanup(hnd);
                hnd = NULL;
                curl_slist_free_all(slist1);
                slist1 = NULL;
                sleep(10);
}

string pullData (bool &openDoor, int &hour, int &min){

	system(("curl --header carriots.apikey:996367ee330a4ed903e2253780215dba3e72d24556bcc9917dcb6960da207441 http://api.carriots.com/streams/?device=defaultDevice@davidnike18.davidnike18 -o fileData.json"));
	//bool openDoor = true;
	//int hour = 0;
	//int min = 0;
	readJson(openDoor, hour, min );
	string temp = "pullDataReturnString";
	return temp;
}


int main(void) {

	bool openDoor = false;
	int hourSet = 0;
	int minSet = 0;
	// Create IP connection
	IPConnection ipcon;
	ipcon_create(&ipcon);

	// Create device object
	DistanceUS dus;
	distance_us_create(&dus, UIDdistance, &ipcon);
	IndustrialQuadRelay iqr;
	industrial_quad_relay_create(&iqr, UIDrelay, &ipcon);

	// Connect to brickd
	if(ipcon_connect(&ipcon, HOST, PORT) < 0) {
		fprintf(stderr, "Could not connect\n");
		return 1;
	}
	// Don't use device before ipcon is connected

	// Register distance value callback to function cb_distance
	distance_us_register_callback(&dus,
	                              DISTANCE_US_CALLBACK_DISTANCE,
	                              (void *)cb_distance,
	                              NULL);

	// Set period for distance value callback to 0.2s (200ms)
	// Note: The distance value callback is only called every 0.2 seconds
	//       if the distance value has changed since the last call!
	distance_us_set_distance_callback_period(&dus, 3000); // cada 3 seg

//	int i;
//	for(i = 0; i < 10; ++i) {
//	millisleep(1000);
//	industrial_quad_relay_set_value(&iqr, 1 << 0);
//	fprintf(stderr, "ON");
//	millisleep(1000);
	industrial_quad_relay_set_value(&iqr, 0 << 1);
//	fprintf(stderr, "OFF");
//	}

	millisleep(5000);
	pullData (openDoor, hourSet, minSet);
	std::cout << "Setted time = " <<hourSet << ":" << minSet << "\n";
	millisleep(5000);

	distanceCallbackCounter = 0;
	pullDataCounter = 0;
	//openMainDoor = true;
	openDoor = false;

	while(true){
		time_t theTime = time(NULL);
		struct tm *aTime = localtime(&theTime);
		int hour=aTime->tm_hour;
		int min=aTime->tm_min;

		if(hour == hourSet){
             		if(minSet <= min && min <= (minSet+10)){
				std::cout <<" Time to open the door (pull data)\n";
				int tempH, tempM;
				pullData (openDoor, tempH, tempM);
			}
		} 
		//''' MAIN DOOR '''
		//std::cout << "pull data \n";
		//string door = pullData (openDoor, hourSet, minSet);
		//millisleep(500);
		//std::cout << "Setted time = " <<hourSet << ":" << minSet << "\n";
		// 1111 close door
		// 2222 open door

		//time_t theTime = time(NULL);
                //struct tm *aTime = localtime(&theTime);
                //int hour=aTime->tm_hour;
                //int min=aTime->tm_min;

		//std::cout << "open door?  " << openDoor << "\n";
		//string doorToCompare = "\"2222\"";
		//std::cout << "String to compare " << doorToCompare << "\n";
		//std::cout << "compare " << door.compare(doorToCompare) << "\n";
		//if(door.compare(doorToCompare) == 1 && openMainDoor){
		if(openDoor){
			std::cout << "Openning door \n";
			std::cout << "Checking set time \n";
		//	if(hour == hourSet){
		//		if(minSet <= min && min <= (min+10)){ 
					industrial_quad_relay_set_value(&iqr, 1 << 0);
					millisleep(1000);
					industrial_quad_relay_set_value(&iqr, 0 << 1);
					millisleep(1000);
					openDoor = false;
					minSet = 99;
					hourSet = 99;
		//		}
//			}
		}

		//std::cout << "pull data \n";
                //string door = pullData (openDoor, hourSet, minSet);
                pullDataCounter = pullDataCounter + 1;
		if(pullDataCounter > 600){
		std::cout <<"pull data \n";
		string door = pullData (openDoor, hourSet, minSet);
		std::cout << "Setted time = " <<hourSet << ":" << minSet << "\n";
		pullDataCounter = 0;
		} 
		//millisleep(500);
                //std::cout << "Setted time = " <<hourSet << ":" << minSet << "\n";
		millisleep(3000);
	} // while

	//printf("Press key to exit\n");
	getchar();
	distance_us_destroy(&dus);
	industrial_quad_relay_destroy(&iqr);
	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally


	return 0;
}

int returnHour(string valueString){
         if(valueString.compare("\"0\"\n")==0){ return 0;
        }else if(valueString.compare("\"1\"\n")==0){return 1;
        }else if(valueString.compare("\"2\"\n")==0){return 2;
        }else if(valueString.compare("\"3\"\n")==0){return 3;
        }else if(valueString.compare("\"4\"\n")==0){return 4;
        }else if(valueString.compare("\"5\"\n")==0){return 5;
        }else if(valueString.compare("\"6\"\n")==0){return 6;
        }else if(valueString.compare("\"7\"\n")==0){return 7;
        }else if(valueString.compare("\"8\"\n")==0){return 8;
        }else if(valueString.compare("\"9\"\n")==0){return 9;
        }else if(valueString.compare("\"10\"\n")==0){return 10;
        }else if(valueString.compare("\"11\"\n")==0){return 11;
        }else if(valueString.compare("\"12\"\n")==0){return 12;
        }else if(valueString.compare("\"13\"\n")==0){return 13;
        }else if(valueString.compare("\"14\"\n")==0){return 14;
        }else if(valueString.compare("\"15\"\n")==0){return 15;
        }else if(valueString.compare("\"16\"\n")==0){return 16;
        }else if(valueString.compare("\"17\"\n")==0){return 17;
        }else if(valueString.compare("\"18\"\n")==0){return 18;
        }else if(valueString.compare("\"19\"\n")==0){return 19;
        }else if(valueString.compare("\"20\"\n")==0){return 20;
        }else if(valueString.compare("\"21\"\n")==0){return 21;
        }else if(valueString.compare("\"22\"\n")==0){return 22;
        }else if(valueString.compare("\"23\"\n")==0){return 23;
        }else if(valueString.compare("\"24\"\n")==0){return 24;
        }else {return 9999;
	}
}

int returnMinute(string valueString){
         if(valueString.compare("\"0\"\n")== 0){ return 0;
	}else if(valueString.compare("\"00\"\n")==0){return 0;
        }else if(valueString.compare("\"1\"\n")==0){return 1;
	}else if(valueString.compare("\"01\"\n")==0){return 1;
        }else if(valueString.compare("\"2\"\n")==0){return 2;
	}else if(valueString.compare("\"02\"\n")==0){return 2;
        }else if(valueString.compare("\"3\"\n")==0){return 3;
	}else if(valueString.compare("\"03\"\n")==0){return 3;
        }else if(valueString.compare("\"4\"\n")==0){return 4;
	}else if(valueString.compare("\"04\"\n")==0){return 4;
        }else if(valueString.compare("\"5\"\n")==0){return 5;
	}else if(valueString.compare("\"05\"\n")==0){return 5;
        }else if(valueString.compare("\"6\"\n")==0){return 6;
	}else if(valueString.compare("\"06\"\n")==0){return 6;
        }else if(valueString.compare("\"7\"\n")==0){return 7;
	}else if(valueString.compare("\"07\"\n")==0){return 7;
        }else if(valueString.compare("\"8\"\n")==0){return 8;
	}else if(valueString.compare("\"08\"\n")==0){return 8;
        }else if(valueString.compare("\"9\"\n")==0){return 9;
	}else if(valueString.compare("\"09\"\n")==0){return 9;
        }else if(valueString.compare("\"10\"\n")==0){return 10;
        }else if(valueString.compare("\"11\"\n")==0){return 11;
        }else if(valueString.compare("\"12\"\n")==0){return 12;
        }else if(valueString.compare("\"13\"\n")==0){return 13;
        }else if(valueString.compare("\"14\"\n")==0){return 14;
        }else if(valueString.compare("\"15\"\n")==0){return 15;
        }else if(valueString.compare("\"16\"\n")==0){return 16;
        }else if(valueString.compare("\"17\"\n")==0){return 17;
        }else if(valueString.compare("\"18\"\n")==0){return 18;
        }else if(valueString.compare("\"19\"\n")==0){return 19;
        }else if(valueString.compare("\"20\"\n")==0){return 20;
        }else if(valueString.compare("\"21\"\n")==0){return 21;
        }else if(valueString.compare("\"22\"\n")==0){return 22;
        }else if(valueString.compare("\"23\"\n")==0){return 23;
        }else if(valueString.compare("\"24\"\n")==0){return 24;
	}else if(valueString.compare("\"25\"\n")==0){return 25;
        }else if(valueString.compare("\"26\"\n")==0){return 26;
        }else if(valueString.compare("\"27\"\n")==0){return 27;
        }else if(valueString.compare("\"28\"\n")==0){return 28;
        }else if(valueString.compare("\"29\"\n")==0){return 29;
        }else if(valueString.compare("\"30\"\n")==0){return 30;
        }else if(valueString.compare("\"31\"\n")==0){return 31;
        }else if(valueString.compare("\"32\"\n")==0){return 32;
        }else if(valueString.compare("\"33\"\n")==0){return 33;
        }else if(valueString.compare("\"34\"\n")==0){return 34;
        }else if(valueString.compare("\"35\"\n")==0){return 35;
        }else if(valueString.compare("\"36\"\n")==0){return 36;
        }else if(valueString.compare("\"37\"\n")==0){return 37;
        }else if(valueString.compare("\"38\"\n")==0){return 38;
        }else if(valueString.compare("\"39\"\n")==0){return 39;
        }else if(valueString.compare("\"40\"\n")==0){return 40;
        }else if(valueString.compare("\"41\"\n")==0){return 41;
        }else if(valueString.compare("\"42\"\n")==0){return 42;
        }else if(valueString.compare("\"43\"\n")==0){return 43;
        }else if(valueString.compare("\"44\"\n")==0){return 44;
        }else if(valueString.compare("\"45\"\n")==0){return 45;
        }else if(valueString.compare("\"46\"\n")==0){return 46;
        }else if(valueString.compare("\"47\"\n")==0){return 47;
        }else if(valueString.compare("\"48\"\n")==0){return 48;
        }else if(valueString.compare("\"49\"\n")==0){return 49;
        }else if(valueString.compare("\"50\"\n")==0){return 50;
        }else if(valueString.compare("\"51\"\n")==0){return 51;
        }else if(valueString.compare("\"52\"\n")==0){return 52;
        }else if(valueString.compare("\"53\"\n")==0){return 53;
        }else if(valueString.compare("\"54\"\n")==0){return 54;
        }else if(valueString.compare("\"55\"\n")==0){return 55;
        }else if(valueString.compare("\"56\"\n")==0){return 56;
	}else if(valueString.compare("\"57\"\n")==0){return 57;
        }else if(valueString.compare("\"58\"\n")==0){return 58;
        }else if(valueString.compare("\"59\"\n")==0){return 59;
	}else{ return 9999;
	}
}
