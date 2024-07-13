/*
* File: src/Experiment3.cpp
* Author: donghy23@mails.tsinghua.edu.cn
* Repo: https://github.com/FHYQ-Dong/Tsinghua-Hardware-Design-Assignment/blob/main/src/Experiment3.cpp
* Date: 2024.7.13
*/

#include <Arduino.h>
#include <TimerOne.h>
#include <SevSeg.h>

const int xPin = A0, yPin = A1, zPin = 2;
int numToShow = 0;
const long interval = 100000;
bool isTiming = true;
byte numDigits = 4, digitPins[] = {1, 3, 12, 13}, segmentPins[] = {7, 6, 5, 10, 11, 8, 9, 4};
SevSeg sevseg;

void reset_timing() {
    numToShow = 0;
    isTiming = true;
    sevseg.setNumber(numToShow, 1);
}

void timing() {
    if (!isTiming) return;

    if (numToShow < 9999) numToShow++;
    else numToShow = 0;
    sevseg.setNumber(numToShow, 1);
}

void setup() {
    Serial.begin(9600);
    pinMode(zPin, INPUT_PULLUP);
    // 按下摇杆
    attachInterrupt(digitalPinToInterrupt(zPin), reset_timing, FALLING);
    // 计时器
    Timer1.initialize(interval);
    Timer1.attachInterrupt(timing);
    // 数码管
    sevseg.begin((byte)(COMMON_CATHODE), numDigits, digitPins, segmentPins);
}

void loop() {
    int x_deg = analogRead(xPin);
    int y_deg = analogRead(yPin);
    if (x_deg <= 256 || x_deg >= 768) isTiming = false;
    else if (y_deg <= 256 || y_deg >= 768) isTiming = true;
    sevseg.refreshDisplay();
}