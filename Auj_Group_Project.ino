/*
  Individual player code for a Rubik's cube solving race
  Sends signals to a referee arduino board to indicate if the player is ready for the game to start or end
  Displays time taken on an LCD screen on the first row 

  **WARNING** I am using a I2C LCD module so the setup for a regular 16pin LCD will be different from mine

  References:
    https://projecthub.arduino.cc/Isaac100/getting-started-with-the-hc-sr04-ultrasonic-sensor-7cabe1
    https://projecthub.arduino.cc/arduino_uno_guy/i2c-liquid-crystal-displays-5eb615
    https://docs.arduino.cc/learn/built-in-libraries/software-serial/

*/

// I2C can be switched out for 16-pin lcd depending on hardware
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27,  16, 2);

// TODO: state if the game is currenty ongoing 
int gameOn = 1;

void displayTime(unsigned long ms) {
  unsigned long centiseconds = (ms / 10) % 100;
  unsigned long seconds      = (ms / 1000) % 60;
  unsigned long minutes      = (ms / 60000);

  char buf[17];
  snprintf(buf, sizeof(buf), "Time:%02lu:%02lu.%02lu   ", minutes, seconds, centiseconds);

  lcd.setCursor(0, 0);
  lcd.print(buf);
}

void setup() {
  // lcd setup
  lcd.init();
  lcd.backlight();


  lcd.setCursor(0, 0);
  lcd.print("Time: --:--.--  ");
}

void loop() {
 
  if (gameOn) {
    displayTime(millis());
  }
  
}