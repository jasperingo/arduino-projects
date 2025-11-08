/**
 * This project uses a 2WD Chassis, 2 DC motors, L298N motor driver, a switch, and Ultrasound sensor
 * The DC motors are connected to the motor driver
 * The power supply is connected to the motor driver and board through the switch
 * The motor driver is connected to output pins on the board
 * The motor driver translates digital voltages from the pins and uses it to control the DC motors rotation
 * Setting both pins of a DC motor to LOW stops rotation
 * Setting one pin HIGH and the other LOW is used to control the rotation direction of the DC motor
 * The car can be rotated by stopping one DC motor and making the other one rotate
 * The ultrasound sensor reads the distance from the car to any object ahead of it
 * If the distance is too close, the car is rotated in either direction (left, right) to avoid hitting the object
*/

#define ULTRASOUND_DELAY 10
#define SOUND_SPEED 0.0343 // speed of sound in centimeters per microsecond
#define OBSTACLE_DISTANCE_THRESHOLD 30

const uint8_t right_top_pin = 4;
const uint8_t right_bottom_pin = 5;
const uint8_t left_top_pin = 6;
const uint8_t left_bottom_pin = 7;
const uint8_t ultrasound_trig_pin = 8;
const uint8_t ultrasound_echo_pin = 9;

void setup() {
  Serial.begin(9600);

  pinMode(left_top_pin, OUTPUT);
  pinMode(left_bottom_pin, OUTPUT);
  pinMode(right_bottom_pin, OUTPUT);
  pinMode(right_top_pin, OUTPUT);
  
  pinMode(ultrasound_echo_pin, INPUT);
  pinMode(ultrasound_trig_pin, OUTPUT);
}

void loop() {
  long duration;
  float distance;
  float rotate_direction;

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

  if (distance > 0 && distance <= OBSTACLE_DISTANCE_THRESHOLD) {
    rotate_direction = random(2);
    
    Serial.print("Rotate value: ");
    Serial.println(rotate_direction);

    if (rotate_direction == 0) {
      rotate_right();
      Serial.println("Right");
    } else {
      rotate_left();
      Serial.println("Left");
    }

    delay(800);
  } else {
    forward();
    Serial.println("Forward");
  }
}

void forward() {  
  digitalWrite(left_top_pin, HIGH);
  digitalWrite(left_bottom_pin, LOW);
  digitalWrite(right_top_pin, HIGH);
  digitalWrite(right_bottom_pin, LOW);
}

void rotate_left() {
  digitalWrite(left_top_pin, LOW);
  digitalWrite(left_bottom_pin, LOW);
  digitalWrite(right_top_pin, LOW);
  digitalWrite(right_bottom_pin, HIGH);
}

void rotate_right() {
  digitalWrite(left_top_pin, LOW);
  digitalWrite(left_bottom_pin, HIGH);
  digitalWrite(right_top_pin, LOW);
  digitalWrite(right_bottom_pin, LOW);
}
