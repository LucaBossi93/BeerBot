// MOVEMENT SUPPORT FUNCTIONS //

#define RIGHT 0                     // To turn right when rotating
#define LEFT 1                      // To turn left when rotating
#define DIR_FORWARD 1               // Clockwise
#define DIR_BACKWARD 0              // Counter-clockwise
#define MOVEMENT_TIME 1300          // Maximum time of movement

// VARIABLES //

int STBY = 8;                       // Standby pin

// Motor A: connected between A01 and A02
int PWMA = 2;                       // Speed control
int AIN1 = 3;                       // Direction
int AIN2 = 4;                       // Direction

// Motor B: connected between B01 and B02
int PWMB = 5;                       // Speed control
int BIN1 = 6;                       // Direction
int BIN2 = 7;                       // Direction

long starting_time;                 // Tells when the robot starts to move forward

// SUPPORT FUNCTIONS //

// Movement set-up
void setupMovement() {
  // Set I/O
  pinMode(STBY, OUTPUT);
  // Set motor A pins
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  // Set motor B pins
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
}

// Stops the robot
void stopRobot() {
  digitalWrite(STBY, LOW);
}

// Rotation on the spot
void rotate(int vel, boolean dir) {
  if (dir == LEFT) {
    moveMotor(1, vel, DIR_BACKWARD);
    moveMotor(0, vel, DIR_FORWARD);
  }
  else {
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
  digitalWrite(STBY, HIGH);

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if (dir == 1) {
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if (motor == 1) {
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, sp);
  } else {
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, sp);
  }
}
