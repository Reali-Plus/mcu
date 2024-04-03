
#include <SPI.h>
#include <ICM20948_WE.h>


#include <Wire.h>


bool spi = true;

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>


#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


ICM20948_WE fingerIMU = ICM20948_WE(36, spi);
ICM20948_WE handIMU = ICM20948_WE(35, spi);

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
char message[64] = {};

void setup() {
  Serial.begin(115200);
  while(!Serial) {}
  pinMode(21, OUTPUT);
  pinMode(37, OUTPUT);
  pinMode(38, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(36, OUTPUT);
  digitalWrite(21, HIGH);
  digitalWrite(37, HIGH);
  digitalWrite(38, HIGH);
  digitalWrite(35, HIGH);
  digitalWrite(36, HIGH);

  delay(750);
  

  Serial.begin(115200);
  while(!Serial) {}
  
  if(!fingerIMU.init()){
    Serial.println("finger does not respond");
  }
  else{
    Serial.println("finger is connected");
  }
  delay(750);
  if(!fingerIMU.initMagnetometer()){
    Serial.println("fingerMagnetometer does not respond");
  }
  //TODO fix finger magnetometer
  else{
    Serial.println("fingerMagnetometer is connected");
  }
  delay(750);
  if(!handIMU.init()){
    Serial.println("hand does not respond");
  }
  else{
    Serial.println("hand is connected");
  }
  delay(750);
  if(!handIMU.initMagnetometer()){
    Serial.println("handMagnetometer does not respond");
  }
  else{
    Serial.println("handMagnetometer is connected");
  }

  
/******************* Basic Settings ******************/

  /* You can set the SPI clock speed. The default is 8 MHz. */ 
  fingerIMU.setSPIClockSpeed(8000000);
 

  //fingerIMU.setAccOffsets(-16330.0, 16450.0, -16600.0, 16180.0, -16520.0, 16690.0);
  
  
  Serial.println("Position your ICM20948 flat and don't move it - calibrating...");
  delay(1000);
  fingerIMU.autoOffsets();
  handIMU.autoOffsets();
  Serial.println("Done!"); 
  

  fingerIMU.setAccRange(ICM20948_ACC_RANGE_2G);
  

  fingerIMU.setAccDLPF(ICM20948_DLPF_6);    

  fingerIMU.setAccSampleRateDivider(1);

  fingerIMU.setGyrRange(ICM20948_GYRO_RANGE_500);

  fingerIMU.setGyrDLPF(ICM20948_DLPF_6);  

  fingerIMU.setGyrSampleRateDivider(10);

  fingerIMU.setTempDLPF(ICM20948_DLPF_6);

  fingerIMU.setMagOpMode(AK09916_CONT_MODE_20HZ);

  fingerIMU.setAccRange(ICM20948_ACC_RANGE_2G);
  
  
  handIMU.setAccDLPF(ICM20948_DLPF_6);    

  handIMU.setAccSampleRateDivider(1);

  handIMU.setGyrRange(ICM20948_GYRO_RANGE_500);

  handIMU.setGyrDLPF(ICM20948_DLPF_6);  

  handIMU.setGyrSampleRateDivider(10);

  handIMU.setTempDLPF(ICM20948_DLPF_6);

  handIMU.setMagOpMode(AK09916_CONT_MODE_20HZ);
  


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
  fingerIMU.readSensor();
  handIMU.readSensor();
  xyzFloat accF = fingerIMU.getGValues();
  xyzFloat gyrF = fingerIMU.getGyrValues();
  xyzFloat magF = fingerIMU.getMagValues();
  float tempF = fingerIMU.getTemperature();
  float resultantGF = fingerIMU.getResultantG(accF);
  xyzFloat accH = handIMU.getGValues();
  xyzFloat gyrH = handIMU.getGyrValues();
  xyzFloat magH = handIMU.getMagValues();
  float tempH = handIMU.getTemperature();
  float resultantGH = handIMU.getResultantG(accH);
  Serial.print("finger: ");
  Serial.print(accF.x);
  Serial.print("\n");
  Serial.print("hand: ");
  Serial.print(accH.x);
  Serial.print("\n");
  
  Serial.println("Acceleration in g (x,y,z):");
  Serial.print(accF.x);
  Serial.print("   ");
  Serial.print(accF.y);
  Serial.print("   ");
  Serial.println(accF.z);
/*
  Serial.println("Gyroscope data in degrees/s: ");
  Serial.print(gyrF.x);
  Serial.print("   ");
  Serial.print(gyrF.y);
  Serial.print("   ");
  Serial.println(gyrF.z);
*/
  /*Serial.println("Magnetometer Data in µTesla: ");
  Serial.print(mag.x);
  Serial.print("   ");
  Serial.print(mag.y);
  Serial.print("   ");
  Serial.println(mag.z);

  Serial.println("Temperature in °C: ");
  Serial.println(temp);*/

  sprintf(message, "%0.2f %0.2f %0.2f %0.2f %0.2f %0.2f",accF.x, accF.y, accF.z-1, gyrF.x, gyrF.y, gyrF.z);
  // sprintf(message, "%0.3f %0.3f %0.3f", acc.x, acc.y, acc.z);
  pCharacteristic->setValue(message);
  delay(150);
}
