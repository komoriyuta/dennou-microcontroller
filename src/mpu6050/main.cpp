#include <Arduino.h>
#include <Wire.h>
#include <MPU6050_tockn.h>

MPU6050 mpu6050(Wire);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true); // ジャイロのオフセットをキャリブレーション
}

void loop() {
  mpu6050.update(); // センサー値を更新

  Serial.print("angleX: ");
  Serial.print(mpu6050.getAngleX());
  Serial.print("\tangleY: ");
  Serial.print(mpu6050.getAngleY());
  Serial.print("\tangleZ: ");
  Serial.println(mpu6050.getAngleZ());
  
  delay(10);
}