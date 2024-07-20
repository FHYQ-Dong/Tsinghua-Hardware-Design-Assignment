/*
 * @file: src/Experiment6.cpp
 * @author: donghy23@mails.tsinghua.edu.cn
 * @repo: https://github.com/FHYQ-Dong/Tsinghua-Hardware-Design-Assignment/blob/main/Experiment6/src/Experiment6.cpp
 * @date: 2024.7.19
 * @video: https://cloud.tsinghua.edu.cn/smart-link/0985994d-3c84-4a14-950a-c4464c5bdeac/
 */

#include <Arduino.h>
#include "TimerOne.h"
#include "U8glib.h"

// 定时器中断间隔, 间隔10ms
const long interval = 10 * 1000;

// 编码器AB相的引脚
const int encoder0PinA = 2;
const int encoder0PinB = 3;

// pmw输出引脚
const int PWMPin = 6;

// 正反转控制引脚
const int inv1 = 10, inv2 = 11;

// pid相关参数
float kp = 2.0f, ki = 0.0f, kd = 10.1f;

// pid计算所用变量
float err = 0.0f, iErr = 0.0f, lastErr = 0.0f;

// 编码器转过的角度
int valRotary = 0;

// 手感相关参数
const int dis = 50;
const int num = 4;
int arr[num];

// 屏幕
U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);

char tmpStr[20];
void draw() {
    double per = valRotary / 220.0f;
    if (per > 1) per = 1;
    else if (per < -1) per = -1;
    sprintf(tmpStr, "current: %d%%+", (int)(per * 100));
    u8g.drawStr(10, 30, tmpStr);
    if (per > 0) u8g.drawBox(64, 10, (int)(56 * per), 5);
    else if (per < 0) u8g.drawBox(64 + (int)(56 * per), 10, (int)(-56 * per), 5);
    else u8g.drawBox(64, 10, 0, 5);
}

void Control() {
    int goal = 0;

    int minDis = 10000, minIdx = 0;
    for (int i = 0; i < num; ++i) {
        int tmp = abs(valRotary - arr[i]);
        if (tmp < minDis) {
            minDis = tmp;
            minIdx = i;
        }
    }

    goal = arr[minIdx];

    err = goal - valRotary;

    // pid计算结果
    float tht = kp * err + ki * iErr + kd * (err - lastErr);
    iErr += err;
    lastErr = err;

    int pwm = 0; // 输出的占空比
    // 判断正反转
    if (tht > 0) {
        pwm = tht;
        if (pwm > 200)
            pwm = 200; // 输出限幅
        digitalWrite(inv1, HIGH);
        digitalWrite(inv2, LOW);
    }
    else {
        tht = -tht;
        pwm = tht;
        if (pwm > 200)
            pwm = 200; // 输出限幅
        digitalWrite(inv1, LOW);
        digitalWrite(inv2, HIGH);
    }

    analogWrite(PWMPin, pwm); // 输出pwm
}

// 处理编码器输入
void doEncoder() {
    if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB))
        valRotary++;
    else
        valRotary--;
}

void setup() {
    // put your setup code here, to run once:
    // 引脚初始化
    pinMode(encoder0PinA, INPUT_PULLUP);
    pinMode(encoder0PinB, INPUT_PULLUP);

    // 设置编码器的输入
    attachInterrupt(0, doEncoder, CHANGE);

    Serial.begin(9600);

    // 设置定时器中断
    Timer1.initialize(interval);
    Timer1.attachInterrupt(Control);

    // 计算挡位位置
    for (int i = 0; i < num; ++i) 
        arr[i] = i * dis;

    // 初始化屏幕
    u8g.setFont(u8g_font_unifont);
}

void loop() {
    // put your main code here, to run repeatedly:
    // 输出电机的角度
    Serial.println(valRotary);

    u8g.firstPage();
    do {
        // u8g.drawStr(0, 22, "hello");
        draw();
    } while (u8g.nextPage());
}
