/*
  **WARNING** I am using a I2C LCD module so the setup for a regular 16pin LCD will be different from mine
  References:
    https://projecthub.arduino.cc/Isaac100/getting-started-with-the-hc-sr04-ultrasonic-sensor-7cabe1
    https://projecthub.arduino.cc/arduino_uno_guy/i2c-liquid-crystal-displays-5eb615
    https://docs.arduino.cc/learn/built-in-libraries/software-serial/

*/

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SoftwareSerial.h>

//  DEFINE OWN PIN VALUES
#define btn1_pin 12 
#define btn2_pin 4 
#define trigPin 8 
#define echoPin 7 
#define rxPin 9
#define txPin 10

LiquidCrystal_I2C lcd(0x27,  16, 2);
SoftwareSerial mySerial (rxPin, txPin);

float duration, distance;

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  
  pinMode(btn1_pin,INPUT_PULLUP);
  pinMode(btn2_pin,INPUT_PULLUP);
}

void loop() {
  
}
