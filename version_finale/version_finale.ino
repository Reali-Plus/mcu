/***************************************************************************
* Example sketch for the ICM20948_WE library
*
* This sketch shows how to use SPI to retrieve accelerometer, gyroscope, 
* temperature and magnetometer data from the ICM20948.
* 
* Further information can be found on:
*
* https://wolles-elektronikkiste.de/icm-20948-9-achsensensor-teil-i (German)
* https://wolles-elektronikkiste.de/en/icm-20948-9-axis-sensor-part-i (English)
* 
***************************************************************************/

#include <SPI.h>
#include <ICM20948_WE.h>
#define CS_PIN 35   // Chip Select Pin

#define CS_A  21
#define CS_B  35
#define CS_C  36

bool spi = true;

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>


#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


ICM20948_WE myIMU = ICM20948_WE(CS_PIN, spi);

BLEServer *pServer;
BLEService *pService; 
BLECharacteristic *pCharacteristic; 
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        Serial.println("*********");
        Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++)
          Serial.print(value[i]);

        Serial.println();
        Serial.println("*********");
      }
    }
};
char message[32] = {};

void setup() {
  pinMode(CS_A, OUTPUT);
  pinMode(CS_C, OUTPUT);
  digitalWrite(CS_A, HIGH);
  digitalWrite(CS_C, HIGH);

  Serial.begin(115200);
  while(!Serial) {}
  
  if(!myIMU.init()){
    Serial.println("ICM20948 does not respond");
  }
  else{
    Serial.println("ICM20948 is connected");
  }

  if(!myIMU.initMagnetometer()){
    Serial.println("Magnetometer does not respond");
  }
  else{
    Serial.println("Magnetometer is connected");
  }

  
/******************* Basic Settings ******************/

  /* You can set the SPI clock speed. The default is 8 MHz. */ 
  myIMU.setSPIClockSpeed(8000000);
 

  //myIMU.setAccOffsets(-16330.0, 16450.0, -16600.0, 16180.0, -16520.0, 16690.0);
    
  
  Serial.println("Position your ICM20948 flat and don't move it - calibrating...");
  delay(1000);
  myIMU.autoOffsets();
  Serial.println("Done!"); 
  

  myIMU.setAccRange(ICM20948_ACC_RANGE_2G);
  

  myIMU.setAccDLPF(ICM20948_DLPF_6);    

  myIMU.setAccSampleRateDivider(1);

  myIMU.setGyrRange(ICM20948_GYRO_RANGE_500);

  myIMU.setGyrDLPF(ICM20948_DLPF_6);  

  myIMU.setGyrSampleRateDivider(10);

  myIMU.setTempDLPF(ICM20948_DLPF_6);

  myIMU.setMagOpMode(AK09916_CONT_MODE_20HZ);



  BLEDevice::init("MyESP32");
  
  pServer = BLEDevice::createServer();
  pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );


  pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->setValue("Hello World");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void loop() {
  myIMU.readSensor();
  xyzFloat acc = myIMU.getGValues();
  xyzFloat gyr = myIMU.getGyrValues();
  xyzFloat mag = myIMU.getMagValues();
  float temp = myIMU.getTemperature();
  float resultantG = myIMU.getResultantG(acc);
  
  Serial.println("Acceleration in g (x,y,z):");
  Serial.print(acc.x);
  Serial.print("   ");
  Serial.print(acc.y);
  Serial.print("   ");
  Serial.println(acc.z);

  Serial.println("Gyroscope data in degrees/s: ");
  Serial.print(gyr.x);
  Serial.print("   ");
  Serial.print(gyr.y);
  Serial.print("   ");
  Serial.println(gyr.z);

  Serial.println("Magnetometer Data in µTesla: ");
  Serial.print(mag.x);
  Serial.print("   ");
  Serial.print(mag.y);
  Serial.print("   ");
  Serial.println(mag.z);

  Serial.println("Temperature in °C: ");
  Serial.println(temp);

  sprintf(message, "%0.3f %0.3f %0.3f", gyr.x, gyr.y, gyr.z);
  // sprintf(message, "%0.3f %0.3f %0.3f", acc.x, acc.y, acc.z);
  pCharacteristic->setValue(message);
  delay(100);
}
