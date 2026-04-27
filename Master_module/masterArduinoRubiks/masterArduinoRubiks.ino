/*
  Team: 
   Edgar Brizuela, ebriz2, 671438415
   Gael Perez, gpere78, 664791976
   Auj Bin Faisal, afais, 657825230
  Abstract:
     The purpose of this project is to allow two users to compete against one another in a
  race to see who can solve a rubiks cube faster. It consists of three different arduino
  microprocessors that use serial communication to send signals to each other. There are LEDs,
  a 16x2 LCD and a passive buzzer on one arduino, while the other two have a distance sensor to
  detect whether or not any of the players are done solving their respective cube. Upon winning,
  the buzzer sounds and the green LED turns on.
*/

#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Player one
// #define rxPinPlayerOne 0
// #define txPinPlayerOne 1

// Player two
#define rxPinPlayerTwo 12
#define txPinPlayerTwo 13

SoftwareSerial playerTwoSerial(rxPinPlayerTwo, txPinPlayerTwo);

uint8_t sendOne;
uint8_t recieveOne;

uint8_t sendTwo;
uint8_t recieveTwo;

// pins 
#define yellowLED 9
#define greenLED 8
#define redLED 10
#define startButton 3
#define buzzer 11

// Serial Write Signals
uint8_t falseStart = 12;

// Variables for countDownSeq
unsigned long prevTime = 0;
const unsigned long twoSec = 2000;
int count = 0;
bool sequenceDone = false; 
bool isCountingDown = false;
bool timerStarted = false;

// Game variables
bool gameActive = false;


// Prints to the first line of the LCD screen
void printToLCD( String sentence, int lineNum ){
  lcd.setCursor(0, lineNum);
  lcd.print("                ");
  lcd.setCursor((16 - sentence.length() ) / 2, lineNum);
  lcd.print(sentence);
}

// Check if both players are ready
bool validateStart(){
  // Check player one first
  if (Serial1.available() > 0){
    recieveOne = Serial1.read();
    Serial.println(recieveOne);
  } 

  // if (playerTwoSerial.available() > 0){
  //   recieveTwo = playerTwoSerial.read();
  //   Serial.println(recieveTwo);
  // } else { return false ;}

  if ( recieveOne > 0){ return true; }
  return false;
}

// different messages for players
void sendToPlayers (uint8_t playerOne, uint8_t playerTwo) {
  Serial1.write(playerOne);
  playerTwoSerial.write(playerTwo);
}
// same message for both players
void sendToPlayers (uint8_t bothPlayers) {
  Serial1.write(bothPlayers);
  playerTwoSerial.write(bothPlayers);
}

// RED, YELLOW, GREEN countdown to start race
void countDownSeq() {
  // We have already hit green and we want to exit
  if (sequenceDone == true) {
    return; 
  }
  unsigned long currTime = millis(); 
  if ( (currTime - prevTime) >= twoSec ) { // Check if one second has passed
    prevTime = currTime; // Reset the timer
    bool valid = validateStart();
      if (valid == false) {
        printToLCD("FALSE START", 0);
        sendToPlayers(falseStart);// send to both players
        // isCountingDown = false;
        // count = 0;
        // digitalWrite(redLED, LOW);
        // digitalWrite(yellowLED, LOW);
        // digitalWrite(greenLED, LOW);
        // return;
      }

    switch(count) { // Update leds based on count
      
      case 0: // Red Light ON
        digitalWrite(redLED, HIGH);
        digitalWrite(yellowLED, LOW);
        digitalWrite(greenLED, LOW);
        tone(buzzer, 1000, 1000);
        printToLCD("READY", 0);
        break;
        
      case 1: // Yellow Light ON
        digitalWrite(redLED, LOW);
        digitalWrite(yellowLED, HIGH);
        digitalWrite(greenLED, LOW);
        tone(buzzer, 1000, 1000);
        printToLCD("SET", 0);
        break;
      
      case 2: // FINAL : Green Light ON
      
        digitalWrite(redLED, LOW);
        digitalWrite(yellowLED, LOW);
        digitalWrite(greenLED, HIGH);
        tone(buzzer, 1700, 1000);
        printToLCD("GO", 0);
        timerStarted = true;
        sequenceDone = true; // Ensure it stays green 
        gameActive = true;
        break;
    }
    // Move to the next step (only if we havent finished seq
    if (sequenceDone == false) {
      count++;
    }
  }
}


void setup() {
  lcd.init();         
  lcd.backlight(); 
  Serial.begin(9600);
  Serial1.begin(9600);
  playerTwoSerial.begin(9600);

  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  pinMode(startButton, INPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  
  if (digitalRead(startButton) == HIGH){
    isCountingDown = true;
  }
  if (isCountingDown == true){
    countDownSeq();
  }
}
