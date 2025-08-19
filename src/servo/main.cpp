#include <Arduino.h>
#include <Servo.h>

#define SERVO_PIN 20
Servo myservo;


void setup(){
    myservo.attach(SERVO_PIN);
}

void loop(){
    myservo.write(90); // Set servo to 90 degrees
    delay(1000);
    myservo.write(0); // Set servo to 0 degrees
    delay(1000);
}