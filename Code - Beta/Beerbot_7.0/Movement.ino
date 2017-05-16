// MOVEMENT SUPPORT FUNCTIONS //

// DEFINITIONS //

#define RIGHT 0                         // To turn right when rotating
#define LEFT 1                          // To turn left when rotating
#define DIR_FORWARD 1                   // Clockwise
#define DIR_BACKWARD 0                  // Counter-clockwise
#define MOVEMENT_TIME 1300              // Maximum time of movement

#define MOTOR_PIN_PWMA 6                // Arduino digital pin 38 tied to motor A speed control
#define MOTOR_PIN_AIN2 40               // Arduino digital pin 40 tied to motor A direction
#define MOTOR_PIN_AIN1 42               // Arduino digital pin 42 tied to motor A direction
#define MOTOR_PIN_STBY 44               // Arduino digital pin 44 tied to the motors standby
#define MOTOR_PIN_BIN1 46               // Arduino digital pin 46 tied to motor B direction
#define MOTOR_PIN_BIN2 48               // Arduino digital pin 48 tied to motor B direction
#define MOTOR_PIN_PWMB 7                // Arduino digital pin 50 tied to motor B speed control

// VARIABLES //

// Millis
long starting_time;                 // Tells when the robot starts to move forward

// SUPPORT FUNCTIONS //

// Movement set-up
void setupMovement() {
  // Set I/O
  pinMode(MOTOR_PIN_STBY, OUTPUT);
  // Set motor A pins
  pinMode(MOTOR_PIN_PWMA, OUTPUT);
  pinMode(MOTOR_PIN_AIN1, OUTPUT);
  pinMode(MOTOR_PIN_AIN2, OUTPUT);
  // Set motor B pins
  pinMode(MOTOR_PIN_PWMB, OUTPUT);
  pinMode(MOTOR_PIN_BIN1, OUTPUT);
  pinMode(MOTOR_PIN_BIN2, OUTPUT);
}

// Stops the robot
void stopRobot() {
  digitalWrite(MOTOR_PIN_STBY, LOW);
}

// Rotation on the spot
void rotate(int vel, boolean dir) {
  if (dir == LEFT) {
    moveMotor(1, vel, DIR_BACKWARD);
    moveMotor(0, vel, DIR_FORWARD);
  } else {
    moveMotor(0, vel, DIR_BACKWARD);
    moveMotor(1, vel, DIR_FORWARD);
  }
}

// Move the robot forward
void moveForward(int vel) {
  moveMotor(1, vel, DIR_FORWARD);
  moveMotor(0, vel, DIR_FORWARD);
}

// Move the robot backward
void moveBackward(int vel) {
  moveMotor(1, vel, DIR_BACKWARD);
  moveMotor(0, vel, DIR_BACKWARD);
}

// Move specific motor at a certain speed and direction
void moveMotor(int motor, int sp, boolean dir) {
  // Motor: 0 for B 1 for A
  // Speed: 0 is off, and 255 is full speed
  // Direction: 0 clockwise, 1 counter-clockwise

  // Disable standby
  digitalWrite(MOTOR_PIN_STBY, HIGH);

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if (dir == 1) {
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if (motor == 1) {
    digitalWrite(MOTOR_PIN_AIN1, inPin1);
    digitalWrite(MOTOR_PIN_AIN2, inPin2);
    analogWrite(MOTOR_PIN_PWMA, sp);
  } else {
    digitalWrite(MOTOR_PIN_BIN1, inPin1);
    digitalWrite(MOTOR_PIN_BIN2, inPin2);
    analogWrite(MOTOR_PIN_PWMB, sp);
  }
}
