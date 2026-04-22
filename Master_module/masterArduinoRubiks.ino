#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); 

int yellowLED = 9;
int greenLED = 8;
int redLED = 10;

int startButton = 3;
int buzzer = 11;

// Variables for countDownSeq
unsigned long prevTime = 0;
const unsigned long twoSec = 2000;
int count = 0;
bool sequenceDone = false; 
bool isCountingDown = false;
bool timerStarted = false;

// Game variables
bool gameActive = false;

void printToLCD( String line ){
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor((16 - line.length() ) / 2, 0);
  lcd.print(line);
}
void countDownSeq() {
  // We have already hit green and we want to exit
  if (sequenceDone == true) {
    return; 
  }

  unsigned long currTime = millis(); 

  // Check if one second has passed
  if ( (currTime - prevTime) >= twoSec ) {
    prevTime = currTime; // Reset the timer
    
    // Update leds based on count
    switch(count) {
      case 0: // Red Light ON
        digitalWrite(redLED, HIGH);
        digitalWrite(yellowLED, LOW);
        digitalWrite(greenLED, LOW);
        tone(buzzer, 1000, 1000);
        printToLCD("READY");
        break;
        
      case 1: // Yellow Light ON
        digitalWrite(redLED, LOW);
        digitalWrite(yellowLED, HIGH);
        digitalWrite(greenLED, LOW);
        tone(buzzer, 1000, 1000);
        printToLCD("SET");
        break;
        
      case 2: // FINAL : Green Light ON
        digitalWrite(redLED, LOW);
        digitalWrite(yellowLED, LOW);
        digitalWrite(greenLED, HIGH);
        tone(buzzer, 1700, 1000);
        printToLCD("GO");
        timerStarted = true;
        sequenceDone = true; // Ensure it stays green 
        gameActive = true;
        break;
    }
    
    // 4. Move to the next step (only if we havent finished seq
    if (sequenceDone == false) {
      count++;
    }
  }
}

void setup() {
  lcd.init();         
  lcd.backlight(); 

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
