/**
 * Inspired by project from YouTube: https://www.youtube.com/shorts/Zy0sGvEWFmc
 * This project uses two LEDs, one LCD, four push buttons, breadboard, and Piezo buzzer
 * The buttons are set up on a breadboard with pull-down resistors to detect button presses
 * First button is the mode button, this sets the timer state to seconds, minutes, hours or start
 * Second button is the time button, which is used to set the timer based on the mode selected apart from start mode
 * Third button is the reset button, which is used to reset all the time set up and can even stop the count down
 * Forth button is the start button, used to start the count down when the mode is start
 * The LCD displays the mode of the timer according as well as the current time
 * The red LED blinks when a button is pressed. It is used as an extra feedback for a button press
 * The green LED blinks while the count down is in progress
 * The buzzer plays a tone when the count down is complete
*/

#include <LiquidCrystal_I2C.h>

#define HOUR_MIN 0
#define HOUR_MAX 24
#define HOUR_SECONDS 3600
#define MINUTE_MIN 0
#define MINUTE_MAX 59
#define MINUTE_SECONDS 60
#define SECOND_MIN 0
#define SECOND_MAX 59
#define COUNT_DOWN_MILLIS 1000
#define BUZZER_TONE 1500
#define BUZZER_DURATION 4000

enum Mode { ModeStart, ModeHours, ModeMinutes, ModeSeconds };

const uint8_t buzzer_pin = 9;
const uint8_t press_led_pin = 7;
const uint8_t count_led_pin = 8;
const uint8_t mode_button_pin = 2;
const uint8_t start_button_pin = 5;
const uint8_t reset_button_pin = 3;
const uint8_t time_button_pin = 4;

const uint8_t debounce_delay = 50;

LiquidCrystal_I2C lcd(0x27, 16, 2);

Mode timer_mode = ModeStart;

uint8_t count_led_previous_state = LOW;
uint8_t mode_button_previous_state = LOW;
uint8_t time_button_previous_state = LOW;
uint8_t reset_button_previous_state = LOW;
uint8_t start_button_previous_state = LOW;

uint8_t hours = 0;
uint8_t minutes = 0;
uint8_t seconds = 0;

uint32_t count_down_seconds = 0;

uint32_t previous_time = 0;

void setup() {
  Serial.begin(9600);

  pinMode(press_led_pin, OUTPUT);
  pinMode(count_led_pin, OUTPUT);
  pinMode(mode_button_pin, INPUT);
  pinMode(start_button_pin, INPUT);
  pinMode(reset_button_pin, INPUT);
  pinMode(time_button_pin, INPUT);

  pinMode(buzzer_pin, OUTPUT);
  noTone(buzzer_pin);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  printToScreen();
}

void loop() {
  unsigned int start_button_state = digitalRead(start_button_pin);
  unsigned int reset_button_state = digitalRead(reset_button_pin);
  unsigned int mode_button_state = digitalRead(mode_button_pin);
  unsigned int time_button_state = digitalRead(time_button_pin);

  if (
    start_button_state == HIGH || 
    reset_button_state == HIGH || 
    mode_button_state == HIGH || 
    time_button_state == HIGH
  ) {
    digitalWrite(press_led_pin, HIGH);
  } else {
    digitalWrite(press_led_pin, LOW);
  }

  if (count_down_seconds == 0 && mode_button_state == LOW && mode_button_previous_state == HIGH) {
    changeMode();
  } else if (count_down_seconds == 0 && time_button_state == LOW && time_button_previous_state == HIGH) {
    changeTime();
  } else if (count_down_seconds == 0 && timer_mode == ModeStart && start_button_state == LOW && start_button_previous_state == HIGH) {
    count_down_seconds = (hours * HOUR_SECONDS) + (minutes * MINUTE_SECONDS) + seconds;
  } else if (reset_button_state == LOW && reset_button_previous_state == HIGH) {
    resetTimer();
  }

  mode_button_previous_state = mode_button_state;
  time_button_previous_state = time_button_state;
  reset_button_previous_state = reset_button_state;
  start_button_previous_state = start_button_state;

  if (count_down_seconds > 0) {
    uint32_t current_time = millis();

    if ((current_time - previous_time) >= COUNT_DOWN_MILLIS) {
      previous_time = current_time;

      count_down_seconds--;

      uint8_t count_led_state = count_down_seconds == 0 ? LOW : (count_led_previous_state == HIGH ? LOW : HIGH);

      digitalWrite(count_led_pin, count_led_state);

      count_led_previous_state = count_led_state;

      if (count_down_seconds == 0) {
        tone(buzzer_pin, BUZZER_TONE, BUZZER_DURATION);
      }

      hours = count_down_seconds / HOUR_SECONDS; // 1 hour = 3600 seconds

      // Calculate remaining minutes after extracting hours
      minutes = (count_down_seconds % HOUR_SECONDS) / MINUTE_SECONDS; // 1 minute = 60 seconds

      // Calculate remaining seconds
      seconds = count_down_seconds % MINUTE_SECONDS;

      printToScreen();
    }
  }
}

void resetTimer() {
  timer_mode = ModeStart;
  seconds = 0;
  minutes = 0;
  hours = 0;
  count_down_seconds = 0;

  printToScreen();
}

void changeMode() {
  switch (timer_mode) {
    case ModeStart:
      timer_mode = ModeSeconds;
      break;
    case ModeSeconds:
      timer_mode = ModeMinutes;
      break;
    case ModeMinutes:
      timer_mode = ModeHours;
      break;
    case ModeHours:
      timer_mode = ModeStart;
      break;
  }

  printToScreen();
}

void changeTime() {
  switch (timer_mode) {
    case ModeSeconds:
      seconds = seconds + 1 > SECOND_MAX ? SECOND_MIN : seconds + 1;
      break;
    case ModeMinutes:
      minutes = minutes + 1 > MINUTE_MAX ? MINUTE_MIN : minutes + 1;
      break;
    case ModeHours:
      hours = hours + 1 > HOUR_MAX ? HOUR_MIN : hours + 1;
      break;
  }

  printToScreen();
}

void printToScreen() {
  lcd.clear();

  switch (timer_mode) {
    case ModeStart:
      lcd.print("Timer");
      break;
    case ModeSeconds:
      lcd.print("Set Timer SS");
      break;
    case ModeMinutes:
      lcd.print("Set Timer MM");
      break;
    case ModeHours:
      lcd.print("Set Timer HH");
      break;
  }

  lcd.setCursor(0, 1);

  if (hours < 10) {
    lcd.print(0);
  }

  lcd.print(hours);

  lcd.print(" : ");

  if (minutes < 10) {
    lcd.print(0);
  }
  
  lcd.print(minutes);

  lcd.print(" : ");

  if (seconds < 10) {
    lcd.print(0);
  }

  lcd.print(seconds);
}
