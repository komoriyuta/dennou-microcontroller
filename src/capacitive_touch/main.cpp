#include <Arduino.h>

#define SEND_PIN 14
#define RECEIVE_PIN 15
#define THRESHOLD 100 // この閾値は環境によって要調整

void setup() {
    Serial.begin(115200);
}

void loop() {
    long duration = readCapacitivePin();

    Serial.print("Duration: ");
    Serial.print(duration);

    if (duration > THRESHOLD) {
        Serial.println(" - Touched!");
    } else {
        Serial.println("");
    }
    delay(100);
}

long readCapacitivePin() {
    // 1. RECEIVEピンを出力LOWにして、コンデンサを放電させる
    pinMode(RECEIVE_PIN, OUTPUT);
    digitalWrite(RECEIVE_PIN, LOW);
    delay(1); // 放電を確実にする

    // 2. SENDピンを出力HIGHにして、充電を開始する準備
    pinMode(SEND_PIN, OUTPUT);
    digitalWrite(SEND_PIN, HIGH);

    // 3. RECEIVEピンを入力モードに戻す（ここから充電時間測定開始）
    pinMode(RECEIVE_PIN, INPUT);

    // 4. 時間測定開始
    long startTime = micros();
    
    // 5. RECEIVEピンがHIGHになるまで待つ
    while(digitalRead(RECEIVE_PIN) == LOW) {
        // タイムアウト処理を入れるのが望ましいが、ここでは省略
    }

    long endTime = micros();

    // 6. SENDピンを低インピーダンス状態にして、次の測定に影響が出ないようにする
    pinMode(SEND_PIN, INPUT);

    return endTime - startTime;
}
