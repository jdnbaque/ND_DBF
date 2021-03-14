#include <bluefruit.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM6DSOX.h>
#include "Adafruit_BMP3XX.h"
#include <string.h>

uint32_t uvindexvalue;
#define UUID16_SVC_ENVIRONMENTAL_SENSING 0X181A
#define UUID16_SVC_LOCATION_AND_NAVIGATION 0X1819

#define UUID_CHR_UV_INDEX 0X2A6E
#define UUID_CHR_TEMPERATURE 0x2A6E 

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_LSM6DSOX sox; 
Adafruit_BMP3XX bmp;


BLEService environmental_sensing_service = BLEService(UUID16_SVC_ENVIRONMENTAL_SENSING);
BLECharacteristic uv_index_characteristic = BLECharacteristic(UUID_CHR_TEMPERATURE);

void setupESService(void){
  environmental_sensing_service.begin();
  uv_index_characteristic.setProperties(CHR_PROPS_INDICATE);
  uv_index_characteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  uv_index_characteristic.setFixedLen(4);
  uv_index_characteristic.begin();
  uv_index_characteristic.write(&uvindexvalue, 4);
}

void startAdv(void) {
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addName();
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);
  Bluefruit.Advertising.setFastTimeout(30);
  Bluefruit.Advertising.start();
}

void setup() {
  Serial.begin(115200);
  delay(50);
  Serial.println("Start!");

  Bluefruit.begin();
  Bluefruit.setName("Bluefruit52");

  setupESService();
  startAdv();

//////////////////////////////////////////////
// Accelerometer
   if (!sox.begin_I2C()) {
    Serial.println("Failed to find LSM6DSOX");
//    leadString += "\t Accelerometer not available (**Ignore following line**) \n";
  }
  Serial.println("LSM6DSOX Online!");
//  leadString += "\t Accelerometer Online! \n";
  Serial.print("Accelerometer range set to: ");
//  leadString += "\t \t Accelerometer set to: ";
  switch (sox.getAccelRange()) {
  case LSM6DS_ACCEL_RANGE_2_G:
    Serial.println("+-2G");
//    leadString += "+- 2G\t";
    break;
  case LSM6DS_ACCEL_RANGE_4_G:
    Serial.println("+-4G");
//    leadString += "+-4G \t";
    break;
  case LSM6DS_ACCEL_RANGE_8_G:
    Serial.println("+-8G");
//       leadString += "+-8G \t";
    break;
  case LSM6DS_ACCEL_RANGE_16_G:
    Serial.println("+-16G");
//    leadString += "+-16G \t";
    break;
  }

  // sox.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS );
  Serial.print("Gyro range set to: ");
//  leadString += "Gyro range set to: ";
  switch (sox.getGyroRange()) {
  case LSM6DS_GYRO_RANGE_125_DPS:
    Serial.println("125 degrees/s");
//    leadString += "125 degrees/s";
    break;
  case LSM6DS_GYRO_RANGE_250_DPS:
//    leadString += "250 degrees/s";
    break;
  case LSM6DS_GYRO_RANGE_500_DPS:
//    leadString += "500 degrees/s";
    break;
  case LSM6DS_GYRO_RANGE_1000_DPS:
//    leadString += "1000 degrees/s";
    break;
  case LSM6DS_GYRO_RANGE_2000_DPS:
    Serial.println("2000 degrees/s");
//    leadString += "2000 degrees/s";
    break;
  case ISM330DHCX_GYRO_RANGE_4000_DPS:
    break; // unsupported range for the DSOX
  }

  // sox.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
  Serial.print("Accelerometer data rate set to: ");
  switch (sox.getAccelDataRate()) {
  case LSM6DS_RATE_SHUTDOWN:
    Serial.println("0 Hz");
    break;
  case LSM6DS_RATE_12_5_HZ:
    Serial.println("12.5 Hz");
    break;
  case LSM6DS_RATE_26_HZ:
    Serial.println("26 Hz");
    break;
  case LSM6DS_RATE_52_HZ:
    Serial.println("52 Hz");
    break;
  case LSM6DS_RATE_104_HZ:
    Serial.println("104 Hz");
    break;
  case LSM6DS_RATE_208_HZ:
    Serial.println("208 Hz");
    break;
  case LSM6DS_RATE_416_HZ:
    Serial.println("416 Hz");
    break;
  case LSM6DS_RATE_833_HZ:
    Serial.println("833 Hz");
    break;
  case LSM6DS_RATE_1_66K_HZ:
    Serial.println("1.66 KHz");
    break;
  case LSM6DS_RATE_3_33K_HZ:
    Serial.println("3.33 KHz");
    break;
  case LSM6DS_RATE_6_66K_HZ:
    Serial.println("6.66 KHz");
    break;
  }

  // sox.setGyroDataRate(LSM6DS_RATE_12_5_HZ);
  Serial.print("Gyro data rate set to: ");
  switch (sox.getGyroDataRate()) {
  case LSM6DS_RATE_SHUTDOWN:
    Serial.println("0 Hz");
    break;
  case LSM6DS_RATE_12_5_HZ:
    Serial.println("12.5 Hz");
    break;
  case LSM6DS_RATE_26_HZ:
    Serial.println("26 Hz");
    break;
  case LSM6DS_RATE_52_HZ:
    Serial.println("52 Hz");
    break;
  case LSM6DS_RATE_104_HZ:
    Serial.println("104 Hz");
    break;
  case LSM6DS_RATE_208_HZ:
    Serial.println("208 Hz");
    break;
  case LSM6DS_RATE_416_HZ:
    Serial.println("416 Hz");
    break;
  case LSM6DS_RATE_833_HZ:
    Serial.println("833 Hz");
    break;
  case LSM6DS_RATE_1_66K_HZ:
    Serial.println("1.66 KHz");
    break;
  case LSM6DS_RATE_3_33K_HZ:
    Serial.println("3.33 KHz");
    break;
  case LSM6DS_RATE_6_66K_HZ:
    Serial.println("6.66 KHz");
    break;
  }
  
// Barometric Pressure Sensor
  if (!bmp.begin_I2C()) {
      Serial.println("Failed to find BMP sensor");
//      leadString += "\n \t Failed to find BMP sensor (**Ignore following line**) \n";
  }
  Serial.println("BMP Online");
//  leadString += "\t BMP Online! \n";

  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  //bmp.setOutputDataRate(BMP3_ODR_50_HZ);

//////////////////////////////////////////////
}

void loop() {
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  sox.getEvent(&accel, &gyro, &temp);
  uvindexvalue = temp.temperature*100.0;
  Serial.println(sizeof(uvindexvalue));
  if(uv_index_characteristic.indicate(&uvindexvalue, sizeof(uvindexvalue))){
    Serial.print("Updated Temp: " );
    Serial.println(uvindexvalue);
  }else {
    Serial.println("error, no receivers or no message sent");
  }
  delay(1000);
}


