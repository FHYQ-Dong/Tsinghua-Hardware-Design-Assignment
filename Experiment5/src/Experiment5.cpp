/*
* @file: src/Experiment5.cpp
* @author: donghy23@mails.tsinghua.edu.cn
* @repo: https://github.com/FHYQ-Dong/Tsinghua-Hardware-Design-Assignment/blob/main/Experiment5/src/Experiment5.cpp
* @date: 2024.7.14
*/

#include <Arduino.h>
#include <TimerOne.h>


int ENA = 6, IN1 = 10, IN2 = 11, ENC_A = 2, ENC_B = 3;
double Kp = 4, Ki = 2, Kd = 4;
int32_t calc_period = 50000, edges_per_cycle = 13, reduction_ratio = 20;
double targetRPM = 300, PWM = 100;
int32_t count = 0;
double rpm = 0, err = 0, derr = 0, dderr = 0;

void count_ISR_A() {
    int volA = digitalRead(ENC_A), volB = digitalRead(ENC_B);
    if ((volA == HIGH && volB == HIGH) || (volA == LOW && volB == LOW)) count++;
    else count--;
}

void count_ISR_B() {
    int volA = digitalRead(ENC_A), volB = digitalRead(ENC_B);
    if ((volA == LOW && volB == HIGH) || (volA == HIGH && volB == LOW)) count++;
    else count--;
}

double PID(double target, double current) {
    dderr = target - current - err - derr;
    derr = target - current - err;
    err = target - current;
    return Ki * err + Kp * derr + Kd * dderr;
}

void calcPWM() {
    rpm = (count/4.0) / (calc_period/1000000.0) * 60 / edges_per_cycle / reduction_ratio;
    count = 0;
    Serial.print(">rpm:");
    Serial.println(rpm);
    PWM += PID(targetRPM, rpm);
    if (PWM > 255) PWM = 255;
    else if (PWM < -255) PWM = -255;
    if (PWM < 0) PWM = 0;
}

void setup() {
    Serial.begin(9600);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENA, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(ENC_A), count_ISR_A, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENC_B), count_ISR_B, CHANGE);
    Timer1.initialize(calc_period);
    Timer1.attachInterrupt(calcPWM);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
}

void loop() {
    analogWrite(ENA, (int32_t)(PWM));
}