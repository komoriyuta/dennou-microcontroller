#include <Arduino.h>
#include <Wire.h>
#include <MPU6050_tockn.h>
#include <Servo.h>
#define SERVO_PIN 20

MPU6050 mpu6050(Wire);
Servo myservo;

// 調整用パラメータ
const float gain = 1.5; // Pゲイン（反応の強さ）。この値を調整する。

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  myservo.attach(SERVO_PIN);
}

void loop() {
  mpu6050.update();

  // 1. MPU-6050から現在の傾斜角度を取得する (Y軸回りの傾きを取得)
  float currentAngle = mpu6050.getAngleY();

  // 2. 目標角度(0度)との誤差を計算する
  float error = 0 - currentAngle;

  // 3. 誤差に基づいてサーボの目標角度を決定する
  float servoAngle = 90 + error * gain;

  // 4. サーボの角度を制限する (0〜180度の範囲に収める)
  servoAngle = constrain(servoAngle, 0, 180);

  // 5. サーボに角度を指示する
  myservo.write(servoAngle);

  delay(10);
}