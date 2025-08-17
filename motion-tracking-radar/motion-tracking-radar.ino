#include <Servo.h>
#include <LiquidCrystal_I2C.h>

#define SERVO_MIN 10
#define SERVO_MAX 160
#define SERVO_STEP 4
#define ULTRASOUND_DELAY 10

const uint8_t servo_pin = 6;
const uint8_t buzzer_pin = 9;
const uint8_t positive_led_pin = 8;
const uint8_t negative_led_pin = 12;
const uint8_t ultrasound_in_pin = 7;
const uint8_t ultrasound_out_pin = 4;
const uint32_t buzzer_tone = 1500;
const uint32_t action_interval = 20;
const uint8_t distance_threshold = 30;

Servo servo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

bool servo_to_zero = false;
bool lcd_is_error = false;
bool buzzer_buzz = false;
uint32_t previous_time = 0;
uint8_t servo_angle = SERVO_MIN;
uint8_t negative_led_state = LOW;

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Area is empty");

  servo.attach(servo_pin);
  servo.write(servo_angle);

  pinMode(negative_led_pin, OUTPUT);
  digitalWrite(negative_led_pin, LOW);

  pinMode(positive_led_pin, OUTPUT);
  digitalWrite(positive_led_pin, LOW);

  pinMode(buzzer_pin, OUTPUT);
  noTone(buzzer_pin);
  
  pinMode(ultrasound_in_pin, INPUT);
  pinMode(ultrasound_out_pin, OUTPUT);

  digitalWrite(ultrasound_out_pin, LOW);
  delayMicroseconds(2);
}

void loop() {
  size_t i;
  long duration;
  float distance;

  uint32_t current_time = millis();

  digitalWrite(ultrasound_out_pin, HIGH);
  delayMicroseconds(ULTRASOUND_DELAY);

  digitalWrite(ultrasound_out_pin, LOW);

  duration = pulseIn(ultrasound_in_pin, HIGH);

  Serial.print("Duration: ");  
	Serial.print(duration);  
  Serial.print(" us - ");

  // convert time to distance
  distance = (duration * 0.0343) / 2;

  Serial.print("Distance: ");  
	Serial.print(distance);  
  Serial.println(" cm");

  
  digitalWrite(positive_led_pin, distance > distance_threshold ? HIGH : LOW);

  if (!lcd_is_error && distance < distance_threshold) {
    lcd.clear();
    lcd.print("Warning!!!");
    lcd.setCursor(0, 1);
    lcd.print("Foreign body");
    lcd_is_error = true;
  } else if (lcd_is_error && distance > distance_threshold) {
    lcd.clear();
    lcd.print("Area is empty");
    lcd_is_error = false;
  }

  if ((current_time - previous_time) >= action_interval) {
    previous_time = current_time;

    if (distance < distance_threshold) {
      negative_led_state = negative_led_state == LOW ? HIGH : LOW;

      digitalWrite(negative_led_pin, negative_led_state);

      if (buzzer_buzz) {
        buzzer_buzz = false;
        tone(buzzer_pin, buzzer_tone);
      } else {
        buzzer_buzz = true;
        noTone(buzzer_pin);
      }
    } else {
      negative_led_state = LOW;
      digitalWrite(negative_led_pin, negative_led_state);

      buzzer_buzz = true;
      noTone(buzzer_pin);
  
    
      servo_angle = servo_to_zero ? (servo_angle - SERVO_STEP) : (servo_angle + SERVO_STEP);

      Serial.print("Servo angel: ");
      Serial.print(servo_angle);
      Serial.println(" degrees");

      servo.write(servo_angle);

      if (servo_to_zero && servo_angle <= SERVO_MIN) {
        servo_to_zero = false;
      } else if (!servo_to_zero && servo_angle >= SERVO_MAX) {
        servo_to_zero = true;
      }
    }
  }

}
