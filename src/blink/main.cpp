#include <Arduino.h>
#define LED_PIN 15
const long interval = 1000; // 点滅間隔
unsigned long previousMillis = 0; // 最後にLEDの状態を変えた時間
int ledState = LOW; // LEDの状態

void setup() {
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    // 現在の時間を取得
    unsigned long currentMillis = millis();

    // 前回の実行からinterval以上経過したか？
    if (currentMillis - previousMillis >= interval) {
        // 時間を更新
        previousMillis = currentMillis;

        // LEDの状態を反転
        if (ledState == LOW) {
            ledState = HIGH;
        } else {
            ledState = LOW;
        }
        digitalWrite(LED_PIN, ledState);
    }

    // ここに他の処理を書ける！
}