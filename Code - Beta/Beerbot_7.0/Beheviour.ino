// BEHEVIOUR SUPPORT FUNCTIONS //

// DEFINITIONS //

// #define CHANGE_RANDOMLY 100000
// #define GUESS 42
#define WAIT_FOR_ANSWER 5000
#define FORWARD_VELOCITY 180
#define BACKWARD_VELOCITY 170
#define ROTATE_FAST_VELOCITY 190
#define ROTATE_MEDIUM_VELOCITY 170
#define EXTRA_ROTATION_TIME 400
#define EXTRA_BACKWARD_TIME 400

// VARIABLES //

// Robot state
int current_state;                      // Current state of the robot
int previous_state;                     // Previous state of the robot
int next_state;                         // Next state of the robot (setted by certain states only)
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
  current_state = 0;
  previous_state = 0;
  resetNeeded = true;
  nextState = false;

  // Initialize millis
  starting_time_state = millis();
}

// Sets the state of the robot and saves the previous one
void setState(int state) {
  previous_state = current_state;
  current_state = state;
  resetNeeded = true;
  Serial.print("State: ");
  Serial.println(state);
}

// Sets the state as one of the two idle  ones
void setStateIdle() {
  if (random(2))
    // Go to state 3 (LOOK FOR PEOPLE)
    setState(3);
  else
    // Go to state 4 (ROAMING)
    setState(4);
}

// Sets the previous state
void resetState() {
  current_state = previous_state;
  resetNeeded = true;
}

// Resets the state and sets all the animations
void resetAndSet(int a, int b, int c, bool rep) {
  starting_time_state = millis();
  starting_time_action = millis();
  resetNeeded = false;
  current_action = 1;
  // K_ANIMATOR - Set the animations for this state
  setAllAnimations(a, b, c, rep);
}

// Set all animations
void setAllAnimations(int a, int b, int c, boolean rep) {
  setEyebrowPosition(a);
  setMustacheAnimin(b);
  setTalk(c, rep);
}

// Menage the animation of the robot
void menageBeheviour() {
  switch (current_state) {
    case 0:
      // CALIBRATION - Claibrate the sensors staying still for 5 seconds
      // Calibrate the temperature sensor
      calibrateTemperature();
      // Process the information and start to operate
      if (millis() - starting_time_state > 1000) {
        processTemperatureCalibration();
        setState(4);
      }
      break;
    case 1:
      // FORWARD - I've detected a person and I'm moving towards him. I only detect the anomalies but I don't
      // process them since I just want to stop instead of starting looking for ground
      // I'm moving towards someone
      moveForwardWithTimeoutAnim(1500);
      // Detect if there is an anomaly
      anomalyDetect();
      // Process the information
      processAnomalyDetectionLazy();
      break;
    case 2:
      // LOOK FOR GROUND - I'm looking for ground. I don't need to look for people while I do it
      // Setup the animation and talk
      lookForGroundAnim();
      // Detect if there is an anomaly
      anomalyDetect();
      // Process the information
      processAnomalyDetection();
      break;
    case 3:
      // LOOK FOR PEOPLE - I'm rotating looking for people. I rotate for some time, I cooldown and then I repeat
      // In this phase I don't need to look for ground since I'm rotating in place
      // Setup the animation and talk
      lookForPeopleAnim();
      // Detect if there is a person
      peopleDetect();
      // Process the information
      processPeopleDetection();
      // Manage unexpected touch
      // unexpectedTouchDetection();
      break;
    case 4:
      // ROAMING - Simply move forward and rotate when anomaly is found.
      // Setup the animation and talk
      roamingAnim();
      // Detect if there is an anomaly
      anomalyDetect();
      // Process the information
      processAnomalyDetection();
      // Detect if there is a person
      peopleDetect();
      // Process the information
      processPeopleDetection();
      // Manage unexpected touch
      // unexpectedTouchDetection();
      break;
    case 5:
      // SCARED NO GROUND - I'm scared. Talk and move back a little.
      // Setup the animation
      scaredNoGroundAnim();
      break;
    case 6:
      // BEER FACT - Move left and right a bit and talk
      // Setup the animation and talk
      beerFactAnim();
      // Move left and right
      // moveLeftRigth(1000, sp * 1.2);
      break;
    case 7:
      // MOVE BACKWARDS AFTER GREETING
      if (millis() - starting_time_state > EXTRA_BACKWARD_TIME) {
        stopRobot();
         setState(8);
      }
      break;
    case 8:
      // GREET - I'm saying hi. I look for people, if the time is over or nobody is watching start looking
      // for people again
      // Say hi
      greetAnim();
      // Detect if there is a person
      // peopleDetect();
      // Process the information
      // processPeopleDetection();
      break;
    case 9:
      // INVITE - Invite people inside
      // Setup the animation
      inviteAnim();
      break;
    case 10:
      // GOODBYE - Stay still and say goodbye
      // Set the animation
      goodbyeAnim();
      break;
    case 11:
      // HELP - Stay still and call for help (TODO)
      // Setup the animation and talk
      helpAnim();
      // Detect if there is an anomaly
      anomalyDetect();
      // Process the information
      processAnomalyDetection();
      break;
    case 12:
      // COUNT PEOPLE - Count how much people there is
      // Setup the animation
      countPeopleAnim();
      // Detect if there is a person
      peopleDetect();
      // Process the information
      processPeopleDetectionLazy();
      // Rotate in place
      rotate(ROTATE_MEDIUM_VELOCITY, rotate_left);
      // Count people
      peopleCount();
      break;
    case 13:
      // INTERACTION SINGLE PERSON - Talk with a single person
      // Setup the animation
      interactionSinglePersonAnim();
      break;
    case 14:
      // INTERACTION MULTIPLE PERSON - Talk with a group
      // Setup the animation
      interactionMultiplePersonAnim();
      break;
    case 15:
      // STATICPEOPLE - Be angry with people who are staying still looking at you
      staticPeopleAnim();
      break;
    case 16:
      // BEER GAME INVITE - Stay still and invite people to play
      // Setup the animation
      beerGameInviteAnim();
      break;
    case 17:
      // BEER GAME SELECTION - Select two persons
      // Setup the animation, look for people and select them
      beerGameSelectionAnim();
      // Detect the people
      peopleDetect();
      // Detect the people
      processPeopleDetectionLazy();
      break;
    case 18:
      // BEER GAME END - Stay still and invite to kiss or have a beer
      // Setup the animation
      beerGameEndAnim();
      break;
    case 19:
      // STATIC CHECK - Stay still and wait for people to leave
      // Setup the animation
      staticCheckAnim();
      // Detect the people
      peopleDetect();
      // Detect the people
      processPeopleDetectionLazy();
      break;
    case 20:
      // FOAM TOUCH - Stay still and ask for touching the foam
      // Setup the animation
      foamTouchAnim();
      // look for touch
      expectedTouchDetection();
      break;
    case 21:
      // FOAM TOUCHED - Stay still and speak
      // Setup the animation
      foamTouchedAnim();
      break;
    case 22:
      // FOAM TOUCHED SURPRISED - Stay still and speak
      // Setup the animation
      foamTouchedSurprisedAnim();
      break;
    case 23:
      // ASK FOR GENDER - Stay still and ask for gender
      // Setup the animation
      askGenderAnim();
      // Touch detection
      expectedTouchDetection();
      break;
    case 24:
      // MAN INTERACTION - Stay still and speak
      // Setup the animation
      manInteractionAnim();
      break;
    case 25:
      // WOMAN INTERACTION - Stay still and speak
      // Setup the animation
      womanInteractionAnim();
      break;
    case 26:
      // BEER LIFT REQUEST - Ask the user to lift you and wait for his interaction
      expectedTouchDetection();
      // Setup the animation
      beerLiftRequestAnim();
      break;
    case 27:
      // HANDLE TOUCHED INTERACTION - Stay still and speak
      // Setup the animation
      handleTouchedInteractionAnim();
      break;
    case 28:
      // HANDLE TOUCHED SURPRISED - Stay still and speak
      // Setup the animation
      handleTouchedSurprisedAnim();
      break;
    case 29:
      // REQUEST CLAPPING - Stay still and speak
      requestClappingAnim();
      break;
    case 30:
      // LOW CLAPPING - Stay still and speak
      lowClappingAnim();
      break;
    case 31:
      // HIGH CLAPPING - Stay still and speak
      highClappingAnim();
      break;
    case 32:
      // REQUEST SCREAMING: - Stay still and speak
      // Setup the animation
      requestScreamingAnim();
      break;
    case 33:
      // LOW SCREAMING: - Stay still and listen
      // Setup the animation
      lowScreamingAnim();
      break;
    case 34:
      // MED SCREAMING: - Stay still and listen
      // Setup the animation
      medScreamingAnim();
      break;
    case 35:
      // HIGH SCREAMING: - Stay still and speak
      // Setup the animation
      highScreamingAnim();
      break;
    case 36:
      // REACT CLAPPING - Stay still and speak
      reactClappingAnim();
      break;
    case 37:
      // MAGIC SHOW BEGIN - (TODO)
      magicShowBeginAnim();
      break;
    case 38:
      // MAGIC SHOW END - (TODO)
      magicShowEndAnim();
      break;
    case 39:
      // TIMEOUT - Stay still and speak
      // Setup the animation
      timeoutAnim();
      break;
    case 40:
      //ASK PREFERENCE - Stay still and speak
      askPreferenceAnim();
      // Touch detection
      expectedTouchDetection();
      break;
    case 41:
      // WINE PREFERENCE - Stay still and speak
      winePreferenceAnim();
      break;
    case 42:
      // BEER PREFERENCE - Stay still and speak
      beerPreferenceAnim();
      break;
    default:
      // Go to the look for people case
      current_state = 1;
      break;
  }
}

// ANIMATIONS //

// Rotate in palce with a cooldown
void rotateWithCooldownAnim(int rotationTime, int cooldown) {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 1, 3, false);
  }
  // Rotate
  if (millis() - starting_time_state < rotationTime) {
    rotate(ROTATE_MEDIUM_VELOCITY, rotate_left);
  } else {
    stopRobot();
    if (millis() - starting_time_state > rotationTime + cooldown) {
      starting_time_state = millis();
    }
  }
}

// Move forward for a certain amout of time
void moveForwardWithTimeoutAnim(int timeout) {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 2, 3, false);
    moveForward(FORWARD_VELOCITY);
  }
  // If I don't have detected an anomaly or I've finished, go forward
  if (!isGroundLazy()) {
    moveBackward(BACKWARD_VELOCITY);
    setState(7);
  } else if (millis() - starting_time_state > timeout) {
    stopRobot();
    // Go to state 8 (GREET)
    setState(8);
  }
}

// Interact with the detected person
void greetAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 1, 6, false);
  }
  // If I've finished search for someone else. PeopleDetection will take care of the case in which
  // everyone left
  if (millis() - starting_time_state > getPlayDuration()) {
    switch (random(4)) {
      case 0:
        next_state = 12;
        // Go to state 6 (BEER FACT)
        setState(6);
        break;
      case 1:
        //Go to state 23 (ASK GENDER)
        setState(23);
        break;
      case 2:
        // Go to state 26 (BEER LIFT REQUEST)
        setState(26);
        break;
      case 3:
        // Go to state 20 (FOAM TOUCH)
        setState(20);
        break;
    }
  }
}

// Tell the person a fact about beer
void beerFactAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 1, 4, false);
  }
  // Go to the next action when done
  if (millis() - starting_time_state > getPlayDuration()) {
    // Go to next state
    setState(next_state);
  }
}

// Stay still and call for help
void helpAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(3, 4, 9, false);
    stopRobot();
  }
}

// Stay still and ask for touch the foam
void foamTouchAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 1, 16, false);
    stopRobot();
  }
  // Wait for input or timeout
  if (millis() - starting_time_state > WAIT_FOR_ANSWER + getPlayDuration()) {
    // Go to state 39 (TIMEOUT)
    setState(39);
  }  else if (getFoamTouched() && millis() - starting_time_state > getPlayDuration()) {
    // Go to state 21 (FOAM TOUCHED)
    setState(21);
  }
}

// Stay still and speak
void foamTouchedAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 4, 17, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration()) {
    if (random(4) == 0)
      // Go to state 19 (STATIC CHECK)
      setState(19);
    else
      // Go to state 12 (COUNT PEOPLE)
      setState(12);
  }
}

// Stay still and speak
void foamTouchedSurprisedAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 2, 18, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration()) {
    // Go to state 3 (LOOK FOR PEOPLE)
    setState(3);
  }
}

// Stay still and ask for gender
void askGenderAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 1, 19, false);
    stopRobot();
  }
  // Wait for input or timeout
  if (millis() - starting_time_state > WAIT_FOR_ANSWER + getPlayDuration() ) {
    // Go to state 39 (TIMEOUT)
    setState(39);
  } else if (getHandleTouched() && millis() - starting_time_state > getPlayDuration()) {
    // Go to state 25 (WOMAN INTERACTION)
    setState(25);
  } else if (getFoamTouched() && millis() - starting_time_state > getPlayDuration()) {
    // Go to state 24 (MAN INTERACTION)
    setState(24);
  }
}

// Stay still and speak
void manInteractionAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(3, 4, 20, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration())
  {
    if (random(4) == 0)
      // Go to state 19 (STATIC CHECK)
      setState(19);
    else
      // Go to state 12 (COUNT PEOPLE)
      setState(12);
  }
}

// Stay still and speak
void womanInteractionAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 5, 21, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration())
  {
    if (random(4) == 0)
      // Go to state 19 (STATIC CHECK)
      setState(19);
    else
      // Go to state 12 (COUNT PEOPLE)
      setState(12);
  }
}

// Stay still and ask for clapping
void requestClappingAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 1, 28, false);
    stopRobot();
  }
  // Wait for input or timeout
  if (millis() - starting_time_state > getPlayDuration() + WAIT_FOR_ANSWER) {
    // Go to state 30 (LOWCLAPPING)
    setState(30);
  }
}

// Stay still and ask for clapping
void lowClappingAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(3, 4, 29, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration() && millis() - starting_time_state < WAIT_FOR_ANSWER + getPlayDuration()) {
    detectVolumeContinuous();
  } else if (millis() - starting_time_state > WAIT_FOR_ANSWER + getPlayDuration()) {
    switch (getVolumeContinuous()) {
      case 0:
        // Go to state 36 (REACTNOCLAPPING)
        setState(36);
        break;
      case 1:
        // Go to state 31 (HIGHCLAPPING)
        setState(31);
        break;
      case 2:
        // Go to state 31(HIGHCLAPPING)
        setState(31);
        break;
      default:
        // Go to state 36 (REACTNOCLAPPING)
        setState(36);
        break;
    }
  }
}

// Stay still and talk
void highClappingAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 4, 30, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration())
  {
    // Go to state 6 (BEERFACT)
    next_state = 9; // INVITE
    setState(6);
  }
}

// Stay still and talk
void reactClappingAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(3, 4, 35, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration())
  {
    // Go to state 3 (LOOK FOR PEOPLE) or  4 (ROAMING)
    setStateIdle();
  }
}

// Stay still and ask for screaming
void requestScreamingAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 1, 31, false);
    stopRobot();
  }
  // Wait for input or timeout
  if (millis() - starting_time_state > getPlayDuration() + WAIT_FOR_ANSWER) {
    // Go to state 33 (LOWSCREAMING)
    setState(33);
  }
}

// Stay still and speak
void lowScreamingAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(3, 4, 32, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration() && millis() - starting_time_state < WAIT_FOR_ANSWER + getPlayDuration()) {
    detectVolumeContinuous();
  } else if (millis() - starting_time_state > WAIT_FOR_ANSWER + getPlayDuration()) {
    switch (getVolumeContinuous()) {
      case 0:
        // Go to state 39 (TIMEOUT)
        setState(39);
        break;
      case 1:
        // Go to state 34 (MEDSCREAMING)
        setState(34);
        break;
      case 2:
        // Go to state 35(HIGHSCREAMING)
        setState(35);
        break;
    }
  }
}

// Stay still and speak
void medScreamingAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(3, 4, 33, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration() && millis() - starting_time_state < WAIT_FOR_ANSWER + getPlayDuration()) {
    detectVolumeContinuous();
  } else if (millis() - starting_time_state > WAIT_FOR_ANSWER + getPlayDuration()) {
    switch (getVolumeContinuous()) {
      case 0:
        // Go to state 39 (TIMEOUT)
        setState(39);
        break;
      case 1:
        // Go to state 39 (TIMEOUT)
        setState(39);
        break;
      case 2:
        // Go to state 35 (HIGHSCREAMING)
        setState(35);
        break;
    }
  }
}

// Stay still and speak
void highScreamingAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 4, 34, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration())
  {
    // Go to state 6 (BEERFACT)
    setState(6);
  }
}

// Stay still and speak
void timeoutAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(3, 5, 38, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration()) {
    // Go to state 3 (LOOK FOR PEOPLE) or  4 (ROAMING)
    setStateIdle();
  }
}

// Stay still and say goodbye
void goodbyeAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 1, 8, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration()) {
    // Go to state 19 (STATIC CHECK)
    setState(19);
  }
}

// Stay still and invite people to play
void beerGameInviteAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 1, 13, false);
    stopRobot();
  }
  // When I've finished talking change state
  if (millis() - starting_time_state > getPlayDuration()) {
    // Go to state 17 (BEER GAME SELECTION)
    setState(17);
  }
}

// Look for people and select them
void beerGameSelectionAnim() {
  switch (current_action) {
    case 1:
      // K_ANIMATOR - Set the animations for this action
      setAllAnimations(1, 3, 0, false);
      // Set the rotation
      rotate(ROTATE_MEDIUM_VELOCITY, random(2));
      // Switch to the next action
      current_action = 2;
      starting_time_action = millis();
      break;
    case 2:
      // When a person is detected stop and talk
      if (millis() - starting_time_action > 1500 && isPersonLazy()) {
        stopRobot();
        // K_ANIMATOR - Set the animations for this action
        setAllAnimations(2, 1, 14, false);
        // Switch to next action
        current_action = 3;
        starting_time_action = millis();
      } else if (millis() - starting_time_action > WAIT_FOR_ANSWER + getPlayDuration()) {
        // Go to state 39 (TIMEOUT)
        setState(39);
      }
      break;
    case 3:
      // If I've finished with this action perform the next one
      if (millis() - starting_time_action > getPlayDuration()) {
        // K_ANIMATOR - Set the animations for this action
        setAllAnimations(1, 3, 0, false);
        // Set the rotation
        rotate(ROTATE_MEDIUM_VELOCITY, random(2));
        // Switch to the next action
        current_action = 4;
        starting_time_action = millis();
      }
      break;
    case 4:
      // When a person is detected stop and talk
      if (millis() - starting_time_action > 1500 && isPersonLazy()) {
        stopRobot();
        // K_ANIMATOR - Set the animations for this action
        setAllAnimations(2, 1, 14, false);
        // Switch to next action
        current_action = 5;
        starting_time_action = millis();
      } else if (millis() - starting_time_action > WAIT_FOR_ANSWER + getPlayDuration()) {
        // Go to state 39 (TIMEOUT)
        setState(39);
      }
      break;
    case 5:
      // If I've finished with this action perform the next one
      if (millis() - starting_time_action > getPlayDuration()) {
        // Go to state 18 (BEER GAME END)
        setState(18);
      }
      break;
    default:
      current_action = 1;
      break;
  }
}

// Stay still and invite to kiss or offer a beer
void beerGameEndAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 4, 15, false);
    stopRobot();
  }
  // When I've finished talking change state
  if (millis() - starting_time_state > getPlayDuration()) {
    // Go to state 10 (GOODBYE)
    setState(10);
  }
}

// Invite the person inside the pub
void inviteAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 1, 7, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration())
  {
    // Go to state 10 (GOODBYE)
    setState(10);
  }
}

// Counts how many people there is and changes state accordingly
void countPeopleAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 3, 0, false);
    stopRobot();
  }
  if ((millis() - starting_time_state > 3500 && isPersonLazy()) || millis() - starting_time_state > 5000) {
    stopRobot();
    if (isCrowd()) {
      // Go to state 14 (INTERACTION MULTIPLE PERSON)
      setState(14);
    } else {
      // Go to state 13 (INTERACTION SINGLE PERSON)
      setState(13);
    }
  }
}

void interactionSinglePersonAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 5, 10, false);
    stopRobot();
  }
  // When I've finished talking change state
  if (millis() - starting_time_state > getPlayDuration()) {
    switch (random(3)) {
      case 0:
        next_state = 29;
        // Go to state 6 (BEER FACT)
        setState(6);
        break;
      case 1:
        // Go to state 40 (ASK PREFERENCE)
        setState(40);
        break;
      case 2:
        // Go to state 37 (MAGIC SHOW BEGIN)
        setState(37);
        break;
    }
  }
}

void interactionMultiplePersonAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 4, 11, false);
    stopRobot();
  }
  // When I've finished talking change state
  if (millis() - starting_time_state > getPlayDuration()) {
    if (random(2))
      // Go to state 16 (BEER GAME INVITE)
      setState(16);
    else
      // Go to state 32 (REQUESTSCREAMING)
      setState(32);
  }
}

void staticPeopleAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 4, 12, false);
    stopRobot();
  }
  // When I've finished talking change state
  if (millis() - starting_time_state > getPlayDuration()) {
    // Go to state 3 (LOOK FOR PEOPLE) or  4 (ROAMING)
    setStateIdle();
  }
}

// Stay still and speak
void magicShowBeginAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 1, 36, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration() && millis() - starting_time_state < WAIT_FOR_ANSWER + getPlayDuration()) {
    // Detect sound
    detectVolumeContinuous();
  } else if (millis() - starting_time_state > WAIT_FOR_ANSWER + getPlayDuration()) {
    switch (getVolumeContinuous()) {
      case 0:
        // Go to state 39 (TIMEOUT)
        setState(39);
        break;
      case 1:
        // Go to state 38 (MAGICSHOWEND)
        setState(38);
        break;
      case 2:
        // Go to state 38 (MAGICSHOWEND)
        setState(38);
        break;
    }
  }
}

// Stay still and speak
void magicShowEndAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 4, 37, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration())
  {
    // Go to state 19 (STATIC CHECK)
    setState(19);
  }
}

// Stay still and speak, wait for touch
void beerLiftRequestAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 1, 25, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration() && getHandleTouched()) {
    // Go to state 27 (HANDLE TOUCHED INTERACTION)
    setState(27);
  } else if (millis() - starting_time_state > WAIT_FOR_ANSWER + getPlayDuration()) {
    // Go to state 39 (TIMEOUT)
    setState(39);
  }
}

void handleTouchedInteractionAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(3, 4, 26, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration()) {
    if (random(4) == 0)
      // Go to state 19 (STATIC CHECK)
      setState(19);
    else
      // Go to state 12 (COUNT PEOPLE)
      setState(12);
  }
}

void handleTouchedSurprisedAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(3, 2, 27, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration()) {
    // Go to state 3 (LOOK FOR PEOPLE)
    setState(3);
  }
}

// Stay still and ask for preference
void askPreferenceAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 1, 22, false);
    stopRobot();
  }
  // Wait for input or timeout
  if (millis() - starting_time_state > WAIT_FOR_ANSWER + getPlayDuration()) {
    // Go to state 39 (TIMEOUT)
    setState(39);
  } else if (millis() - starting_time_state > getPlayDuration() && getHandleTouched()) {
    // Go to state 41 (WINEPREFERENCE)
    setState(41);
  } else if (millis() - starting_time_state > getPlayDuration() &&  getFoamTouched()) {
    // Go to state 42 (BEERPREFERENCE)
    setState(42);
  }
}

// Stay still and speak
void winePreferenceAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(3, 5, 23, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration())
  {
    // Go to state 19 (STATIC CHECK)
    setState(19);
  }
}

// Stay still and speak
void beerPreferenceAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 4, 24, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration()) {
    // Go to state 19 (STATIC CHECK)
    setState(19);
  }
}

// Talk while rotating in place
void lookForPeopleAnim()  {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 3, 3, true);
    // Perform the rotation
    rotate(ROTATE_MEDIUM_VELOCITY, rotate_left);
  }
  if (millis() - starting_time_state > 10000) {
    // Go to state 4 (ROAMING)
    setState(4);
  }
}

// Talk while staying still
void scaredNoGroundAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 2, 2, false);
    moveBackward(BACKWARD_VELOCITY);
  }
  if (millis() - starting_time_state > getPlayDuration()) {
    // Go to state 2 (LOOK FOR GROUND)
    setState(2);
  } else if (millis() - starting_time_state > EXTRA_BACKWARD_TIME) {
    stopRobot();
  }
}

// Do nothing
void lookForGroundAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 2, 0, false);
    // Rotate in place
    rotate(ROTATE_MEDIUM_VELOCITY, rotate_left);
  }
  if (getCanStop() && millis() - getStartingTimeStop() > EXTRA_ROTATION_TIME) {
    setState(4);
  }
}

// Just move mustaches while moving
void roamingAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 3, 0, false);
    // I'm moving forward
    moveForward(FORWARD_VELOCITY);
  }
  // I randomly change animation
  /*if (random(CHANGE_RANDOMLY) == GUESS) {
    // Go to state 3 (LOOK FOR PEOPLE)
    setState(3);
    }*/
}

// Do nothing
void staticCheckAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 3, 0, false);
  }
  if (!isPersonLazy())
    // Go to state 3 (LOOK FOR PEOPLE)
    setState(3);
  else if (millis() - starting_time_state > WAIT_FOR_ANSWER + getPlayDuration())
    // Go to state 15 (STATIC PEOPLE)
    setState(15);
}

// MOVEMENTS //

// Moves left and right
void moveLeftRigth(int oscillation_time, int v) {
  switch (current_action) {
    case 1:
      rotate(v, true);
      current_action = 2;
      break;
    case 2:
      // If I've finished with this action perform the next one
      if (millis() - starting_time_action > oscillation_time / 4) {
        rotate(v, false);
        current_action = 3;
      }
      break;
    case 3:
      // If I've finished with this action perform the next one
      if (millis() - starting_time_action > oscillation_time * 3 / 4) {
        rotate(v, true);
        current_action = 4;
      }
      break;
    case 4:
      // If I've finished with this action perform the next one
      if (millis() - starting_time_action > oscillation_time) {
        starting_time_action = millis();
        rotate(v, true);
        current_action = 2;
      }
      break;
    default:
      current_action = 1;
      break;
  }

}
