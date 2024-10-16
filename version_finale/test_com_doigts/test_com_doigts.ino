#include <SPI.h>
#include <ICM20948_WE.h>
#include <Wire.h>

bool spi = true;

const int NUMSENSORS = 8;
const int MSG_BUFFER_SIZE = 34;

const int HEART_BEAT_PIN = 5;

float sensorData[48];

ICM20948_WE IMU_AVANT_BRAS = ICM20948_WE(37,spi);
ICM20948_WE IMU_MAIN = ICM20948_WE(35,spi);
ICM20948_WE IMU_EPAULE = ICM20948_WE(38,spi);
ICM20948_WE IMU_AURI = ICM20948_WE(36,spi);
ICM20948_WE IMU_ANNU = ICM20948_WE(21,spi);
ICM20948_WE IMU_MAJEUR = ICM20948_WE(35,spi);
ICM20948_WE IMU_INDEX = ICM20948_WE(35,spi);
ICM20948_WE IMU_POUCE = ICM20948_WE(35,spi);

ICM20948_WE sensors[NUMSENSORS] = {
  IMU_AVANT_BRAS, IMU_MAIN, IMU_EPAULE, IMU_AURI, IMU_ANNU, IMU_MAJEUR, IMU_INDEX, IMU_POUCE
};


char message[64] = {};
void update_mux(int id){
  switch(id){
    case 0:
      //avant-bras -- logic
      digitalWrite(21, HIGH);
      digitalWrite(37, LOW);
      digitalWrite(38, HIGH);
      digitalWrite(35, HIGH);
      digitalWrite(36, HIGH);
    break;

    case 1:
      //main
      digitalWrite(21, HIGH);
      digitalWrite(37, HIGH);
      digitalWrite(38, HIGH);
      digitalWrite(35, LOW);
      digitalWrite(36, HIGH);
    break;

    case 2:
      //épaule
      digitalWrite(21, HIGH);
      digitalWrite(37, HIGH);
      digitalWrite(38, LOW);
      digitalWrite(35, HIGH);
      digitalWrite(36, HIGH);
    break;

    case 3:
      //auriculaire
      digitalWrite(21, HIGH);
      digitalWrite(37, HIGH);
      digitalWrite(38, HIGH);
      digitalWrite(35, HIGH);
      digitalWrite(36, LOW);
    break;

    case 4:
      //annulaire
      digitalWrite(21, LOW);
      digitalWrite(37, HIGH);
      digitalWrite(38, HIGH);
      digitalWrite(35, HIGH);
      digitalWrite(36, LOW);
    break;

    case 5:
      //majeur
      digitalWrite(21, HIGH);
      digitalWrite(37, HIGH);
      digitalWrite(38, HIGH);
      digitalWrite(35, LOW);
      digitalWrite(36, LOW);
    break;

    case 6:
      //index
      digitalWrite(21, LOW);
      digitalWrite(37, HIGH);
      digitalWrite(38, HIGH);
      digitalWrite(35, LOW);
      digitalWrite(36, LOW);
    break;

    case 7:
      //pouce
      digitalWrite(21, LOW);
      digitalWrite(37, HIGH);
      digitalWrite(38, HIGH);
      digitalWrite(35, LOW);
      digitalWrite(36, HIGH);
    break;

    default:
      digitalWrite(21, HIGH);
      digitalWrite(37, HIGH);
      digitalWrite(38, HIGH);
      digitalWrite(35, HIGH);
      digitalWrite(36, HIGH);
    break;
    
    }
}
void setup() {
  Serial.begin(115200);
  while(!Serial) {}

  // heart beat pin 
  pinMode(HEART_BEAT_PIN, OUTPUT);


  pinMode(21, OUTPUT);
  pinMode(37, OUTPUT);
  pinMode(38, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(36, OUTPUT);

  delay(200);
  

  Serial.begin(115200);
  while(!Serial) {}


  for(int i = 0; i < NUMSENSORS; i++){
    update_mux(i);
    if(!sensors[i].init()){

    Serial.println((String)"| finger " + i + " does not respond"); 
    digitalWrite(HEART_BEAT_PIN, HIGH);
    delay(100);
    digitalWrite(HEART_BEAT_PIN, LOW);
    delay(100);
    }
    else{
      Serial.println((String)"| finger " + i + " is connected");
    }
    
    delay(200);
    /******************* Basic Settings ******************/
    
    /* You can set the SPI clock speed. The default is 8 MHz. */ 
    sensors[i].setSPIClockSpeed(4000000);
    
    Serial.println("| Position your ICM20948 flat and don't move it - calibrating...");
    delay(200);
    sensors[i].autoOffsets();
    Serial.println("| Done!"); 
    
    sensors[i].setAccRange(ICM20948_ACC_RANGE_2G);
    sensors[i].setAccDLPF(ICM20948_DLPF_6);    
    sensors[i].setAccSampleRateDivider(1);
    sensors[i].setGyrRange(ICM20948_GYRO_RANGE_500);
    sensors[i].setGyrDLPF(ICM20948_DLPF_6);  
    sensors[i].setGyrSampleRateDivider(10);
    sensors[i].setTempDLPF(ICM20948_DLPF_6);
    sensors[i].setMagOpMode(AK09916_CONT_MODE_20HZ);
    
  }
}  

void loop() {
  for(int i = 0; i < NUMSENSORS; i++){
    update_mux(i);
    sensors[i].readSensor();
    xyzFloat acc = sensors[i].getGValues();
    xyzFloat gyr = sensors[i].getGyrValues();
    //sensorData[i*6]   = 0.0;//acc.x;  // Acceleration in x-axis for sensorIndex
    //sensorData[i*6+1] = 1.0;//acc.y;  // Acceleration in y-axis for sensorIndex
    //sensorData[i*6+2] = 2.0;//acc.z;  // Acceleration in z-axis for sensorIndex
    //sensorData[i*6+3] = 4.0;//gyr.x;  // Angular speed in x-axis for sensorIndex
    //sensorData[i*6+4] = 8.0;//gyr.y;  // Angular speed in y-axis for sensorIndex
    //sensorData[i*6+5] = 16.0;//gyr.z;  // Angular speed in z-axis for sensorIndex
    
    sprintf(message,"%d %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f", i, acc.x, acc.y, acc.z, gyr.x, gyr.y, gyr.z);
    //sprintf(message+i*MSG_BUFFER_SIZE,"%d04 %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f", i, acc.x, acc.y, acc.z, gyr.x, gyr.y, gyr.z);
    Serial.println(message);
    // Serial.write(sensorData[i])
  }
  delay(50);
  //Serial.println(message);
  //Serial.println(sizeof(sensorData));

  // for (int i = 0; i < 48; i++) {
  //     // Get the current float value
  //     float value = sensorData[i];

  //     // Convert the float into bytes and send each byte
  //     byte *bytePointer = (byte*) &value;  // Reinterpret the float as an array of 4 bytes
  //     for (int j = 0; j < 4; j++) {
  //       Serial.write(bytePointer[j]);  // Send each byte of the float
  //     }
  // }
}
