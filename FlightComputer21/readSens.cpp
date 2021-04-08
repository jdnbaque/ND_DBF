<<<<<<< HEAD
// Reading GPS and IMU
// TODO: Parse data to isolate ground speed, lat, lon, heading. 

#include <Arduino_LSM9DS1.h>  // Library for integrated IMU
#include <Adafruit_GPS.h> // Include GPS library for parsing

#define GPSSerial Serial1 // GPS in Serial1
Adafruit_GPS GPS(&GPSSerial); // Constructor to use Adafruit library

#define GPSECHO false
uint32_t timer = millis();


void setup() {
  while (!Serial); // for debugging through serial
  Serial.begin(115200);
  Serial.println("Started");

 //IMU SETUP
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
  }
  Serial.print("Magnetic field sample rate = ");
  Serial.print(IMU.magneticFieldSampleRate());
  Serial.println(" uT");
  Serial.println();
  Serial.println("Magnetic Field in uT");
  Serial.println("X\tY\tZ");
  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Gyroscope in degrees/second");
  Serial.println("X\tY\tZ");
  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Acceleration in G's");
  Serial.println("X\tY\tZ");

  // 9600 baud is the default rate for the Ultimate GPS
  GPSSerial.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); // Receive defaul RMC GGA data
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  delay(1000);
  GPSSerial.println(PMTK_Q_RELEASE);
  
}




void loop() {
  float x, y, z;
  char c = GPS.read();
  
  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(x, y, z);
    //Serial.print("Magnetometer \t");
    //Serial.print(x);
    //Serial.print('\t');
    //Serial.print(y);
    //Serial.print('\t');
    //Serial.println(z);

    IMU.readGyroscope(x, y, z);
    //Serial.print("Gyroscope \t");
    //Serial.print(x);
    //Serial.print('\t');
    //Serial.print(y);
    //Serial.print('\t');
    //Serial.println(z);

    IMU.readAcceleration(x, y, z);
    //Serial.print("Accelerometer \t");
    //Serial.print(x);
    //Serial.print('\t');
    //Serial.print(y);
    //Serial.print('\t');
    //Serial.println(z);
  }
  //if (Serial.available()) {
  //  char c = Serial.read();
  //  GPSSerial.write(c);
  //}
  //if (GPSSerial.available()) {
  //  char c = GPSSerial.read();
  //  Serial.write(c);
  //}
  //delay(100);
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    Serial.print(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }
    if (millis() - timer > 1000) {
    timer = millis(); // reset the timer
    Serial.print("\nTime: ");
    if (GPS.hour < 10) { Serial.print('0'); }
    Serial.print(GPS.hour, DEC); Serial.print(':');
    if (GPS.minute < 10) { Serial.print('0'); }
    Serial.print(GPS.minute, DEC); Serial.print(':');
    if (GPS.seconds < 10) { Serial.print('0'); }
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    if (GPS.milliseconds < 10) {
      Serial.print("00");
    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      Serial.print("0");
    }
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    }
  }
}
=======
// Reading GPS and IMU
// TODO: Parse data to isolate ground speed, lat, lon, heading. 

#include <Arduino_LSM9DS1.h>  // Library for integrated IMU
#include <Adafruit_GPS.h> // Include GPS library for parsing
#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;

#define GPSSerial Serial1 // GPS in Serial1
Adafruit_GPS GPS(&GPSSerial); // Constructor to use Adafruit library

#define GPSECHO false
uint32_t timer = millis();

char filename[15];
void setup() {
  /*
  while (!Serial); // for debugging through serial
  Serial.begin(115200);
  Serial.println("Started");
*/
// sd setup

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

strcpy(filename, "/fdata00.TXT");
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = '0' + i/10;
    filename[7] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }

File dataFile = SD.open(filename, FILE_WRITE);
 if (dataFile) {
 //IMU SETUP
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
  }
  dataFile.print("Magnetic field sample rate = ");
  dataFile.print(IMU.magneticFieldSampleRate());
  dataFile.println(" uT");
  dataFile.println();
  dataFile.println("Magnetic Field in uT");
  dataFile.println("X\tY\tZ");
  dataFile.print("Gyroscope sample rate = ");
  dataFile.print(IMU.gyroscopeSampleRate());
  dataFile.println(" Hz");
  dataFile.println();
  dataFile.println("Gyroscope in degrees/second");
  dataFile.println("X\tY\tZ");
  dataFile.print("Accelerometer sample rate = ");
  dataFile.print(IMU.accelerationSampleRate());
  dataFile.println(" Hz");
  dataFile.println();
  dataFile.println("Acceleration in G's");
  dataFile.println("X\tY\tZ");

  dataFile.close();
  }
// if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }

  // 9600 baud is the default rate for the Ultimate GPS and it works for the sd card reader
  GPSSerial.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); // Receive defaul RMC GGA data
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  delay(1000);
  GPSSerial.println(PMTK_Q_RELEASE);




  
}





void loop() {
  float x, y, z;
  char c = GPS.read();
  File dataFile = SD.open(filename, FILE_WRITE);
    // if the file is available, write to it:
  if (dataFile) {
  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(x, y, z);
    dataFile.print("Magnetometer \t");
    dataFile.print(x);
    dataFile.print('\t');
    dataFile.print(y);
    dataFile.print('\t');
    dataFile.println(z);

    IMU.readGyroscope(x, y, z);
    dataFile.print("Gyroscope \t");
    dataFile.print(x);
    dataFile.print('\t');
    dataFile.print(y);
    dataFile.print('\t');
    dataFile.println(z);

    IMU.readAcceleration(x, y, z);
    dataFile.print("Accelerometer \t");
    dataFile.print(x);
    dataFile.print('\t');
    dataFile.print(y);
    dataFile.print('\t');
    dataFile.println(z);
  }
  if (Serial.available()) {
    char c = Serial.read();
    GPSSerial.write(c);
  }
  if (GPSSerial.available()) {
    char c = GPSSerial.read();
    Serial.write(c);
  }
  // delay(100);
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    Serial.print(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }





  
    if (millis() - timer > 1000) {
    timer = millis(); // reset the timer
    dataFile.print("\nTime: ");
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
    dataFile.println(GPS.milliseconds);
    dataFile.print("Date: ");
    dataFile.print(GPS.day, DEC); dataFile.print('/');
    dataFile.print(GPS.month, DEC); dataFile.print("/20");
    dataFile.println(GPS.year, DEC);
    dataFile.print("Fix: "); dataFile.print((int)GPS.fix);
    dataFile.print(" quality: "); dataFile.println((int)GPS.fixquality);
    if (GPS.fix) {
      dataFile.print("Location: ");
      dataFile.print(GPS.latitude, 4); dataFile.print(GPS.lat);
      dataFile.print(", ");
      dataFile.print(GPS.longitude, 4); dataFile.println(GPS.lon);
      dataFile.print("Speed (knots): "); dataFile.println(GPS.speed);
      dataFile.print("Angle: "); dataFile.println(GPS.angle);
      dataFile.print("Altitude: "); dataFile.println(GPS.altitude);
      dataFile.print("Satellites: "); dataFile.println((int)GPS.satellites);
    }
  }
  dataFile.close();
}
    // if the file isn't open, pop up an error:
    else {
    Serial.println("error opening datalog.txt");
  }
  }
>>>>>>> ff41ae8d33450feb5696b5eb516d54c9200f6f5f
