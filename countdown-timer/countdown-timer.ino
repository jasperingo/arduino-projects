
unsigned int led_pin = 7;
unsigned int mode_button_pin = 2;
unsigned int start_button_pin = 5;
unsigned int reset_button_pin = 3;
unsigned int time_button_pin = 4;

void setup() {
  pinMode(led_pin, OUTPUT);
  pinMode(mode_button_pin, INPUT);
  pinMode(start_button_pin, INPUT);
  pinMode(reset_button_pin, INPUT);
  pinMode(time_button_pin, INPUT);
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
}
