/*
* File: src/Experiment4.cpp
* Author: donghy23@mails.tsinghua.edu.cn
* Repo: https://github.com/FHYQ-Dong/Tsinghua-Hardware-Design-Assignment/blob/main/Experiment4/src/Experiment4.cpp
* Date: 2024.7.13
*/

#include <Arduino.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include <U8glib.h>
#include <String.h>


String passwd("9876");
SoftwareSerial softwareserial(7, 6);
uint8_t status = 0;
bool isLocked = true;
U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);


void drawScreen() {
    u8g.firstPage();
    do {
        u8g.setFont(u8g_font_unifont);
        if (isLocked) u8g.drawStr(30, 32, "locked");
        else u8g.drawStr(30, 22, "unlocked");
    } while (u8g.nextPage());
}

void setup() {
    Serial.begin(9600);
    softwareserial.begin(9600);
    drawScreen();
}

void loop() {
    if (softwareserial.available()) {
        String input_passwd = softwareserial.readString();
        if (status == 0) {
            if (input_passwd == passwd) {
                Serial.print("pass\n");
                isLocked = false;
                status = 1;
                drawScreen();
            }
            else {
                Serial.print("fail\n");
                isLocked = true;
                status = 0;
                drawScreen();
            }
        }
        else if (status == 1) {
            passwd = input_passwd;
            Serial.print("password changed\n");
            isLocked = true;
            status = 0;
            drawScreen();
        }
    }
}
