/*
 * GPX.cpp
 *
 * author: SL7
 * version: v0.1
 */

#include "GPX.h"

void GPX::init(String foldername, write_delay delay = DELAY_2IT, data_mode mode = MODE1)
{
    this->foldername = foldername;
    this->mode = mode;
    //this->zone = zone;
}

bool GPX::gotGPSLock(HardwareSerial GPSSerial, TinyGPSPlus gps)
{
    for (unsigned long start = millis(); millis() - start < 1000;)
    {
        while (GPSSerial.available())
        {
            char c = GPSSerial.read();
            gps.encode(c);
            //Serial.print(c);
        }
    }
    if (gps.location.isUpdated())
    {
        return true;
    }

    return false;
}

String GPX::setFile(File file)
{
    if (!file)
    {
        return "[!] File is invalid";
    }
    else
    {
        this->isvalidFile = true;
        this->isclosedFile = false;
        this->file = file;
        for (String line : base_struct_start)
        {
            this->file.print(line);
            this->file.flush();
        }
        return "[+] File succesfully set";
    }
}

void GPX::write(TinyGPSPlus gps)
{
    if (this->isvalidFile && !this->isclosedFile)
    {
        data d1;
        d1.lat = gps.location.lat();
        d1.lng = gps.location.lng();
        char lat_final[20];
        char lng_final[20];
        sprintf(lat_final, "%.6f", d1.lat);
        sprintf(lng_final, "%.6f", d1.lng);
        d1.alt = gps.altitude.meters();
        d1.date = String(gps.date.year()) + "-" + String(gps.date.month()) + "-" + String(gps.date.day()) + "T" + String(gps.time.hour()) + "-" + String(gps.time.minute()) + "UTC+1";
        switch (mode)
        {
        case 0:
            file.print("      <trkpt lat=\"" + String(lat_final) + "\" lon=\"" + String(lng_final) + "\"></trkpt>\n");
            file.flush();
            break;
        case 1:
            file.print("      <trkpt lat=\"" + String(lat_final) + "\" lon=\"" + String(lng_final) + "\"><ele>" + String(d1.alt) + "</ele></trkpt>\n");
            file.flush();
            break;
        case 2:
            file.print("      <trkpt lat=\"" + String(lat_final) + "\" lon=\"" + String(lng_final) + "\"><ele>" + String(d1.alt) + "</ele><time>" + d1.date + "</time></trkpt>\n");
            file.flush();
            break;
        default:
            file.print("      <trkpt lat=\"" + String(lat_final) + "\" lon=\"" + String(lng_final) + "\"></trkpt>\n");
            file.flush();
            break;
        }
    }
}

void GPX::close()
{
    if (this->isvalidFile && !this->isclosedFile)
    {
        for (String line : base_struct_end)
        {
            file.print(line);
        }
        file.close();
        isclosedFile = true;
        Serial.println("[+] File closed.");
    }
    else if (!this->isvalidFile)
    {
        Serial.println("[-] Cannot close the file becaue no SD Card was initialized.");
    }
    else
    {
        Serial.println("[!] The file is closed or the SD Card was removed. ");
    }
}
