#include <Arduino.h>
#define SENSOR_PIN 26 // 可変抵抗器の出力を接続するピン (ADC0)

void setup() {
    Serial.begin(115200); // シリアル通信の開始
    analogReadResolution(12); // ADCの解像度を12ビットに設定 (0〜4095)
}

long readAverage(int pin, int samples) {
    long sum = 0;
    for (int i = 0; i < samples; i++) {
        sum += analogRead(pin);
        delay(1); // わずかな時間をおいてサンプリング
    }
    return sum / samples;
}

void loop() {
    // analogReadでセンサーの値を読み取る (0〜4095の範囲)
    int sensorValue = analogRead(SENSOR_PIN);

    // 読み取った値をシリアルモニタに表示
    Serial.print("Sensor Value: ");
    Serial.println(sensorValue);

    delay(500); // 0.5秒待機
}