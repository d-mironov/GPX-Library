/*
 * Source File
 * GPX Libary simple example
 *
 * This a an extension for the TinyGPS++ Library and it is not Standalone
 * it needs the GPS and the standard SD libaray installed
 *
 */

#include <Arduino.h>
#include "TinyGPS++.h"
#include <HardwareSerial.h>
#include "GPX.h"
/*
 * Hardware CS pin for the SD card module
 */
#define SD_CS 5
/*
 * I use an ESP32 so this is the GPS Init for the 2nd Serial Hardware Port
 * On Uno it would be a Software Serial Port
 */

/*
 * Connected a button to GPIO 13 to later close the file with a button press
 */
#define btn 13

HardwareSerial GPSSerial(2);

GPX gpx;
TinyGPSPlus gps;
File file;

void setup() {
	Serial.begin(9600);
	GPSSerial.begin(9600, SERIAL_8N1, 16, 17);

	pinMode(btn, INPUT);

	if (!SD.begin(SD_CS)) {
		Serial.println("[-] Initialization failed...");
	}
	Serial.println("[+] SD Card initialized.");

	/*
	 * If the file exists, just use the file
	 * If it does not then just create a new File
	 * on the SD Card
	 */
	if (SD.exists("file.gpx")) {
		Serial.println("[+] File exists. opening....");
		file = SD.open("file.gpx", FILE_WRITE);
	} else {
		Serial.println("[+] Creating File...");
		file = SD.open("file.gpx", FILE_WRITE);
	}

	gpx.init(NO_DELAY, MODE1, TZ_MEZ, file);

	while(!GPSSerial.available()) {
		Serial.print(".");
		delay(200);
	}
	Serial.println("[+] GPS successfully initialized...");
}

void loop() {
	bool newData = false;

	for (unsigned long start = millis(); millis() - start < 1000;) {
		while (GPSSerial.available()) {
			char c = GPSSerial.read();
			if (gps.encode(c)) {
				newData = true;
			}
			Serial.print(c);
		}
	}

	/*
	 * Wait until the GPS library receives all the Data so you can write it on the SD Card
	 */
	if (newData) {
		gpx.write(gps);
	}
	/*
	 * To close the file and make the file valid, there need to be added some lines into the file
	 * ATTENTION : The file itself is also closen after that
	 */
	if (digitalRead(btn) == 1) {
		gpx.close();
	}
}



