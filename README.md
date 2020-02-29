# GPX-Library
This is an Arduino library to create a .gpx library from the GPS Module readings and insert it into a GPX Data Visualizer.

Therer is not much implemented at the moment but it does it job

# Prequisites

You need TinyGPS++ and the standard SD Library installed first before using this library

(optional) you can also use a button to close the file

# Usage
```cpp
GPX gpx;
/* The DelayMode specifies the delay to write the Data in
 * The Write Mode specifies the different data to write into the file
 * file Specifies the file to write the data to. It needs to be opened with the SD library first
 */
gpx.init(DelayMode, Writemode, file);

/* Writes the data to the specified file. 
 * requires the gps object as an attribute
 */
gpx.write(gps);

// Writes the "ending" of this fileformat and closes the file after that
gpx.close();
```

## Author
**SL7** - contact@flate.io

[Flate Industries](flate.io)  -   Website under construction

