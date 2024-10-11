#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>
#include <avr/sleep.h>
#include "gameState.h"
#include "gameFunctions.h"
#include <EnableInterrupt.h>


long randomValue;
unsigned int score = 0;
unsigned long start = millis();
unsigned long timeRound = 7000;
boolean alreadyPressed = false;
boolean isOn = true;
enum State {
  INITIATION,
  GAMING,
  SET_DIFFICULT
};

State currentState = INITIATION;
LiquidCrystal_I2C lcd(0x27, 20, 2);

//when system wakes up, it start to time, if after 10 seconds no button was pressed it restart to sleep.
void wakeUp() {
  start = millis();
}

void setup() {
  Serial.begin(9600);
  pinMode(RED_PIN, OUTPUT);  
  pinMode(LED_PIN1, OUTPUT);      
  pinMode(BUTTON_PIN1, INPUT);
  pinMode(LED_PIN2, OUTPUT);      
  pinMode(BUTTON_PIN2, INPUT);
  pinMode(LED_PIN3, OUTPUT);      
  pinMode(BUTTON_PIN3, INPUT);
  pinMode(LED_PIN4, OUTPUT);      
  pinMode(BUTTON_PIN4, INPUT);
  pinMode(POTENTIOMETER_PIN, INPUT);

  lcd.init();
  lcd.backlight();
  randomSeed(analogRead(0));
  randomValue = random(0, 16);
}

void loop() {
  // put your main code here, to run repeatedly:
  switch (currentState) {
    case INITIATION:
      initiationState();
      break;
    case GAMING:
      gamingState();
      break;
    case SET_DIFFICULT:
      chooseDifficulty();
  }

}

void initiationState() {
  welcomeState();
  lcd.setCursor(0, 1);
  lcd.print("Press B1 to start");
  digitalWrite(RED_PIN, HIGH);
  if (digitalRead(BUTTON_PIN1) == HIGH) {
    digitalWrite(RED_PIN, LOW);
    currentState = SET_DIFFICULT;
    lcd.clear();
  }
}

void roundState() {
  buttonLed(LED_PIN1, BUTTON_PIN1);
  buttonLed(LED_PIN2, BUTTON_PIN2);
  buttonLed(LED_PIN3, BUTTON_PIN3);
  buttonLed(LED_PIN4, BUTTON_PIN4);
  if (getPlayerValue() == randomValue) {
    score++;
    randomValue = random(0, 16);
    lcd.clear();
    lcd.print("GOOD! Score: ");
    lcd.print(score);
    delay(WAIT_TIME);
    increaseDifficult();
    lcd.clear();
    lcd.print(randomValue);
    start = millis();
  }
}

void sleepState() {
  enableInterrupt(BUTTON_PIN1, wakeUp, CHANGE);
  enableInterrupt(BUTTON_PIN2, wakeUp, CHANGE);
  enableInterrupt(BUTTON_PIN3, wakeUp, CHANGE);
  enableInterrupt(BUTTON_PIN4, wakeUp, CHANGE);
  lcd.noBacklight();
  digitalWrite(RED_PIN, LOW);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  sleep_disable();
  disableInterrupt(BUTTON_PIN1);
  disableInterrupt(BUTTON_PIN2);
  disableInterrupt(BUTTON_PIN3);
  disableInterrupt(BUTTON_PIN4);
  lcd.backlight();
}

void chooseDifficulty() {
  int levelDifficulty = (int) (analogRead(POTENTIOMETER_PIN) / 255) ;
  if (levelDifficulty == 0) {
    levelDifficulty = 1;
  }
  timeRound = (unsigned long) (TIME_ROUND_DEFAULT / levelDifficulty);
  lcd.setCursor(0, 0);
  lcd.print("difficulty: ");
  lcd.print(levelDifficulty);
  lcd.setCursor(0, 1);
  lcd.print("press B4 to start");
  if (digitalRead(BUTTON_PIN4) == HIGH) {
    currentState = GAMING;
    lcd.clear();
    lcd.print("Go");
    score = 0;
    delay(500);
    lcd.clear();
    lcd.print(randomValue);
    start = millis();
    roundState();
  }
}

void welcomeState() {
  lcd.setCursor(0,0);
  lcd.print("WELCOME TO GMB");
  if (millis() - start >= 10000) {
    sleepState();
  }
}

 void showResultState() {
    digitalWrite(RED_PIN, HIGH);
    delay(WAIT_TIME);
    digitalWrite(RED_PIN, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game Over");
    lcd.setCursor(0, 1);
    lcd.print("Score: ");
    lcd.print(score);
    delay(SHOW_TIME); 
}

void gamingState() {
  roundState();
  if (millis() - start >= timeRound) {
    showResultState();
    currentState = INITIATION;
  }
}
//bouncing is not a problem in our case
void buttonLed(int led, int button) {
  digitalWrite(led, digitalRead(button) == HIGH ? HIGH : LOW);
}

int getPlayerValue() {
  return (digitalRead(LED_PIN1) == HIGH ? 1 : 0) * 8 
    + (digitalRead(LED_PIN2) == HIGH ? 1 : 0) * 4
    + (digitalRead(LED_PIN3) == HIGH ? 1 : 0) * 2
    + (digitalRead(LED_PIN4) == HIGH ? 1 : 0) * 1;
}

void increaseDifficult() {
  timeRound-=500;
}