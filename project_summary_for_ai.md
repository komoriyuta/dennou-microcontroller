# Project Summary for AI

## 1. Problem Description

The PlatformIO project fails to build any of its environments (e.g., `blink`). The build process consistently fails with a linker error: `undefined reference to 'setup'` and `undefined reference to 'loop'`.

This indicates that the source code for the selected environment (e.g., `src/blink/main.ino`) is not being compiled and linked.

The `platformio.ini` file has been configured to use multiple environments, with each environment's source code located in a separate subdirectory under `src/`. Despite trying several standard configuration approaches (`src_dir` in each environment, `[env]` inheritance, `build_src_filter`), the problem persists, suggesting a fundamental issue with how the build system is interpreting the configuration file.

The latest attempt at `platformio.ini` uses a fully explicit configuration where each environment defines all its settings independently, but this has also failed.

## 2. `platformio.ini`

```ini
[platformio]
default_envs = blink

[env:blink]
platform = https://github.com/maxgerhardt/platform-raspberrypi.git
board = pico
framework = arduino
board_build.core = earlephilhower
upload_protocol = picotool
monitor_speed = 115200
src_dir = src/blink

[env:servo]
platform = https://github.com/maxgerhardt/platform-raspberrypi.git
board = pico
framework = arduino
board_build.core = earlephilhower
upload_protocol = picotool
monitor_speed = 115200
src_dir = src/servo
lib_deps =
    arduino-libraries/Servo

[env:mpu6050]
platform = https://github.com/maxgerhardt/platform-raspberrypi.git
board = pico
framework = arduino
board_build.core = earlephilhower
upload_protocol = picotool
monitor_speed = 115200
src_dir = src/mpu6050
lib_deps =
    tockn/MPU6050_tockn

[env:reflector]
platform = https://github.com/maxgerhardt/platform-raspberrypi.git
board = pico
framework = arduino
board_build.core = earlephilhower
upload_protocol = picotool
monitor_speed = 115200
src_dir = src/reflector

[env:self_balancing_platform]
platform = https://github.com/maxgerhardt/platform-raspberrypi.git
board = pico
framework = arduino
board_build.core = earlephilhower
upload_protocol = picotool
monitor_speed = 115200
src_dir = src/self_balancing_platform
lib_deps =
    tockn/MPU6050_tockn
    arduino-libraries/Servo

[env:capacitive_touch]
platform = https://github.com/maxgerhardt/platform-raspberrypi.git
board = pico
framework = arduino
board_build.core = earlephilhower
upload_protocol = picotool
monitor_speed = 115200
src_dir = src/capacitive_touch
```

## 3. Source Code

### `src/blink/main.ino`

```cpp
#include <Arduino.h>
#define LED_PIN 13
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
```

### `src/capacitive_touch/main.ino`

```cpp
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
```

### `src/mpu6050/main.ino`

```cpp
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
```

### `src/reflector/main.ino`

```cpp
#include <Arduino.h>
#define SENSOR_PIN 26 // フォトリフレクタのOUTを接続するピン (ADC0)

void setup() {
    Serial.begin(115200); // シリアル通信の開始
    // アナログピンは pinMode の設定は不要
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
    int sensorValue = readAverage(SENSOR_PIN, 10); // 10回の測定値の平均を取る

    // 読み取った値をシリアルモニタに表示
    Serial.print("Sensor Value: ");
    Serial.println(sensorValue);

    delay(500); // 0.5秒待機
}
```

### `src/self_balancing_platform/main.ino`

```cpp
#include <Arduino.h>
#include <Wire.h>
#include <MPU6050_tockn.h>
#include <Servo.h>

MPU6050 mpu6050(Wire);
Servo myservo;

// 調整用パラメータ
const float gain = 1.5; // Pゲイン（反応の強さ）。この値を調整する。
const int servoPin = 15;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  myservo.attach(servoPin);
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
```

### `src/servo/main.ino`

```cpp
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
```
