#include <Arduino_LSM9DS1.h>  // Library for integrated IMU
#include <Adafruit_GPS.h> // Include GPS library for parsing
#include <SPI.h>
#include <SD.h>
#include <string>

const int SDpin = 10;

#define GPSSerial Serial1 // GPS in Serial1
Adafruit_GPS GPS(&GPSSerial); // Constructor to use Adafruit library
#define GPSECHO false
uint32_t timer = millis();
char filename[15];
File dataFile;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

//----------- SD SETUP ----------- //
  Serial.print("Initializing SD card..."); // Open serial communications and wait for port to open:

  if (!SD.begin(SDpin)) {   // see if the card is present and can be initialized:
      Serial.println("Reader failed, or SD card not present");
      while (1); // don't do anything more:
  }
  Serial.println("card initialized.");

strcpy(filename, "/fdata00.TXT");
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = '0' + i/10;
    filename[7] = '0' + i%10;
    // create file if name does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }
dataFile = SD.open(filename, FILE_WRITE);
 if (dataFile) {
//----------- IMU SETUP ----------- //
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
  }
  dataFile.println("******* Sensor initialization ********")
  dataFile.println("IMU: ")
  dataFile.print("Magnetic field sample rate = ");
  dataFile.print(IMU.magneticFieldSampleRate());
  dataFile.print(" uT");
  dataFile.println("\t[X\tY\tZ]");
  dataFile.println();
  dataFile.print("Gyroscope sample rate = ");
  dataFile.print(IMU.gyroscopeSampleRate());
  dataFile.print(" Hz");
  dataFile.print("\tGyroscope in degrees/second");
  dataFile.println("[X\tY\tZ]");
  dataFile.println();
  dataFile.print("Accelerometer sample rate = ");
  dataFile.print(IMU.accelerationSampleRate());
  dataFile.print(" Hz");
  dataFile.print("\tAcceleration in G's");
  dataFile.println("[X\tY\tZ]");
//----------- GPS SETUP ----------- //
  // 9600 baud is the default rate for the Ultimate GPS and it works for the sd card reader
  dataFile.println("GPS: ")
  GPSSerial.begin(9600);
  if (!GPS.available()) {
    dataFile.println("GPS not available (**Ignore following line**)");
  }
  dataFile.println("GPS Online!");
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGAGSA); // Receive RMC, GGA, GSA data
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ); // 10 Hz update rate
  GPS.sendCommand(PMTK_API_SET_FIX_CTL_5HZ);
  delay(1000);
  GPSSerial.println(PMTK_Q_RELEASE);


  dataFile.println("__________________________________________ F l i g h t  -  D a t a __________________________________________");
  dataFile.println("x-acc \ty-acc \tz-acc \tx-gyr \ty-gyr \tz-gyr \tx-mag \ty-mag \tz-mag \tFix \tQuality  \tLat \tLong \tSpeed \tSats \ttime \tGPS Alt \tBaro Alt \tTemp ");
  dataFile.println("(m/s) \t(m/s) \t(m/s) \t(deg) \t(deg) \t(deg) \t(uT)  \t(uT)  \t(uT)  \t --- \t --- \t  \t   \t(knots) \t ---  \tUTC  \t(m) \t  \t  (m) \t \t(C)");
  dataFile.println("--------------------------------------------------------------------------------------------------------- ");
  dataFile.close();

//----------- BAROM SETUP ----------- //
//************************** TO DO

  }// if the file isn't open, pop up an error:
  else {
    Serial.println("error writting to file");
    while(1);
  }

}

void loop() {
  float x, y, z;
  char c = GPS.read();
  String out = ""
  dataFile = SD.open(filename, FILE_WRITE);
    // if the file is available, write to it:
  if (dataFile) {
  if (IMU.magneticFieldAvailable()) {
    IMU.readAcceleration(x, y, z);
    // dataFile.print("Accelerometer \t");
    dataFile.print(x);
    dataFile.print('\t');
    dataFile.print(y);
    dataFile.print('\t');
    dataFile.print(z);
    dataFile.print('\t');

    IMU.readGyroscope(x, y, z);
    // dataFile.print("Gyroscope \t");
    dataFile.print(x);
    dataFile.print('\t');
    dataFile.print(y);
    dataFile.print('\t');
    dataFile.print(z);
    dataFile.print('\t');

    IMU.readMagneticField(x, y, z);
    // dataFile.print("Magnetometer \t");
    dataFile.print(x);
    dataFile.print('\t');
    dataFile.print(y);
    dataFile.print('\t');
    dataFile.print(z);
    dataFile.print('\t');
  }
  //if (Serial.available()) {
  //  char c = Serial.read();
  //  GPSSerial.write(c);
  //}
  //if (GPSSerial.available()) {
    //char c = GPSSerial.read();
    //Serial.write(c);
  //}
  // delay(100);
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    Serial.print(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    GPS.parse(GPS.lastNMEA())
    //if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
    //  return; // we can fail to parse a sentence in which case we should just wait for another
  }
    if (millis() - timer > 500) {
    timer = millis(); // reset the timer
    //dataFile.print("\nTime: ");
    if (GPS.hour < 10) { dataFile.print('0'); }
    dataFile.print(GPS.hour, DEC); dataFile.print(':');
    if (GPS.minute < 10) { dataFile.print('0'); }
    dataFile.print(GPS.minute, DEC); dataFile.print(':');
    if (GPS.seconds < 10) { dataFile.print('0'); }
    dataFile.print(GPS.seconds, DEC); dataFile.print('.');
    if (GPS.milliseconds < 10) {
      dataFile.print("00");
    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      dataFile.print("0");
    }
    dataFile.print(GPS.milliseconds);
    //dataFile.print("Date: ");
    //dataFile.print(GPS.day, DEC); dataFile.print('/');
    //dataFile.print(GPS.month, DEC); dataFile.print("/20");
    //dataFile.println(GPS.year, DEC);
    //dataFile.print("Fix: "); 
    dataFile.print((int)GPS.fix);
    //dataFile.print(" quality: "); 
    dataFile.print((int)GPS.fixquality);
    if (GPS.fix) {
      //dataFile.print("Location: ");
      dataFile.print(GPS.latitude, 4); dataFile.print(GPS.lat);
      //dataFile.print(", ");
      dataFile.print('\t');
      dataFile.print(GPS.longitude, 4); dataFile.println(GPS.lon);
      //dataFile.print("Speed (knots): "); 
      dataFile.print('\t');
      dataFile.println(GPS.speed);
      //dataFile.print("Angle: "); 
      //dataFile.print('\t');
      //dataFile.println(GPS.angle);
      //dataFile.print("Altitude: "); 
      dataFile.print('\t');
      //dataFile.print("Satellites: "); 
      dataFile.println((int)GPS.satellites);
      dataFile.print('\t');
      dataFile.print(GPS.altitude);
    }
  }
  //****************** Barometer data goes here
  dataFile.println('baro not configged');
  dataFile.close();
}
    // if the file isn't open, pop up an error:
    else {
    Serial.println("error opening datalog.txt");
  }
}
