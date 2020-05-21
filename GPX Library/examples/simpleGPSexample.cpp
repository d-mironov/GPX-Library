/*
 * GPX Library example
 *
 * This is an example for the GPX library
 *
 * THis library automatically creates a .gpx file from the GPS data of your GPS module,
 * so you can easyly just import it into GeoBrowser or other GPX Data visualizers.
 *
 */

#include <Arduino.h>
#include "TinyGPS++.h"
#include <HardwareSerial.h>
#include "GPX.h"

#define SD_CS 5
#define btn 13

HardwareSerial GPSSerial(2);

float lat, lng;

GPX gpx;
TinyGPSPlus gps;
File file;

void setup()
{
    Serial.begin(9600);
    GPSSerial.begin(9600, SERIAL_8N1, 16, 17);
    pinMode(btn, INPUT);

    if (!SD.begin(SD_CS))
    {
        Serial.println("[-] SD Card init failed....");
    }
    Serial.println("[+] SD card init successfull");
    if (SD.exists("/file.gpx"))
    {
        Serial.println("[+] File exists. Opening....");
        file = SD.open("/file.gpx", FILE_WRITE);
    }
    Serial.println("Creating file...");
    file = SD.open("/file.gpx", FILE_WRITE);

    /*
	 * Initializes the library where you can set the mode and the Delay.
	 * Also you need to specify the file where you want to store your .gpx
	 */
    gpx.init(NO_DELAY, MODE3, file);

    while (!GPSSerial.available())
    {
        Serial.print(".");
        delay(200);
    }
    Serial.println("[+] GPS successfully initialized...");
}

void loop()
{
    bool newData = false;
    //printValues();

    for (unsigned long start = millis(); millis() - start < 1000;)
    {
        while (GPSSerial.available())
        {
            char c = GPSSerial.read();
            if (gps.encode(c))
            {
                newData = true;
            }
            Serial.print(c);
        }
    }
    /*
	 * Write the data to the file as soon as the new Data is available
	 */
    if (newData)
    {
        gpx.write(gps);
    }

    /*
	 * I could not make it that it does it automatically so you will need to press a button to write the Data
	 * into the file to close and safe it.
	 */
    if (digitalRead(btn) == 1)
    {
        gpx.close();
    }
}
