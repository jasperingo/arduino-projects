/**
 * This project uses two LEDs, Servo motor, Ultrasound sensor & Paper can
 * The utltrasound sensor and LEDs are set up in front of the paper can
 * The Servo motor is set up inside the paper can, close to its lid (cover)
 * When an object is detected, the servo rotates to an angle that opens the can
 * The green LED is turned on and the red LED is turned off
 * After some times of no object detected, the servo rotates to an angle that closes the can
 * The red LED is turned on and the green LED is turned off
*/

#include <Servo.h>

#define SERVO_MIN 40
#define SERVO_MAX 110
#define ULTRASOUND_DELAY 10
#define SOUND_SPEED 0.0343 // speed of sound in centimeters per microsecond
#define OPEN_DISTANCE_THRESHOLD 20
#define CLOSE_DURATION_THRESHOLD 2000

const uint8_t servo_pin = 6;
const uint8_t can_opened_led_pin = 3;
const uint8_t can_closed_led_pin = 2;
const uint8_t ultrasound_echo_pin = 7;
const uint8_t ultrasound_trig_pin = 4;

Servo servo;
uint32_t previous_time = 0;

void setup() {
  Serial.begin(9600);

  servo.attach(servo_pin);
  servo.write(SERVO_MAX);

  pinMode(can_opened_led_pin, OUTPUT);
  digitalWrite(can_opened_led_pin, LOW);

  pinMode(can_closed_led_pin, OUTPUT);
  digitalWrite(can_closed_led_pin, LOW);

  
  pinMode(ultrasound_echo_pin, INPUT);
  pinMode(ultrasound_trig_pin, OUTPUT);

  digitalWrite(ultrasound_trig_pin, LOW);
  delayMicroseconds(2);
}

void loop() {
  long duration;
  float distance;

  uint32_t current_time = millis();

  // Read distance from object with ultrasound
  digitalWrite(ultrasound_trig_pin, HIGH);
  delayMicroseconds(ULTRASOUND_DELAY);

  digitalWrite(ultrasound_trig_pin, LOW);

  duration = pulseIn(ultrasound_echo_pin, HIGH);

  Serial.print("Duration: ");  
	Serial.print(duration);  
  Serial.print(" us - ");

  // Convert time to distance
  // Divide it by 2 because the sound waves travel to the object and back
  distance = (duration * SOUND_SPEED) / 2;

  Serial.print("Distance: ");  
	Serial.print(distance);  
  Serial.println(" cm");

  if (distance <= OPEN_DISTANCE_THRESHOLD) {
    previous_time = current_time;
    servo.write(SERVO_MIN);
    digitalWrite(can_closed_led_pin, LOW);
    digitalWrite(can_opened_led_pin, HIGH);
  } else if ((current_time - previous_time) >= CLOSE_DURATION_THRESHOLD) {
    previous_time = current_time;
    servo.write(SERVO_MAX);
    digitalWrite(can_opened_led_pin, LOW);
    digitalWrite(can_closed_led_pin, HIGH);
  }
}
