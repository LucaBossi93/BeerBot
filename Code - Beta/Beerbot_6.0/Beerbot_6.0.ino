// BEERBOT 6.0 //

// VARIABLES //

// Miscellaneous
int backwardDelay = 300;                // Amount of time the robot has to move backward
int forwardDelay = 2000;                // Amount of time the robot has to move forward
int sp = 250;                           // Default speed
bool rotate_left;                       // Tells if the robot has to rotate left or right

// CODE //

void setup() {
  // Initialize the serial port
  Serial.begin(9600);
  // Set a seed for the random number generator
  randomSeed(1234);

  // Setup everything
  setupAll();

  // K_ANIMATOR - Initialize animations
  setTalk(0, false);
  setEyebrowPosition(1);
  setMustacheAnimin(2);
}

void loop() {
  // Menage the beheviour of the robot depending on the current state
  menageBeheviour();
  // K_ANIMATOR - Manage the mustache animation
  menageMustache();
  // K_ANIMATOR - Manage the eyebrows animation
  menageEyebrow();
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
