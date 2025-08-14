#include <Arduino.h>
#include <Servo.h>

Servo myservo;
const int servoPin = 15;
const int analogPin = 26; // アナログ入力ピン

void setup() {
    myservo.attach(servoPin);
    // アナログピンはpinMode不要
}

void loop() {
    // アナログ値を読み取る (0-4095)
    int sensorValue = analogRead(analogPin);

    // センサーの値をサーボの角度(0-180)に変換する
    // map(value, fromLow, fromHigh, toLow, toHigh)
    int angle = map(sensorValue, 0, 4095, 0, 180);

    // サーボに角度を指示
    myservo.write(angle);
    delay(15);
}