// BEHEVIOUR SUPPORT FUNCTIONS //

// List of states:
// 1 - Rotating looking for people
// 2 - Looking for ground
// 3 - Moving towards someone
// 4 - Moving bacwards
// 5 - Saying hi

// VARIABLES //

// Robot state
int current_state;                      // Current state of the robot
int previous_state;                     // Previous state of the robot

// Miscellaneous
bool resetNeeded;                       // Tells if we need to reset the animation
bool nextAction;                        // Tells if I have to skip to next action
// Millis
long starting_time_action;              // Tells when the robot starts to perform an action

// SUPPORT FUNCTIONS //

// Setup the animation
void setupBeheviour() {
  // Robot is initialized as looking for people
  current_state = 1;
  previous_state = 1;
  resetNeeded = true;
  nextAction = false;

  // Initialize millis
  starting_time_action = millis();
}

// Menage the animation of the robot
void menageBeheviour() {
  switch (current_state) {
    case 1:
      // LOOK FOR PEOPLE - I'm rotating looking for people. I rotate for some time, I cooldown and then I repeat
      // In this phase I don't need to look for ground since I'm rotating in place
      // Detect if there is a person
      peopleDetect();
      // Process the information
      processPeopleDetection();
      // Perform the rotation
      rotateWithCooldown(1500, 300);
      break;
    case 2:
      // LOOK FOR GROUND - I'm looking for ground. I don't need to look for people while I do it
      // Detect if there is an anomaly
      anomalyDetect();
      // Process the information
      processAnomalyDetection();
      // Rotate in place
      rotate(sp, rotate_left);
      break;
    case 3:
      // FORWARD - I've detected a person and I'm moving towards him. I only detect the anomalies but I don't
      // process them since I just want to stop instead of starting looking for ground
      // Detect if there is an anomaly
      anomalyDetect();
      // Process the information
      processAnomalyDetectionLazy();
      // I'm moving towards someone
      moveForwardWithTimeout(1500);
      break;
    case 4:
      // GOODBYE - (TODO)
      break;
    case 5:
      // GREET - I'm saying hi. I look for people, if the time is over or nobody is watching start looking
      // for people again
      // Detect if there is a person
      peopleDetect();
      // Process the information
      processPeopleDetection();
      // Say hi
      sayHi(5000);
      break;
    case 6:
      // ROAMING - Simply move forward and rotate when anomaly is found.
      // Detect if there is an anomaly
      anomalyDetect();
      // Process the information
      processAnomalyDetection();
      // I'm moving forward
      moveForward(sp);
      break;
    case 7:
      // SINGING - (TODO)
      break;
    case 8:
      // SUGGESTION DRIVE - (TODO)
      break;
    case 9:
      // BEER FACT - (TODO)
      break;
    case 10:
      // MUSTACHE SHOW - (TODO)
      break;
    case 11:
      // INVITE - (TODO)
      break;
    case 12:
      // HELP - (TODO)
      break;
    default:
      // Go to the look for people case
      current_state = 1;
      break;
  }
}

// Sets the state of the robot and saves the previous one
void setState(int state) {
  previous_state = current_state;
  current_state = state;
  resetNeeded = true;
}

void resetState() {
  current_state = previous_state;
  resetNeeded = true;
}

// Set all animations
void setAllAnimations(int a, int b, int c) {
  setEyebrowPosition(a);
  setMustacheAnimin(b);
  setTalk(c);
}

// ANIMATIONS //

// Rotate in palce with a cooldown
void rotateWithCooldown(int rotationTime, int cooldown) {
  // Reset the variables if needed
  if (resetNeeded) {
    starting_time_action = millis();
    resetNeeded = false;
    // K_ANIMATOR - Set the animations for this state
    setAllAnimations(2, 1, 2);
  }
  // Rotate
  if (millis() - starting_time_action < rotationTime) {
    rotate(sp, rotate_left);
  } else {
    stopRobot();
    if (millis() - starting_time_action > rotationTime + cooldown) {
      starting_time_action = millis();
    }
  }
}

// Move forward for a certain amout of time
void moveForwardWithTimeout(int timeout) {
  // Reset the variables if needed
  if (resetNeeded) {
    starting_time_action = millis();
    resetNeeded = false;
    // K_ANIMATOR - Set the animations for this state
    setAllAnimations(2, 2, 2);
  }
  // If I don't have detected an anomaly or I've finished, go forward
  if (millis() - starting_time_action > timeout || !isGroundLazy()) {
    stopRobot();
    setState(5);
    Serial.println("STOP!!!");
  } else {
    moveForward(sp);
  }
}

// Interact with the detected person
void sayHi(int timeout) {
  // Reset the variables if needed
  if (resetNeeded) {
    starting_time_action = millis();
    resetNeeded = false;
    // K_ANIMATOR - Set the animations for this state
    setAllAnimations(2, 1, 1);
  }

  // If I've finished search for someone else. PeopleDetection will take care of the case in which
  // everyone left
  if (millis() - starting_time_action > timeout)
    setState(1);
}
