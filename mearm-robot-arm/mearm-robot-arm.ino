/**
 * This project uses Four Servo motors, & Wooden MeArm kit
 * Set up the MeArm kit according to this: https://youtu.be/Q9JOKQaIR1w?si=FH6BwU4QdSnlqUQY
 * The claw servo is programmed to close the claw. Picking up any object in between it
 * The left and right servo will move in sequence to lift the object from the ground
 * The bottom servo will rotate to move the object to the other side
 * Finally the motion sequence is executed in a reverse order to drop the item where it was intially
 * This is just to verify the set up of the MeArm kit
*/

#include <Servo.h>

#define MOVE_ARM_DELAY_TIME 2000
#define MOVE_SERVO_DELAY_TIME 20
#define TOP_SERVO_MIN 0
#define TOP_SERVO_MAX 40
#define BOTTOM_SERVO_MIN 0
#define BOTTOM_SERVO_MAX 80
#define LEFT_SERVO_MIN 0
#define LEFT_SERVO_MAX 90
#define RIGHT_SERVO_MIN 140
#define RIGHT_SERVO_MAX 180

const uint8_t top_servo_pin = 6;
const uint8_t bottom_servo_pin = 5;
const uint8_t left_servo_pin = 9;
const uint8_t right_servo_pin = 10;

Servo top_servo;
Servo bottom_servo;
Servo left_servo;
Servo right_servo;

void setup() {
  Serial.begin(9600);

  top_servo.attach(top_servo_pin);
  bottom_servo.attach(bottom_servo_pin);
  left_servo.attach(left_servo_pin);
  right_servo.attach(right_servo_pin);
}

void loop() {
  top_servo.write(TOP_SERVO_MIN);

  delay(MOVE_ARM_DELAY_TIME);

  moveServo(&left_servo, LEFT_SERVO_MIN, LEFT_SERVO_MAX);

  delay(MOVE_ARM_DELAY_TIME);

  moveServo(&right_servo, RIGHT_SERVO_MAX, RIGHT_SERVO_MIN);
  
  delay(MOVE_ARM_DELAY_TIME);

  moveServo(&bottom_servo, BOTTOM_SERVO_MIN, BOTTOM_SERVO_MAX);

  delay(MOVE_ARM_DELAY_TIME);

  moveServo(&bottom_servo, BOTTOM_SERVO_MAX, BOTTOM_SERVO_MIN);

  delay(MOVE_ARM_DELAY_TIME);

  moveServo(&right_servo, RIGHT_SERVO_MIN, RIGHT_SERVO_MAX);
  
  delay(MOVE_ARM_DELAY_TIME);
  
  moveServo(&left_servo, LEFT_SERVO_MAX, LEFT_SERVO_MIN);
  
  delay(MOVE_ARM_DELAY_TIME);

  top_servo.write(TOP_SERVO_MAX);

  delay(MOVE_ARM_DELAY_TIME);
}

void moveServo(Servo* servo, uint8_t start, uint8_t stop) {
  uint8_t i = start;

  if (stop > start) {
    for (; i < stop; i++) {
      servo->write(i); 
      delay(MOVE_SERVO_DELAY_TIME);
    }
  } else if (start > stop) {
    for (; i > stop; i--) {
      servo->write(i); 
      delay(MOVE_SERVO_DELAY_TIME);
    }
  }
}
