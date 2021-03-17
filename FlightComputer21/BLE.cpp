// Transmitt over bluetooth reveiving onto Chrome

#include <bluefruit.h>

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