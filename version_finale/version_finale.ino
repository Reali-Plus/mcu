
#include <SPI.h>
#include <ICM20948_WE.h>


#include <Wire.h>


bool spi = true;

const int NUMSENSORS = 8;

ICM20948_WE IMU0 = ICM20948_WE(37,spi);
ICM20948_WE IMU1 = ICM20948_WE(35,spi);
ICM20948_WE IMU2 = ICM20948_WE(38,spi);
ICM20948_WE IMU3 = ICM20948_WE(36,spi);
ICM20948_WE IMU4 = ICM20948_WE(21,spi);
ICM20948_WE IMU5 = ICM20948_WE(35,spi);
ICM20948_WE IMU6 = ICM20948_WE(35,spi);
ICM20948_WE IMU7 = ICM20948_WE(35,spi);

ICM20948_WE sensors[NUMSENSORS] = {
  IMU0, IMU1, IMU2, IMU3, IMU4, IMU5, IMU6, IMU7
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
    Serial.println("| finger does not respond");
    }
    else{
      Serial.println("| finger is connected");
    }
    
    delay(200);
    /******************* Basic Settings ******************/
    
    /* You can set the SPI clock speed. The default is 8 MHz. */ 
    sensors[i].setSPIClockSpeed(8000000);
    
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
    sprintf(message, "%d %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f", i, acc.x, acc.y, acc.z-1, gyr.x, gyr.y, gyr.z);
    Serial.println(message);
  }

}
