#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>
#include <avr/sleep.h>
#include "gameState.h"
#include "gameFunctions.h"

long randomValue;
unsigned int score = 0;
unsigned long start = millis();
unsigned long intervallo = 7000;
boolean alreadyPressed = false;
enum State {
  INITIATION,
  GAMING
};

State currentState = INITIATION;
LiquidCrystal_I2C lcd(0x27, 20, 2);

void wakeUp() {
  buttonLed(LED_PIN1, BUTTON_PIN1);
  buttonLed(LED_PIN2, BUTTON_PIN2);
  buttonLed(LED_PIN3, BUTTON_PIN3);
  buttonLed(LED_PIN4, BUTTON_PIN4);
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

  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  randomSeed(analogRead(0));
  randomValue = random(0, 16);
  Timer1.initialize(ON_MICRO_TIME);
  Timer1.attachInterrupt(sleepState);
  attachInterrupt(digitalPinToInterrupt(2), wakeUp, RISING); 
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
  }

}

void initiationState() {
  welcomeState();
  digitalWrite(RED_PIN, HIGH);
  buttonLed(LED_PIN1, BUTTON_PIN1);
  if (digitalRead(LED_PIN1) == HIGH) {
    digitalWrite(RED_PIN, LOW);
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
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  sleep_disable();  
}

void chooseDifficulty() {

}

void welcomeState() {
  lcd.setCursor(0,0);
  lcd.print("WELCOME TO GMD");
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
  if (millis() - start >= intervallo) {
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
  intervallo-=500;
}