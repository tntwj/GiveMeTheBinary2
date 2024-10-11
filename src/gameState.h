#define LED_PIN1 13
#define LED_PIN2 12
#define LED_PIN3 11
#define LED_PIN4 10
#define BUTTON_PIN1 7
#define BUTTON_PIN2 6
#define BUTTON_PIN3 5
#define BUTTON_PIN4 4
#define RED_PIN 9
#define POTENTIOMETER_PIN A0

#define SHOW_TIME 10000
#define WAIT_TIME 1000
#define ON_MICRO_TIME 10000000

void initiationState();

void gamingState();

//a round which player will be asked to choose buttons that corresponding the given value.
void roundState();

//a state which the system is in sleep mode, waiting player to click any button to awake the system.
void sleepState();

//let player to choose difficulty of the game.
void chooseDifficulty();

//the first state waiting player click B1 button to initialize.
void welcomeState();

//when player lose the game, and system will show his score.
void showResultState();