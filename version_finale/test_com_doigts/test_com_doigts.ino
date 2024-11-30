#include <SPI.h>
#include <ICM20948_WE.h>
#include <Wire.h>

#define PCF8574_ADDRESS 0x20

#define LOOP_DELAY 50
#define HAPTIC_DELAY 1000
#define SKIP_CALIBRATION false


bool spi = true;
bool hapticUpdate = false;
const int NUMSENSORS = 8;
const int MSG_BUFFER_SIZE = 34;

const int HEART_BEAT_PIN = 5;

float sensorData[48];
byte hapticData = 0x00;

int hapticResetCounter = 0;

void updateHaptic() {
  Wire.beginTransmission(PCF8574_ADDRESS);
  Wire.write(~(hapticData));  // Write the byte to the PCF8574
  Wire.endTransmission();  // End the transmission
}

void updateHapticData(int id, bool flag1, bool flag2) {

  byte mask = 0b00000000;
  switch (id) {

    case 3:
      mask = 0b10001000;
      break;

    case 4:
      mask = 0b01000100;
      break;

    case 5:
      mask = 0b00100010;
      break;

    case 6:
      mask = 0b00010001;
      break;

    default:
      mask = 0b00000000;
      break;

  }
  if (flag1) {
    hapticData |= mask & 0x0F ;
  }
  else {
    hapticData &= ~(mask & 0x0F) ;
  }
  if (flag2) {
    hapticData |= mask & 0xF0 ;
  }
  else {
    hapticData &= ~(mask & 0xF0) ;
  }

}


ICM20948_WE IMU_AVANT_BRAS = ICM20948_WE(37, spi);
ICM20948_WE IMU_MAIN = ICM20948_WE(35, spi);
ICM20948_WE IMU_EPAULE = ICM20948_WE(38, spi);
ICM20948_WE IMU_AURI = ICM20948_WE(36, spi);
ICM20948_WE IMU_ANNU = ICM20948_WE(36, spi);
ICM20948_WE IMU_MAJEUR = ICM20948_WE(36, spi);
ICM20948_WE IMU_INDEX = ICM20948_WE(35, spi);
ICM20948_WE IMU_POUCE = ICM20948_WE(35, spi);

ICM20948_WE sensors[NUMSENSORS] = {
  IMU_AVANT_BRAS, IMU_MAIN, IMU_EPAULE, IMU_INDEX, IMU_MAJEUR, IMU_ANNU , IMU_AURI, IMU_POUCE
};

char message[64] = {};
char messageDebug[64] = {};
void update_mux(int id) {
  switch (id) {
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
      //index
      digitalWrite(21, LOW);
      digitalWrite(37, HIGH);
      digitalWrite(38, HIGH);
      digitalWrite(35, LOW);
      digitalWrite(36, LOW);
      break;

    case 4:
      //majeur
      digitalWrite(21, HIGH);
      digitalWrite(37, HIGH);
      digitalWrite(38, HIGH);
      digitalWrite(35, LOW);
      digitalWrite(36, LOW);
      break;

    case 5:
      //annulaire
      digitalWrite(21, LOW);
      digitalWrite(37, HIGH);
      digitalWrite(38, HIGH);
      digitalWrite(35, HIGH);
      digitalWrite(36, LOW);
      break;

    case 6:
      //auriculaire
      digitalWrite(21, HIGH);
      digitalWrite(37, HIGH);
      digitalWrite(38, HIGH);
      digitalWrite(35, HIGH);
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
      digitalWrite(21, LOW);
      digitalWrite(37, HIGH);
      digitalWrite(38, HIGH);
      digitalWrite(35, HIGH);
      digitalWrite(36, HIGH);
      break;

  }
}
void setup() {
  delay(2000);
  Serial.begin(115200);
  Wire.begin(17, 18);
  updateHaptic();
  while (!Serial) {}

  // heart beat pin
  pinMode(HEART_BEAT_PIN, OUTPUT);


  pinMode(21, OUTPUT);
  pinMode(37, OUTPUT);
  pinMode(38, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(36, OUTPUT);

  delay(500);

  if(SKIP_CALIBRATION)
    return;

  int i = 0;
  initloop:
  for (; i < NUMSENSORS; i++) {
    Serial.println("---------------");
    Serial.println(i);

    delay(300);
    update_mux(i);
    int attempts = 0;
    bool response = false;
    do {
      response = sensors[i].init();
      attempts++;
    }

    while (!response && attempts < 5);

    if (attempts >= 5)
    {
      Serial.println("AAAAA");
      i++;
      goto vachier;
    }



    delay(500);
    /******************* Basic Settings ******************/

    /* You can set the SPI clock speed. The default is 8 MHz. */

    sensors[i].setSPIClockSpeed(1000000);

    Serial.println("| Position your ICM20948 flat and don't move it - calibrating...");
    delay(800);
    sensors[i].autoOffsets();
    delay(800);
    Serial.println("| Done!");
    sensors[i].setAccRange(ICM20948_ACC_RANGE_2G);
    sensors[i].setAccDLPF(ICM20948_DLPF_1);
    sensors[i].setAccSampleRateDivider(1);
    sensors[i].setGyrRange(ICM20948_GYRO_RANGE_250);
    sensors[i].setGyrDLPF(ICM20948_DLPF_1);
    sensors[i].setGyrSampleRateDivider(1);
    delay(100);
    SPI.end();
  }
  SPI.begin();
  return;

  vachier:
  SPI.end();
  goto initloop;
}

void loop() {
  for (int i = 0; i < NUMSENSORS; i++) {
    update_mux(i);
    sensors[i].readSensor();
    xyzFloat acc = sensors[i].getGValues();
    xyzFloat gyr = sensors[i].getGyrValues();

    sprintf(message, "%d %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f", i, acc.x, acc.y, acc.z, gyr.x, gyr.y, gyr.z);
    Serial.println(message);
  }
  if (Serial.available() >= 3) {  // Check if we have enough bytes (ID, 2 flags)
    char id = Serial.read();       // Read the ID character
    char flag1 = Serial.read();    // Read the first boolean character
    char flag2 = Serial.read();    // Read the second boolean character

    while(Serial.available()) Serial.read();

    // Convert characters to integers
    int intId = id - '0';          // Convert '0' to '9' to int 0 to 9
    bool boolFlag1 = flag1 - '0';  // Convert '0' or '1' to boolean
    bool boolFlag2 = flag2 - '0';

    if (intId == 3 || intId == 4 || intId == 5 || intId == 6)
    {
      updateHapticData(3, boolFlag1, boolFlag2);
      updateHapticData(4, boolFlag1, boolFlag2);
      updateHapticData(5, boolFlag1, boolFlag2);
      updateHapticData(6, boolFlag1, boolFlag2);
    }
    else
    {
      updateHapticData(intId, boolFlag1, boolFlag2);
    }

    hapticResetCounter = 0;
  }

  updateHaptic();
  delay(LOOP_DELAY);

  if(hapticResetCounter++ >= HAPTIC_DELAY / LOOP_DELAY)
  {
    updateHapticData(3, false, false);
    updateHapticData(4, false, false);
    updateHapticData(5, false, false);
    updateHapticData(6, false, false);
  }
}
