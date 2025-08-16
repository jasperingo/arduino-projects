const uint8_t led_pin = 8;
const uint8_t buzzer_pin = 9;
const uint32_t buzzer_tone = 1500;
const uint32_t action_interval = 1000;

uint8_t led_state = LOW;
uint8_t buzzer_state = LOW;
uint32_t previous_time = 0;

void setup() {
  pinMode(led_pin, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);
}

void loop() {
  uint32_t current_time = millis();

  if ((current_time - previous_time) >= action_interval) {
    previous_time = current_time;

    led_state = led_state == LOW ? HIGH : LOW;

    digitalWrite(led_pin, led_state);

    buzzer_state = buzzer_state == LOW ? HIGH : LOW;

    if (buzzer_state == HIGH) {
      tone(buzzer_pin, buzzer_tone);
    } else {
      noTone(buzzer_pin);
    }
  }
}
