// movement.ino
// Recipe of funcmoetions regarding the movement

// DEFINITIONS

#define RIGHT 0  // to turn right when rotating
#define LEFT 1  // to turn left when rotating
#define DIR_FORWARD 1  //clockwise
#define DIR_BACKWARD 0  //counter-clockwise
#define MOVEMENT_TIME 1300  // maximum time of movement

int STBY = 8; // standby pin

// MOTOR A: connected between A01 and A02
int PWMA = 2; // speed control 
int AIN1 = 3; // direction
int AIN2 = 4; // direction

// MOTOR B:  connected between B01 and B02
int PWMB = 5; // speed control
int BIN1 = 6; // direction
int BIN2 = 7; // direction

long starting_time;   // when the robot starts to move forward

// FUNCTIONS

// movement set-up
void setupMovement(){
    // setting I/O 
  pinMode(STBY, OUTPUT);

  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
}

// stops the robot
void stopRobot(){
  Serial.println("STOP!");
  digitalWrite(STBY, LOW);
}

// rotation on the spot in order to detect someone
void rotate(int vel, boolean dir){
  if(dir == LEFT){

    Serial.println("I'm rotating left");
    moveMotor(1, vel, DIR_BACKWARD);
    moveMotor(0, vel, DIR_FORWARD);
  }
  else{

    Serial.println("I'm rotating right");
    moveMotor(0, vel, DIR_BACKWARD);
    moveMotor(1, vel, DIR_FORWARD);
  }
}

// move the robot forward
void moveForward(int vel){
  moveMotor(1, vel, DIR_FORWARD);
  moveMotor(0, vel, DIR_FORWARD);
}

// move the robot backward
void moveBackward(int vel){
  starting_time = millis();
  moveMotor(1, vel, DIR_BACKWARD);
  moveMotor(0, vel, DIR_BACKWARD);
  
  // it moves forward for a fixed amount of time
  delay(300);
  Serial.println("I'm moving BACKWARD");
  
  stopRobot();
}

//Move specific motor at speed and direction
void moveMotor(int motor, int sp, boolean dir){

//motor: 0 for B 1 for A
//speed: 0 is off, and 255 is full speed
//direction: 0 clockwise, 1 counter-clockwise

  digitalWrite(STBY, HIGH); //disable standby

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if(dir == 1){
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if(motor == 1){
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, sp);
  }else{
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, sp);
  }
}



