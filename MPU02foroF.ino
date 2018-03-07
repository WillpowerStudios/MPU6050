#include <Wire.h> 

long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ; 

long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;

void setup() {
  Serial.begin(9600);
  
  // initialize i2c communication
  Wire.begin();
  setupMPU();
}

void loop() {
  recordAccelRegisters();
  recordGyroRegisters();
  printData();
  delay(100); 
}

// establish communication with the MPU / i2c 
// setup all the registers from which we'll retrieve data from the MPU into arduino
void setupMPU() {
  Wire.beginTransmission(0b1101000); // I2C address of the MPU
  Wire.write(0x6B); // Accessing register 6B - Power Management (sec. 4.28)
  Wire.write(0b00000000); // making sure we're not in sleep mode > setting sleep register to 0
  Wire.endTransmission();
  // lets setup gyroscope parameters 
  Wire.beginTransmission(0b1101000); // I2C address of the MPU
  Wire.write(0x00000000); // setting up gyro to full scale range +/- 2g
  Wire.endTransmission(); 
  // lets setup acceleromter parameters 
  Wire.beginTransmission(0b1101000); // I2C address of the MPU
  Wire.write(0b00000000); // setting the accel to +/- 2g 
  Wire.endTransmission();
}

void recordAccelRegisters() {
  Wire.beginTransmission(0b1101000); // I2C address of the MPU
  Wire.write(0x3B); // Accessing register 3B for accel readings 
  Wire.endTransmission(); 
  Wire.requestFrom(0b1101000, 6); // request accel registers 
  while (Wire.available() < 6);
  accelX = Wire.read()<<8|Wire.read(); // store first 2 bytes in accelX
  accelY = Wire.read()<<8|Wire.read(); // store middle 2 bytes in accelY
  accelZ = Wire.read()<<8|Wire.read(); // store last 2 bytes in accelZ
  processAccelData();
}

// get acceleration in gForce
void processAccelData() {
  gForceX = accelX / 16384.0;
  gForceY = accelY / 16384.0;
  gForceZ = accelZ / 16384.0;
}

void recordGyroRegisters() {
  Wire.beginTransmission(0b1101000); // I2C address of the MPU
  Wire.write(0x43); // starting register for gyro readings 
  Wire.endTransmission(); 
  Wire.requestFrom(0b1101000, 6); // request accel registers 
  while (Wire.available() < 6); 
  gyroX = Wire.read()<<8|Wire.read(); // store the first 2 bytes into gyroX
  gyroY = Wire.read()<<8|Wire.read(); // store the middle 2 bytes into gyroY
  gyroZ = Wire.read()<<8|Wire.read(); // store the last 2 bytes into gyroZ 
  processGyroData();
}

// get rotation in degrees
void processGyroData() {
    rotX = gyroX / 131.0;
    rotY = gyroY / 131.0;
    rotZ = gyroZ / 131.0;
}

void printData() {
  Serial.print("X: ");
  Serial.println(rotX);
  Serial.print("Y: ");
  Serial.println(rotY);
  Serial.print("Z: ");
  Serial.println(rotZ);
  Serial.print("Gx: ");
  Serial.println(gForceX);
  Serial.print("Gy: ");
  Serial.println(gForceY);
  Serial.print("Gz: ");
  Serial.println(gForceZ);
}

