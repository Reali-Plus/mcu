bool hapticUpdate = false;
const int NUMSENSORS = 8;

const int HEART_BEAT_PIN = 4;
const int GPIO0 = 6;
const int GPIO1 = 7;
const int GPIO2 = 8;
const int GPIO3 = 9;

byte hapticData = 0x00;

void updateHaptic() {
  Serial.println(hapticData, BIN);  

  digitalWrite(GPIO0, hapticData & 0b0001 ? LOW : HIGH);
  digitalWrite(GPIO1, hapticData & 0b0010 ? LOW : HIGH);
  digitalWrite(GPIO2, hapticData & 0b0100 ? LOW : HIGH);
  digitalWrite(GPIO3, hapticData & 0b1000 ? LOW : HIGH);
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
  Serial.println(mask, BIN);
  if (flag1) {
    hapticData |= mask & 0x0F;
  }
  else {
    hapticData &= ~(mask & 0x0F) ;
  }
}


void setup() {
  delay(1000);
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("Bon matin");

  // heart beat pin
  pinMode(HEART_BEAT_PIN, OUTPUT);
  pinMode(GPIO0, OUTPUT);
  pinMode(GPIO1, OUTPUT);
  pinMode(GPIO2, OUTPUT);
  pinMode(GPIO3, OUTPUT);
}

void loop() {
  if (Serial.available() >= 3) {  // Check if we have enough bytes (ID, 2 flags)
    char id = Serial.read();       // Read the ID character
    char flag1 = Serial.read();    // Read the first boolean character
    char flag2 = Serial.read();    // Read the second boolean character

    while(Serial.available()) Serial.read();

    Serial.print("Received Data ");
    Serial.println(id);
    // Convert characters to integers
    int intId = id - '0';          // Convert '0' to '9' to int 0 to 9
    bool boolFlag1 = flag1 - '0';  // Convert '0' or '1' to boolean
    bool boolFlag2 = flag2 - '0';
    updateHapticData(intId, boolFlag1, boolFlag2);
  }
  updateHaptic();
  delay(50);
}
