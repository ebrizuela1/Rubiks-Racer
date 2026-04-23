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

#define distance_threshold 10

LiquidCrystal_I2C lcd(0x27,  16, 2);
SoftwareSerial mySerial (rxPin, txPin);

// Global vals for determining distance from ultrasonic sensor
float         duration_uss, distance_uss;
unsigned int  last_measure = 0;

// Communication Variables
// We will be using bitmasking for latency worries
uint8_t  send;
uint8_t  receive;

// Game logic related variables
int           game_state = 0;
bool          buttonL_press = false, 
              buttonR_press = false, 
              uss_near = false;

// Button variables
int           btnL_state, 
              btnR_state, 
              btnL_state_prev = LOW, 
              btnR_state_prev = LOW;

unsigned long lastDebounceTimeL = 0, 
              lastDebounceTimeR = 0;

unsigned long debounceDelayL = 50, 
              debounceDelayR = 50;
              
unsigned long finalTime, startTime;

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
  pinMode(rxPin, INPUT_PULLUP);
  pinMode(txPin, OUTPUT);
  mySerial.begin(9600);

  // button setup
  pinMode(btnL_pin,INPUT_PULLUP);
  pinMode(btnR_pin,INPUT_PULLUP);

  // Regular serial for debugging purposes

  // game_state = 2;
  // startTime = millis();
  Serial.begin(9600);
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
      buttonL_press = (btnL_state == HIGH);
      // if( btnL_state == LOW){
      //   lcd.print("L: Not Pressed  ");
      // } else {
      //   lcd.print("L: Pressed      ");
      // }
    }
  }

  if( millis() - lastDebounceTimeR > debounceDelayR ){
    if( btnR_read != btnR_state ){
      btnR_state = btnR_read;

      lcd.setCursor(0,1);
      buttonR_press = (btnR_state == HIGH);
      // if( btnR_state == LOW){
      //   lcd.print("R: Not Pressed  ");
      // } else {
      //   lcd.print("R: Pressed      ");
      // }
    }
  }

  if( (millis() - last_measure) > 12){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // duration_uss = pulseIn(echoPin, HIGH); 
    duration_uss = pulseIn(echoPin, HIGH, 24000); 
    
    if (duration_uss > 0) {
      distance_uss = (duration_uss*0.0343)/2;

      uss_near = (distance_uss < distance_threshold);
      // Serial.println(distance_uss); 
    }
    last_measure = millis();
  }
  
  // least significant bit in send represents in position conditional
  if(buttonL_press && buttonR_press && uss_near){
    send |= (1 << 0);
  } else {
    send &= ~(1 << 0);
  }

  // only 1 byte is ever needed for commuication since we are using bit masking for information
  if( mySerial.available() > 0 ){
    receive = mySerial.read();

    // DEBUG STATEMENT
    Serial.println(receive);

  }

  // GO TO SPECIFIC GAME STATE BASED ON RECEIVED SIGNAL
  if( (receive >> 0) & 1 == 1 ){
    game_state = 0;
  } else if ( (receive >> 1) & 1 == 1 ){
    game_state = 2;
  } else if ( (receive >> 2) & 1 == 1 ){
    game_state = 4;
  }

  // SPECIAL CONDITIONS:
  if( (receive >> 3) & 1 == 1 ){
    // HANDLE FALSE START
  }
  if( (receive >> 4) & 1 == 1 ){
    // HANDLE WIN
  } else {
    // HANDLE LOSS
  }

  switch(game_state){
    case 0:
    case 1:
      if(receive>>1 & 1){ // start the game 
        startTime = millis(); // set game start time
        game_state = 2;
      }
      break;
    case 2:
      displayTime(millis() - startTime);
      if( !(buttonL_press && buttonR_press && uss_near) ){
        finalTime = millis() - startTime; // freeze time on exit
        game_state = 3;
      }
      break;
    case 3:
      if(buttonL_press && buttonR_press && uss_near){
        send |= (1 << 1);
      } else {
        send &= ~(1 << 1);
      }
      break;
    case 4:
      displayTime(finalTime);
      displayResults();
      break;
    default:
      break;
  }

  mySerial.write(send);

  btnL_state_prev = btnL_read;
  btnR_state_prev = btnR_read;
}

void displayResults(){
  lcd.setCursor(0, 1);
  lcd.print("Game Over!");
}

void displayTime(unsigned long ms) {
  char buf[17];

  unsigned long centiseconds = (ms / 10) % 100;
  unsigned long seconds      = (ms / 1000) % 60;
  unsigned long minutes      = (ms / 60000);

  snprintf(buf, sizeof(buf), "Time:%02lu:%02lu.%02lu   ", minutes, seconds, centiseconds);
 

  lcd.setCursor(0, 0);
  lcd.print(buf);

  // return buf;
}