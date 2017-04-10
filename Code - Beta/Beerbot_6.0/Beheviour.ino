// BEHEVIOUR SUPPORT FUNCTIONS //

// DEFINITIONS //

#define WAIT_FOR_ANSWER 10000

// VARIABLES //

// Robot state
int current_state;                      // Current state of the robot
int previous_state;                     // Previous state of the robot
int current_action;                     // Action inside a state

// Miscellaneous
bool resetNeeded;                       // Tells if we need to reset the animation
bool nextState;                         // Tells if I have to skip to next action
// Millis
long starting_time_state;              // Tells when the robot starts to perform a state
long starting_time_action;             // Tells when the robot starts to perform an action

// SUPPORT FUNCTIONS //

// Setup the animation
void setupBeheviour() {
  // Robot is initialized as looking for people
  current_state = 1;
  previous_state = 1;
  resetNeeded = true;
  nextState = false;

  // Initialize millis
  starting_time_state = millis();
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
      // GOODBYE - Stay still and say goodbye
      // Set the animation
      goodbye();
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
      // BEER FACT - Move left and right a bit and talk
      // Setup the animation and talk
      beerFact();
      // Move left and right
      moveLeftRigth(500, 50);
      break;
    case 10:
      // MUSTACHE SHOW - (TODO)
      break;
    case 11:
      // INVITE - (TODO)
      break;
    case 12:
      // HELP - Stay still and call for help
      // Setup the animation
      help();
      break;
    case 20:
      // FOAMTOUCH - Stay still and ask for touching the foam
      foamTouch();
      break;
    case 21:
      // FOAMTOUCHED - Stay still and speak
      foamTouched();
      break;
    case 23:
      // ASKFORGENDER - Stay still and ask for gender
      //
      askForGender();
      break;
    case 24:
      // MANINTERACTION - Stay still and speak
      manInteraction();
      break;
    case 25:
      // WOMANINTERACTION - Stay still and ask speak
      womanInteraction();
      break;
    case 39:
      // TIMEOUT - Stay still and ask speak
      timeout();
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

void resetAndSet(int a, int b, int c) {
  starting_time_state = millis();
  starting_time_action = millis();
  resetNeeded = false;
  current_action = 1;
  // K_ANIMATOR - Set the animations for this state
  setAllAnimations(a, b, c);
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
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 1, 2);
  }
  // Rotate
  if (millis() - starting_time_state < rotationTime) {
    rotate(sp, rotate_left);
  } else {
    stopRobot();
    if (millis() - starting_time_state > rotationTime + cooldown) {
      starting_time_state = millis();
    }
  }
}

// Move forward for a certain amout of time
void moveForwardWithTimeout(int timeout) {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 2, 2);
  }
  // If I don't have detected an anomaly or I've finished, go forward
  if (millis() - starting_time_state > timeout || !isGroundLazy()) {
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
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 1, 1);
  }
  // If I've finished search for someone else. PeopleDetection will take care of the case in which
  // everyone left
  if (millis() - starting_time_state > timeout)
    setState(1);
}

// Tell the person a fact about beer
void beerFact() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 1, 1);
  }
}

// Stay still and call for help
void help() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(3, 4, 1);
    stopRobot();
  }
}

// Stay still and ask for touch the foam
void foamTouch() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 1, 1);
    stopRobot();
  }

  if (millis() - starting_time_state > WAIT_FOR_ANSWER) {
    //Go to state 39 - Timeout
    setState(39);

  }  else if (getFoamTouched ()) {
    //Go to state 21 -  Foam touched
    setState(21);
  }

}
// Stay still and speak
void foamTouched() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 1, 1);
    stopRobot();
  }
  //TO-DO condizione d'uscita

}
// Stay still and ask for gender
void askForGender() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 1, 1);
    stopRobot();
  }

  if (millis() - starting_time_state > WAIT_FOR_ANSWER) {
    //Go to state 39 - Timeout
    setState(39);

  } else if (getHandleTouched ()) {
    //Go to state 25 - Woman interaction
    setState(25);

  } else if (getFoamTouched ()) {
    //Go to state 24 -  Man interaction
    setState(24);
  }

}

// Stay still and speak
void manInteraction() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(3, 1, 1);
    stopRobot();
  }
  //TO-DO condizione d'uscita
}


// Stay still and speak
void womanInteraction() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 1, 1);
    stopRobot();
  }
  //TO-DO condizione d'uscita
}


// Stay still and speak
void timeout() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(3, 1, 1);
    stopRobot();
  }
  //TO-DO condizione d'uscita
}
// Stay still and say goodbye
void goodbye() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 1, 1);
    stopRobot();
  }
}

// MOVEMENTS //

// Moves left and right
void moveLeftRigth(int oscillation_time, int velocity) {
  switch (current_action) {
    case 1:
      rotate(velocity, true);
      current_action = 2;
      break;
    case 2:
      // If I've finished with this action perform the next one
      if (millis() - starting_time_action > oscillation_time / 2) {
        rotate(velocity, false);
        current_action = 3;
      }
      break;
    case 3:
      // If I've finished with this action perform the next one
      if (millis() - starting_time_action > oscillation_time * 3 / 2) {
        rotate(velocity, true);
        current_action = 3;
      }
      break;
    case 4:
      // If I've finished with this action perform the next one
      if (millis() - starting_time_action > oscillation_time * 2) {
        starting_time_action = millis();
        rotate(velocity, true);
        current_action = 3;
      }
      break;
    default:
      current_action = 1;
      break;
  }
}
