/*
 * GPX.cpp
 *
 * author: SL7
 * version: v0.1
 */

#include "GPX.h"

void GPX::init(write_delay delay, data_mode mode, File file) {
	this->mode = mode;
	//this->zone = zone;
	if (!file) {
		Serial.println("[-] The File could not be found....");
	} else {
		isvalidFile = true;
		this->file = file;
		for (String line : base_struct_start) {
			file.print(line);
		}
	}
}

void GPX::write(TinyGPSPlus gps) {
	if (isvalidFile && !isclosedFile) {
		data d1;
		d1.lat = gps.location.lat();
		d1.lng = gps.location.lng();
		char lat_final[20];
		char lng_final[20];
		sprintf(lat_final, "%.6f", d1.lat);
		sprintf(lng_final, "%.6f", d1.lng);
		d1.alt = gps.altitude.meters();
		d1.date = String(gps.date.year())
				+ "-" + String(gps.date.month()) + "-" + String(gps.date.day())
				+ "T" + String(gps.time.hour()) + "-" + String(gps.time.minute()) + "UTC+1";
		switch (mode) {
		case 0:
			file.print("      <trkpt lat=\"" + String(lat_final) + "\" lon=\"" + String(lng_final) + "\"></trkpt>\n");
			break;
		case 1:
			file.print("      <trkpt lat=\"" + String(lat_final) + "\" lon=\"" + String(lng_final) + "\"><ele>" + String(d1.alt) + "</ele></trkpt>\n");
			break;
		case 2:
			file.print("      <trkpt lat=\"" + String(lat_final) + "\" lon=\"" + String(lng_final) + "\"><ele>" + String(d1.alt) + "</ele><time>" + d1.date + "</time></trkpt>\n");
		}
	} else {
		Serial.println("[-] Cannot write because no SD Card was initialized.");
	}
}

void GPX::close() {
	if (isvalidFile && !isclosedFile) {
		for (String line : base_struct_end) {
			file.print(line);
		}
		file.close();
		isclosedFile = true;
	} else {
		Serial.println("[-] Cannot close the file becaue no SD Card was initialized.");
	}

}

