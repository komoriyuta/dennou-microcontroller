#include <Arduino.h>
#include <Wire.h>
#include <MPU6050_tockn.h>
#define SDA_PIN 14
#define SCL_PIN 15
MPU6050 mpu6050(Wire1);

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing I2C...");
  pinMode(SDA_PIN, INPUT_PULLUP);
  pinMode(SCL_PIN, INPUT_PULLUP);
  Wire1.setSDA(SDA_PIN);
  Wire1.setSCL(SCL_PIN);
  Wire1.begin();
  mpu6050.begin();
  Serial.println("MPU6050 initialized");
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