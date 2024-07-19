/*
* @file: src/Experiment2.cpp
* @author: donghy23@mails.tsinghua.edu.cn
* @repo: https://github.com/FHYQ-Dong/Tsinghua-Hardware-Design-Assignment/blob/main/Experiment2/src/Experiment2.cpp
* @date: 2024.7.12
*/

#include <Arduino.h>
#include <Servo.h>

int trigPin = 11, ecoPin = 12;
int ledR = A0, ledG = A1, ledB = A2;
float dis;
Servo MyServo;

void setup() {
    Serial.begin(9600);
    pinMode(trigPin, OUTPUT);
    pinMode(ecoPin, INPUT);
    pinMode(ledR, OUTPUT);
    pinMode(ledG, OUTPUT);
    pinMode(ledB, OUTPUT);
    digitalWrite(ledG, HIGH);
    digitalWrite(ledR, LOW);
    digitalWrite(ledB, LOW);
    MyServo.attach(7);
    MyServo.write(0);
}

void loop() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(8);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    dis = pulseIn(ecoPin, HIGH) / 58.00;

    if (dis && dis <= 10) {
        digitalWrite(ledG, LOW);
        digitalWrite(ledR, HIGH);
        delay(200);
        digitalWrite(ledR, LOW);
        delay(200);
        digitalWrite(ledR, HIGH);
        MyServo.write(90);
    }
    else if (dis) {
        digitalWrite(ledR, LOW);
        digitalWrite(ledG, HIGH);
        MyServo.write(0);
    }
    delay(100);
}