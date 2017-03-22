// BEERBOT 0.1 //

// INCLUSIONS //

#include <NewPing.h>

// DEFINITIONS //

#define TRIGGER_PIN_BORDER 7       // Arduino pin 2 tied to trigger pin on the border ultrasonic sensor
#define ECHO_PIN_BORDER 8          // Arduino pin 3 tied to echo pin on the border ultrasonic sensor

#define MAX_DISTANCE_BORDER 200    // Maximum distance we want to ping for (in centimeters)

//motor A connected between A01 and A02
//motor B connected between B01 and B02

int STBY = 0; //standby

//Motor A
int PWMA = 3; //Speed control
int AIN1 = 1; //Direction
int AIN2 = 2; //Direction

//Motor B
int PWMB = 6; //Speed control
int BIN1 = 4; //Direction
int BIN2 = 5; //Direction

// VARIABLES //

// Others
bool groundDetected;               // Tells if there is ground in front of the robot
bool canMove;                      // Tells if the robot can move
bool lookForGround;                // Tells if the robot has to look for ground
bool moveForward;                  // Tells if the robot has to move forwasrd
bool rotateLeft;                   // Tells if the robot has to rotate left or right
int cmSonarBorder;                 // Distance acquired by the border sonar
int groundLimit = 10;              // Maximum distance for which the sonar output is considered ground
int isGroundCounter = 0;           // Counter needed to have an effective detection of ground

// Servo mototors

// Sensors
NewPing sonarBorder(TRIGGER_PIN_BORDER, ECHO_PIN_BORDER, MAX_DISTANCE_BORDER);

// Positions

// Velocities

// Millis
long startSonarBorderAcquisition;
long startStopLookForGround;

// PROGRAM //

void setup() {
  // Initialize the serial port:
  Serial.begin(9600);
  pinMode(STBY, OUTPUT);

  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  // Set the millis
  startSonarBorderAcquisition = millis();

  groundDetected = false;
  canMove = true;
  lookForGround = false;
}

void loop() {
  // Acquire data from sonar
  acquireSonarBorderData();

  // Move
  if (canMove)
    moveRobot();

  // Print infos
  // Serial.print("Grounded: ");
  // Serial.print(groundDetected);
  // Serial.print(", with counter: ");
  // Serial.print(isGroundCounter);
  // Serial.print(" and value: ");
  // Serial.println(cmSonarBorder);
}

// FUNCTIONS //

// Menage the detection of the border
void acquireSonarBorderData() {
  if (millis() - startSonarBorderAcquisition > 10) {
    cmSonarBorder = sonarBorder.ping_cm();

    if (cmSonarBorder < groundLimit && cmSonarBorder != 0) {
      isGroundCounter ++;
    } else {
      isGroundCounter --;
    }

    if (isGroundCounter >= 3) {
      isGroundCounter = 3;
      if (!groundDetected) {
        Serial.print("Ground detected, with value: ");
        Serial.println(cmSonarBorder);
        groundDetected = true;
        setLookForGround(false);
      }
    } else if (isGroundCounter <= 0) {
      isGroundCounter = 0;
      if (groundDetected) {
        Serial.print("No more ground, with value: ");
        Serial.println(cmSonarBorder);
        groundDetected = false;
        if (!lookForGround)
          setLookForGround(true);
      }
    }

    startSonarBorderAcquisition = millis();
  }
}

// Menage the movement of the robot
void moveRobot() {
  // Look for ground if the robot has to
  if (lookForGround) {
    if (rotateLeft) {
      // Rotate left
      move(1, 100, 1); //motor 1, full speed, left
      move(0, 100, 0); //motor 2, full speed, left
    } else {
      // Rotate rigth
      move(1, 100, 0); //motor 1, half speed, right
      move(0, 100, 1); //motor 2, half speed, right
    }
    // If I've detected something I need to stop, I wait one second
    if (startStopLookForGround != 0 && millis() - startStopLookForGround > 1000) {
      lookForGround = false;
      // Stop motors
      Serial.println("I stopped rotating, I'll move forward");
      moveForward = true;
    }
  }
  // Move forward if the robot has to
  if (moveForward) {
    // Move forward
    move(1, 100, 0); //motor 1, half speed, right
    move(0, 100, 0); //motor 2, half speed, right
  }
}

// Menage the robot looking for ground
void setLookForGround(bool b) {
  if (b) {
    Serial.println("I'm looking for ground");
    startStopLookForGround = 0;
    lookForGround = b;
    moveForward = false;
    // Get the sense of rotation;
    rotateLeft = random(2);
  } else {
    Serial.println("I'm going to stop looking for ground");
    startStopLookForGround = millis();
  }
}

void move(int motor, int speed, int direction) {
  //Move specific motor at speed and direction
  //motor: 0 for B 1 for A
  //speed: 0 is off, and 255 is full speed
  //direction: 0 clockwise, 1 counter-clockwise

  digitalWrite(STBY, HIGH); //disable standby

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if (direction == 1) {
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if (motor == 1) {
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed);
  } else {
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, speed);
  }
}

