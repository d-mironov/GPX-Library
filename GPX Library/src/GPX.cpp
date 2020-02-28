/*
 * GPX.cpp
 *
 * author: SL7
 * version: v0.1
 */

#include "GPX.h"

void GPX::init(write_delay delay, data_mode mode, timezones zone, File file) {
	this->mode = mode;
	this->zone = zone;
	if (!file) {
		Serial.println("[-] The File could not be found....");
	} else {
		fileValid = true;
		this->file = file;
	}
}

void GPX::write(TinyGPSPlus gps) {
	if (fileValid) {
		for (String line : base_struct_start) {
			file.print(line);
		}
		data d1;
		d1.lat = gps.location.lat();
		d1.lng = gps.location.lng();
		d1.alt = gps.altitude.meters();
		d1.date = gps.date.year()
				+ "-" + gps.date.month() + "-" + gps.date.day()
				+ "T" + gps.time.hour() + "-" + gps.time.minute() + "-" + zone;
		switch (mode) {
		case MODE1:
			file.print("      <trkpt lat=\"" + d1.lat + "\" lon=\"" + d1.lng + "\"></trkpt>\n");
			break;
		case MODE2:
			file.print("      <trkpt lat=\"" + d1.lat + "\" lon=\"" + d1.lng + "\"><ele>" + d1.alt + "</ele></trkpt>\n");
			break;
		case MODE3:
			file.print("      <trkpt lat=\"" + d1.lat + "\" lon=\"" + d1.lng + "\"><ele>" + d1.alt + "</ele><time>" + d1.date + "</time></trkpt>\n");
		}
	} else {
		Serial.println("[-] Cannot write because no SD Card was initialized.");
	}
}

void GPX::close() {
	if (fileValid) {
		for (String line : base_struct_end) {
			file.print(line);
		}
		file.close();
	} else {
		Serial.println("[-] Cannot close the file becaue no SD Card was initialized.");
	}

}

