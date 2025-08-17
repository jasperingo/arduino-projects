/**
 * Inspired by project from YouTube: https://www.youtube.com/watch?v=IIe3DMzaRSA
 * This project uses two LEDs, one LCD, Servo motor, Piezo buzzer & Ultrasound sensor
 * The utltrasound sensor is set up on the servo which rotates it to scan for object within a distance
 * If an object is found, the servo stops rotating, the buzzer starts buzzing and the red LED starts blinking
 * The LCD also displays a warning messsge denoting the presence of an object
 * On the other hand, the servo continues rotating, the buzzer and red LED are turned off and the positive LED is turned on
 * The LCD also displayes a message denoting the absence of an object
*/

#include <Servo.h>
#include <LiquidCrystal_I2C.h>

#define SERVO_MIN 10
#define SERVO_MAX 160
#define SERVO_STEP 4
#define ULTRASOUND_DELAY 10
#define SOUND_SPEED 0.0343 // speed of sound in centimeters per microsecond

const uint8_t servo_pin = 6;
const uint8_t buzzer_pin = 9;
const uint8_t positive_led_pin = 8;
const uint8_t negative_led_pin = 12;
const uint8_t ultrasound_in_pin = 7;
const uint8_t ultrasound_out_pin = 4;
const uint32_t buzzer_tone = 1500;
const uint32_t action_interval = 20;
const uint8_t distance_threshold = 20;

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

  // Read distance from object with ultrasound
  digitalWrite(ultrasound_out_pin, HIGH);
  delayMicroseconds(ULTRASOUND_DELAY);

  digitalWrite(ultrasound_out_pin, LOW);

  duration = pulseIn(ultrasound_in_pin, HIGH);

  Serial.print("Duration: ");  
	Serial.print(duration);  
  Serial.print(" us - ");

  // Convert time to distance
  // Divide it by 2 because the sound waves travel to the object and back
  distance = (duration * SOUND_SPEED) / 2;

  Serial.print("Distance: ");  
	Serial.print(distance);  
  Serial.println(" cm");

  // Toggle positive LED based on if there is object or not
  digitalWrite(positive_led_pin, distance > distance_threshold ? HIGH : LOW);

  // Display message on LCD based on if there is object or not
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
      // Toggle negative LED when object exist at time interval
      negative_led_state = negative_led_state == LOW ? HIGH : LOW;

      digitalWrite(negative_led_pin, negative_led_state);

      // Toggle buzzer when object exist at time interval
      if (buzzer_buzz) {
        buzzer_buzz = false;
        tone(buzzer_pin, buzzer_tone);
      } else {
        buzzer_buzz = true;
        noTone(buzzer_pin);
      }
    } else {
      // Turn off negative LED when no object
      negative_led_state = LOW;
      digitalWrite(negative_led_pin, negative_led_state);

      // Turn off buzzer when no object
      buzzer_buzz = true;
      noTone(buzzer_pin);
  

      // Move the servo only when there is no object at time interval
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
