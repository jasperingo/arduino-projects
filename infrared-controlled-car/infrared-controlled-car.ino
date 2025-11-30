/**
 * This project uses a 2WD Chassis, 2 DC motors, L298N motor driver, a switch, Infrared sensor, and Infrared remote
 * The DC motors are connected to the motor driver
 * The power supply is connected to the motor driver and board through the switch
 * The motor driver is connected to output pins on the board
 * The motor driver translates digital voltages from the pins and uses it to control the DC motors rotation
 * Setting both pins of a DC motor to LOW stops rotation
 * Setting one pin HIGH and the other LOW is used to control the rotation direction of the DC motor
 * The car can be rotated by stopping one DC motor and making the other one rotate
 * The infrared sensor transalates key presses from the remote to motion change or speed change
 * The arrow keys are used to control the movement in different directions
 * The OK (center) key is used to stop the movement
 * The keys 1 - 9 are used to change the motor rotation speeds using pulse width modulation
*/

#include <IRremote.hpp>

#define ONE_REMOTE_KEY 0xBA45FF00
#define TWO_REMOTE_KEY 0xB946FF00
#define THREE_REMOTE_KEY 0xB847FF00
#define FOUR_REMOTE_KEY 0xBB44FF00
#define FIVE_REMOTE_KEY 0xBF40FF00
#define SIX_REMOTE_KEY 0xBC43FF00
#define SEVEN_REMOTE_KEY 0xF807FF00
#define EIGHT_REMOTE_KEY 0xEA15FF00
#define NINE_REMOTE_KEY 0xF609FF00
#define UP_REMOTE_KEY 0xE718FF00
#define DOWN_REMOTE_KEY 0xAD52FF00
#define LEFT_REMOTE_KEY 0xF708FF00
#define RIGHT_REMOTE_KEY 0xA55AFF00
#define OK_REMOTE_KEY 0xE31CFF00

const uint8_t ir_receive_pin = 11;

const uint8_t right_speed_pin = 10;
const uint8_t right_top_pin = 4;
const uint8_t right_bottom_pin = 5;
const uint8_t left_speed_pin = 9;
const uint8_t left_top_pin = 6;
const uint8_t left_bottom_pin = 7;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(ir_receive_pin, ENABLE_LED_FEEDBACK); // Start the receiver

  pinMode(left_speed_pin, OUTPUT);
  pinMode(left_top_pin, OUTPUT);
  pinMode(left_bottom_pin, OUTPUT);
  pinMode(right_speed_pin, OUTPUT);
  pinMode(right_top_pin, OUTPUT);
  pinMode(right_bottom_pin, OUTPUT);
}

void loop() {
  if (IrReceiver.decode()) {
    uint32_t remote_key = IrReceiver.decodedIRData.decodedRawData;

    Serial.print("IR value: ");
    Serial.println(remote_key, HEX); // Print "old" raw data

    switch (remote_key) {
      case OK_REMOTE_KEY:
        move_none();
      break;
      
      case UP_REMOTE_KEY:
        move_forward();
      break;
      
      case LEFT_REMOTE_KEY:
        move_left();
      break;
      
      case RIGHT_REMOTE_KEY:
        move_right();
      break;
      
      case DOWN_REMOTE_KEY:
        move_backward();
      break;
      
      case ONE_REMOTE_KEY:
        change_speed(30);
      break;
      
      case TWO_REMOTE_KEY:
        change_speed(60);
      break;
      
      case THREE_REMOTE_KEY:
        change_speed(90);
      break;
      
      case FOUR_REMOTE_KEY:
        change_speed(120);
      break;
      
      case FIVE_REMOTE_KEY:
        change_speed(150);
      break;
      
      case SIX_REMOTE_KEY:
        change_speed(180);
      break;
      
      case SEVEN_REMOTE_KEY:
        change_speed(210);
      break;
      
      case EIGHT_REMOTE_KEY:
        change_speed(235);
      break;
      
      case NINE_REMOTE_KEY:
        change_speed(255);
      break;
    }

    IrReceiver.resume(); // Enable receiving of the next value
  }
}

void change_speed(uint8_t pwm_value) {
  Serial.print("Change speed to: ");
  Serial.println(pwm_value);

  analogWrite(left_speed_pin, pwm_value);
  analogWrite(right_speed_pin, pwm_value);
}

void move_none() {
  Serial.println("Stop movement");

  digitalWrite(left_top_pin, LOW);
  digitalWrite(left_bottom_pin, LOW);
  digitalWrite(right_top_pin, LOW);
  digitalWrite(right_bottom_pin, LOW);
}

void move_forward() {
  Serial.println("Move forward");

  digitalWrite(left_top_pin, HIGH);
  digitalWrite(left_bottom_pin, LOW);
  digitalWrite(right_top_pin, HIGH);
  digitalWrite(right_bottom_pin, LOW);
}

void move_backward() {
  Serial.println("Move backward");

  digitalWrite(left_top_pin, LOW);
  digitalWrite(left_bottom_pin, HIGH);
  digitalWrite(right_top_pin, LOW);
  digitalWrite(right_bottom_pin, HIGH);
}

void move_left() {
  Serial.println("Move left");

  digitalWrite(left_top_pin, LOW);
  digitalWrite(left_bottom_pin, LOW);
  digitalWrite(right_top_pin, LOW);
  digitalWrite(right_bottom_pin, HIGH);
}

void move_right() {
  Serial.println("Move right");

  digitalWrite(left_top_pin, LOW);
  digitalWrite(left_bottom_pin, HIGH);
  digitalWrite(right_top_pin, LOW);
  digitalWrite(right_bottom_pin, LOW);
}
