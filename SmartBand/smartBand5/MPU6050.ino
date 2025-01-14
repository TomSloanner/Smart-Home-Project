#include <Wire.h>


byte a = 0b00000000;
long temp;
float tempC;

void setupMPU() {

  Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(a); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4)
  Wire.write(a); //Setting the gyro to full scale +/- 250deg./s
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5)
  Wire.write(a); //Setting the accel to +/- 2g
  Wire.endTransmission();
}

void recordAccelRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000, 6); //Request Accel Registers (3B - 40)
  while (Wire.available() < 6);
  accelX = Wire.read() << 8 | Wire.read(); //Store first two bytes into accelX
  accelY = Wire.read() << 8 | Wire.read(); //Store middle two bytes into accelY
  accelZ = Wire.read() << 8 | Wire.read(); //Store last two bytes into accelZ
  processAccelData();
}

void processAccelData() {
  gForceX = accelX / 16384.0 - 0.05;
  gForceY = accelY / 16384.0 - 0.035;
  gForceZ = accelZ / 16384.0 - 0.83;
  velocity();
}
void velocity() {
  
  timeBetweenAccelRead = millis() - lastReadingTime;
  lastReadingTime = millis();
  velX = velX + gForceX * timeBetweenAccelRead;
  velY = velY + gForceY * timeBetweenAccelRead;
  velZ = velZ + gForceZ * timeBetweenAccelRead;
  printData();
}

void recordGyroRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000, 6); //Request Gyro Registers (43 - 48)
  while (Wire.available() < 6);
  gyroX = Wire.read() << 8 | Wire.read(); //Store first two bytes into accelX
  gyroY = Wire.read() << 8 | Wire.read(); //Store middle two bytes into accelY
  gyroZ = Wire.read() << 8 | Wire.read(); //Store last two bytes into accelZ
  processGyroData();
}

void processGyroData() {
  rotX = gyroX / 131.0;
  rotY = gyroY / 131.0;
  rotZ = gyroZ / 131.0;
}

void recordTemperatureRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x41); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000, 2); //Request Gyro Registers (43 - 48)
  while (Wire.available() < 2);
  temp = Wire.read() << 8 | Wire.read(); //Store first two bytes into temp
  precessTemperatureData();
}

void precessTemperatureData() {
  tempC = temp / 340 + 36.53;
}
void printData() {
  //  Serial.print("Gyro (deg)");
  //  Serial.print(" X=");
  //  Serial.print(rotX);
  //  Serial.print(" Y=");
  //  Serial.print(rotY);
  //  Serial.print(" Z=");
  //    Serial.print(rotZ);
  //  Serial.print(" Accel (g)");
  //  Serial.print(" \t X=");
  //  Serial.print(gForceX);
  //  Serial.print("\t  Y=");
  //  Serial.print(gForceY);
  //  Serial.print(" \t Z=");
  //  Serial.println(gForceZ);
  Serial.print(" Vel (g)");
  Serial.print(" \t X=");
  Serial.print(velX);
  Serial.print("\t  Y=");
  Serial.print(velY);
  Serial.print(" \t Z=");
  Serial.println(velZ);
  //  Serial.print(" Temp= ");
  //  Serial.println(tempC);
}
