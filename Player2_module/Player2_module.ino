/*
  **WARNING** I am using a I2C LCD module so the setup for a regular 16pin LCD will be different from mine
  References:
    https://projecthub.arduino.cc/Isaac100/getting-started-with-the-hc-sr04-ultrasonic-sensor-7cabe1
    https://projecthub.arduino.cc/arduino_uno_guy/i2c-liquid-crystal-displays-5eb615
    https://docs.arduino.cc/learn/built-in-libraries/software-serial/

*/

// I2C can be switched out for 16-pin lcd depending on hardware
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// using digital pins for serial communication from R3<->R4
#include <SoftwareSerial.h>

//  DEFINE OWN PIN VALUES
#define btnL_pin 12 
#define btnR_pin 4 
#define trigPin 8 
#define echoPin 7 
#define rxPin 9
#define txPin 10

LiquidCrystal_I2C lcd(0x27,  16, 2);
SoftwareSerial mySerial (rxPin, txPin);

// Global vals for determining distance from ultrasonic sensor
float duration, distance;

// Communication Variables
// We will be using bitmasking for latency worries
unsigned int send;
unsigned int receive;

// Game logic related variables
int game_state = 0;
int gameOn = 1;

// Button variables
int btnL_state, btnR_state;
int btnL_state_prev = LOW, btnR_state_prev = LOW;
unsigned long lastDebounceTimeL = 0, lastDebounceTimeR = 0;
unsigned long debounceDelayL = 50, debounceDelayR = 50;

void setup() {
  // lcd setup
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Time: --:--.--  ");
  // lcd.print("Not Pressed      ");

  // ultrasonic sensor setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // software serial related setup for serial communcation
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(9600);

  // button setup
  pinMode(btnL_pin,INPUT_PULLUP);
  pinMode(btnR_pin,INPUT_PULLUP);
}

void loop() {
  int btnL_read = digitalRead(btnL_pin);
  int btnR_read = digitalRead(btnR_pin);

  if(btnL_read != btnL_state_prev) lastDebounceTimeL = millis();
  if(btnR_read != btnR_state_prev) lastDebounceTimeR = millis();


  if( millis() - lastDebounceTimeL > debounceDelayL ){
    if( btnL_read != btnL_state ){
      btnL_state = btnL_read;

      lcd.setCursor(0,0);
      if( btnL_state == LOW){
        lcd.print("L: Not Pressed  ");
      } else {
        lcd.print("L: Pressed      ");
      }
    }
  }

  if( millis() - lastDebounceTimeR > debounceDelayR ){
    if( btnR_read != btnR_state ){
      btnR_state = btnR_read;

      lcd.setCursor(0,1);
      if( btnR_state == LOW){
        lcd.print("R: Not Pressed  ");
      } else {
        lcd.print("R: Pressed      ");
      }
    }
  }

  if( mySerial.available() > 0 ){
    // receive communication
    // maybe switch states or someething
  }

  if (gameOn) {
    displayTime(millis());
  }
  
  btnL_state_prev = btnL_read;
  btnR_state_prev = btnR_read;
}

void displayTime(unsigned long ms) {
  unsigned long centiseconds = (ms / 10) % 100;
  unsigned long seconds      = (ms / 1000) % 60;
  unsigned long minutes      = (ms / 60000);

  char buf[17];
  snprintf(buf, sizeof(buf), "Time:%02lu:%02lu.%02lu   ", minutes, seconds, centiseconds);

  lcd.setCursor(0, 0);
  lcd.print(buf);
}