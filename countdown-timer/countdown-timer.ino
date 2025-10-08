
#include <LiquidCrystal_I2C.h>

#define HOUR_MIN 0
#define HOUR_MAX 24
#define HOUR_SECONDS 3600
#define MINUTE_MIN 0
#define MINUTE_MAX 59
#define MINUTE_SECONDS 60
#define SECOND_MIN 0
#define SECOND_MAX 59

enum Mode { ModeStart, ModeHours, ModeMinutes, ModeSeconds };

const unsigned int led_pin = 7;
const unsigned int mode_button_pin = 2;
const unsigned int start_button_pin = 5;
const unsigned int reset_button_pin = 3;
const unsigned int time_button_pin = 4;

const unsigned int debounce_delay = 50;

LiquidCrystal_I2C lcd(0x27, 16, 2);

Mode timer_mode = ModeStart;

uint8_t mode_button_previous_state = LOW;
uint8_t time_button_previous_state = LOW;

uint8_t hours = 0;
uint8_t minutes = 0;
uint8_t seconds = 0;

void setup() {
  Serial.begin(9600);

  pinMode(led_pin, OUTPUT);
  pinMode(mode_button_pin, INPUT);
  pinMode(start_button_pin, INPUT);
  pinMode(reset_button_pin, INPUT);
  pinMode(time_button_pin, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Timer");
  lcd.setCursor(0, 1);
  lcd.print("00 : 00 : 00");
}

void loop() {
  unsigned int start_button_state = digitalRead(start_button_pin);
  unsigned int reset_button_state = digitalRead(reset_button_pin);
  unsigned int mode_button_state = digitalRead(mode_button_pin);
  unsigned int time_button_state = digitalRead(time_button_pin);

  if (start_button_state == HIGH || reset_button_state == HIGH || mode_button_state == HIGH || time_button_state == HIGH) {   
    digitalWrite(led_pin, HIGH);
  } else {
    digitalWrite(led_pin, LOW);
  }

  if (mode_button_state == LOW && mode_button_previous_state == HIGH) {
    changeMode();
  }

  if (time_button_state == LOW && time_button_previous_state == HIGH) {
    changeTime();
  }

  mode_button_previous_state = mode_button_state;
  time_button_previous_state = time_button_state;
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

// uint32_t previous_time = 0;
// uint8_t mode_button_current_state = LOW;
// uint8_t mode_button_previous_state = LOW;
  // if (mode_button_state != mode_button_previous_state) {
  //   Serial.println("a");
  //   previous_time = current_time;
  // }

  // if (current_time > (previous_time + debounce_delay)) {
  //   Serial.println("b");
  //   if (mode_button_state != mode_button_current_state) {
  //     mode_button_current_state = mode_button_state;

  //     if (mode_button_current_state == HIGH) {
  //       changeMode();
  //     }
  //   }
  // }

  // mode_button_previous_state = mode_button_state;

