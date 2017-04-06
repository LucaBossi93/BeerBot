// BEERBOT 6.0 //

// INCLUSIONS //

#include <NewPing.h>
#include <SharpIR.h>

// DEFINITIONS //

#define TRIGGER_PIN_DETECTION 12        // Arduino pin 12 tied to trigger pin of the detection sonar
#define ECHO_PIN_DETECTION 11           // Arduino pin 11 tied to echo pin of the detection sonar
#define MAX_DISTANCE_BORDER 200         // Maximum distance we want to ping for (in centimeters)
#define MAX_DISTANCE_DETECTION 150      // Maximum distance we want to ping for (in centimeters)
#define INTERACTION_TIME 5000           // Maximum time of interaction with the user 
#define ROTATION_TIME 1000              // Maximum time of rotation 
#define IR_PIN_LEFT A0                  // Arduino analog pin 0 tied to the left IR sensor
#define IR_PIN_RIGTH A1                 // Arduino analog pin 1 tied to the rigth IR sensor
#define model 1080                      // Model of the IR sensor

// VARIABLES //

// Miscellaneous
int backwardDelay = 300;                // Amount of time the robot has to move backward
int forwardDelay = 2000;                // Amount of time the robot has to move forward
int sp = 250;                           // Default speed
bool rotate_left;                       // Tells if the robot has to rotate left or right

// Sensors
NewPing detection_sonar(TRIGGER_PIN_DETECTION, ECHO_PIN_DETECTION, MAX_DISTANCE_DETECTION);
SharpIR left_IR(IR_PIN_LEFT, model);
SharpIR rigth_IR(IR_PIN_RIGTH, model);

// CODE //

void setup() {
  // Initialize the serial port
  Serial.begin(9600);
  // Set a seed for the random number generator
  randomSeed(1234);

  // Setup everything
  setupAll();

  // K_ANIMATOR - Initialize animations
  setTalk(2);
  setEyebrowPosition(1);
  setMustacheAnimin(2);
}

// TODO: for now I'll make everything modular, then I'll optimize by calling stuff only when needed
void loop() {
  // Menage the beheviour of the robot depending on the current state
  menageBeheviour();
  // K_ANIMATOR - Manage the mustache animation
  menageMustache();
  // K_ANIMATOR - Manage the eyebrows animation
  menageEyebrow();
  // K_ANIMATOR - Menage the sound
  menageSound();
}

// SUPPORT FUNCTIONS //

void setupAll() {
  // Initialize pins and whatever regarding the movement
  setupMovement();
  // Setup the sound system
  setupSound();
  // Setup of the mustache movement
  setupMustache();
  // Setup of the eyebrow movement
  setupEyebrow();
  // Setup the anomaly detection
  setupAnomalyDetection();
  // Setup people detection
  setupPeopleDetection();
  // Setup beheviour
  setupBeheviour();
}

// Setter for the sense of rotation
void setRotateLeft(int b) {
  rotate_left = b;
}

// Getter for the sense of rotation
int getRotateLeft() {
  return rotate_left;
}
