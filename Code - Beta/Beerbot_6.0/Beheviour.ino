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
      // FORWARD - I've detected a person and I'm moving towards him. I only detect the anomalies but I don't
      // process them since I just want to stop instead of starting looking for ground
      // Detect if there is an anomaly
      anomalyDetect();
      // Process the information
      processAnomalyDetectionLazy();
      // I'm moving towards someone
      moveForwardWithTimeoutAnim(1500);
      break;
    case 2:
      // LOOK FOR GROUND - I'm looking for ground. I don't need to look for people while I do it
      // Detect if there is an anomaly
      anomalyDetect();
      // Process the information
      processAnomalyDetection();
      // Rotate in place
      rotate(sp, rotate_left);
      // Setup the animation and talk
      lookForGroundAnim();
      break;
    case 3:
      // LOOK FOR PEOPLE - I'm rotating looking for people. I rotate for some time, I cooldown and then I repeat
      // In this phase I don't need to look for ground since I'm rotating in place
      // Detect if there is a person
      peopleDetect();
      // Process the information
      processPeopleDetection();
      // Perform the rotation
      rotateWithCooldownAnim(1500, 300);
      // Setup the animation and talk
      lookForPeopleAnim();
      break;
    case 4:
      // ROAMING - Simply move forward and rotate when anomaly is found.
      // Detect if there is an anomaly
      anomalyDetect();
      // Process the information
      processAnomalyDetection();
      // I'm moving forward
      moveForward(sp);
      break;
    case 5:
      // SINGING - (TODO)
      break;
    case 6:
      // BEER FACT - Move left and right a bit and talk
      // Setup the animation and talk
      beerFactAnim();
      // Move left and right
      moveLeftRigth(500, 50);
      break;
    case 7:
      // MUSTACHE SHOW - (TODO)
      break;
    case 8:
      // GREET - I'm saying hi. I look for people, if the time is over or nobody is watching start looking
      // for people again
      // Detect if there is a person
      peopleDetect();
      // Process the information
      processPeopleDetection();
      // Say hi
      sayHiAnim(5000);
      break;
    case 9:
      // INVITE - (TODO)
      // Setup the animation
      inviteAnim();
      break;
    case 10:
      // GOODBYE - Stay still and say goodbye
      // Set the animation
      goodbyeAnim();
      break;
    case 11:
      // HELP - Stay still and call for help
      // Setup the animation
      helpAnim();
      break;
    case 12:
      // COUNTPEOPLE - Count how much people there is
      // Setup the animation
      countPeopleAnim();
      // Move left and right
      moveLeftRigth(500, 50);
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
      // STATICPEOPLE - Be angy with people who are staying still looking at you
      staticPeopleAnim();
      break;
    case 16:
      // BEER GAME INVITE - Stay still and invite people to play
      // Setup the animation
      beerGameInviteAnim();
      break;
    case 17:
      // BEER GAME SELECTION - Select two persons
      // Detect the people
      peopleDetect();
      // Detect the people
      processPeopleDetectionLazy();
      // Setup the animation, look for people and select them
      beerGameSelectionAnim();
      break;
    case 18:
      // BEER GAME END - Stay still and invite to kiss or have a beer
      // Setup the animation
      beerGameEndAnim();
    case 19:
      // NA
      break;
    case 20:
      // FOAM TOUCH - Stay still and ask for touching the foam
      // Setup the animation
      foamTouchAnim();
      break;
    case 21:
      // FOAM TOUCHED - Stay still and speak
      // Setup the animation
      foamTouchedAnim();
      break;
    case 23:
      // ASK FOR GENDER - Stay still and ask for gender
      // Setup the animation
      askForGenderAnim();
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
      // Acquire the volume
      detectVolumeContinuous();
      // Setup the animation
      lowScreamingAnim();
      break;
    case 34:
      // MED SCREAMING: - Stay still and listen
      // Acquire the volume
      detectVolumeContinuous();
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

// ANIMATIONS //

// Rotate in palce with a cooldown
void rotateWithCooldownAnim(int rotationTime, int cooldown) {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 1, 2, false);
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
void moveForwardWithTimeoutAnim(int timeout) {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 2, 1, false);
  }
  // If I don't have detected an anomaly or I've finished, go forward
  if (millis() - starting_time_state > timeout || !isGroundLazy()) {
    stopRobot();
    // Go to state 8 (GREET)
    setState(7);
    Serial.println("STOP!!!");
  } else {
    moveForward(sp);
  }
}

// Interact with the detected person
void sayHiAnim(int timeout) {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 1, 7, false);
  }
  // If I've finished search for someone else. PeopleDetection will take care of the case in which
  // everyone left
  if (millis() - starting_time_state > timeout)
    // Go to state 4 (ROAMING)
    setState(4);//ERROR
}

// Tell the person a fact about beer
void beerFactAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 1, 5, false);
  }
}

// Stay still and call for help
void helpAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(3, 4, 10, false);
    stopRobot();
  }
}

// Stay still and ask for touch the foam
void foamTouchAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 1, 17, false);
    stopRobot();
  }
  // Wait for input or timeout
  if (millis() - starting_time_state > WAIT_FOR_ANSWER) {
    // Go to state 39 (TIMEOUT)
    setState(39);
  }  else if (getFoamTouched()) {
    // Go to state 21 (FOAMTOUCHED)
    setState(21);
  }
}

// Stay still and speak
void foamTouchedAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 1, 18, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration())
  {
    // Go to state 6 (BEERFACT)
    setState(6);
  }
}

// Stay still and ask for gender
void askForGenderAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 1, 20, false);
    stopRobot();
  }
  // Wait for input or timeout
  if (millis() - starting_time_state > WAIT_FOR_ANSWER ) {
    // Go to state 39 (TIMEOUT)
    setState(39);
  } else if (getHandleTouched ()) {
    // Go to state 25 (WOMANINTERACTION)
    setState(25);
  } else if (getFoamTouched ()) {
    // Go to state 24 (MANINTERACTION)
    setState(24);
  }
}

// Stay still and speak
void manInteractionAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(3, 1, 21, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration())
  {
    // TODO
  }
}

// Stay still and speak
void womanInteractionAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 1, 22, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration())
  {
    // TODO
  }
}
// Stay still and ask for clapping
void requestClappingAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 1, 29, false);
    stopRobot();
  }
  // Wait for input or timeout
  if (millis() - starting_time_state > getPlayDuration()) {
    // Go to state 30 (LOWCLAPPING)
    setState(30);
  }
}
// Stay still and ask for clapping
void lowClappingAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(3, 1, 30, false);
    stopRobot();
  }
  if (millis() - starting_time_state > WAIT_FOR_ANSWER) {
    // Detect sound
    getVolumeContinuous();
  } else {
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
    resetAndSet(2, 1, 31, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration())
  {
    // Go to state 6 (BEERFACT)
    setState(6);
  }
}

// Stay still and talk
void reactClappingAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(3, 1, 36, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration())
  {
    // Go to state 4 (ROAMING)
    setState(4);
  }
}

// Stay still and ask for screaming
void requestScreamingAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 1, 32, false);
    stopRobot();
  }
  // Wait for input or timeout
  if (millis() - starting_time_state > getPlayDuration()) {
    // Go to state 33 (LOWSCREAMING)
    setState(33);
  }
}

// Stay still and speak
void lowScreamingAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(3, 1, 33, false);
    stopRobot();
  }
  if (millis() - starting_time_state > WAIT_FOR_ANSWER) {
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
      default:
        // Go to state 39 (TIMEOUT)
        setState(39);
        break;
    }
  }
}

// Stay still and speak
void medScreamingAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(3, 1, 34, false);
    stopRobot();
  }
  if (millis() - starting_time_state > WAIT_FOR_ANSWER) {
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
      default:
        // Go to state 39 (TIMEOUT)
        setState(39);
        break;
    }
  }
}

// Stay still and speak
void highScreamingAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 1, 35, false);
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
    resetAndSet(3, 1, 39, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration())
  {
    // TODO
  }
}


// Stay still and say goodbye
void goodbyeAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 1, 9, false);
    stopRobot();
  }
  // TODO
}

// Stay still and invite people to play
void beerGameInviteAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 1, 14, false);
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
      setAllAnimations(1, 0, 0, false);
      // Set the rotation
      rotate(50, random(2));
      // Switch to the next action
      current_action = 2;
      starting_time_action = millis();
      break;
    case 2:
      // When a person is detected stop and talk
      if (millis() - starting_time_action > random(100, 10000) && isPersonLazy()) {
        stopRobot();
        // K_ANIMATOR - Set the animations for this action
        setAllAnimations(2, 1, 15, false);
        // Switch to next action
        current_action = 3;
        starting_time_action = millis();
      } else if (millis() - starting_time_action > WAIT_FOR_ANSWER) {
        // Go to state 39 (TIMEOUT)
      }
      break;
    case 3:
      // If I've finished with this action perform the next one
      if (millis() - starting_time_action > getPlayDuration()) {
        // K_ANIMATOR - Set the animations for this action
        setAllAnimations(1, 0, 0, false);
        // Set the rotation
        rotate(50, random(2));
        // Switch to the next action
        current_action = 4;
        starting_time_action = millis();
      }
      break;
    case 4:
      // When a person is detected stop and talk
      if (millis() - starting_time_action > random(100, 10000) && isPersonLazy()) {
        stopRobot();
        // K_ANIMATOR - Set the animations for this action
        setAllAnimations(2, 1, 15, false);
        // Switch to next action
        current_action = 5;
        starting_time_action = millis();
      } else if (millis() - starting_time_action > WAIT_FOR_ANSWER) {
        // Go to state 39 (TIMEOUT)
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
    resetAndSet(2, 1, 16, false);
    stopRobot();
  }
  // When I've finished talking change state
  if (millis() - starting_time_state > getPlayDuration()) {
    // Go to state 4 (ROAMING)
    setState(4);
  }
}

// Invite the person inside the pub
void inviteAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 1, 8, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration())
  {
    // TODO
  }
}

// Counts how many people there is and changes state accordingly
void countPeopleAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 0, 0, false);
    stopRobot();
  }
  if (millis() - starting_time_state > 3000)
  {
    if (isCrowd()) {
      // TODO
    } else {
      // TODO
    }
  }
}

void interactionSinglePersonAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 1, 11, false);
    stopRobot();
  }
  // When I've finished talking change state
  if (millis() - starting_time_state > getPlayDuration()) {
    // Go to state 4 (ROAMING)
    setState(4);
  }
}

void interactionMultiplePersonAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 2, 12, false);
    stopRobot();
  }
  // When I've finished talking change state
  if (millis() - starting_time_state > getPlayDuration()) {
    // Go to state 4 (ROAMING)
    setState(4);
  }
}

void staticPeopleAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 2, 13, false);
    stopRobot();
  }
  // When I've finished talking change state
  if (millis() - starting_time_state > getPlayDuration()) {
    // Go to state 4 (ROAMING)
    setState(4);
  }
}

// Stay still and speak
void magicShowBeginAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 1, 37, false);
    stopRobot();
  }
  if (millis() - starting_time_state > WAIT_FOR_ANSWER) {
    // Detect sound
    getVolumeContinuous();
  } else {
    switch (getVolumeContinuous()) {
      case 0:
        // Go to state 39 (TIMEOUT)
        setState(39);
        break;
      case 1:
        // Go to state 38 (MAGICSHOWEND)
        setState(39);
        break;
      case 2:
        // Go to state 38 (MAGICSHOWEND)
        setState(35);
        break;
      default:
        // Go to state 39 (TIMEOUT)
        setState(39);
        break;
    }
  }
}

// Stay still and speak
void magicShowEndAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 1, 38, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration())
  {
    // Go to state 4 (ROAMING)
    setState(4);
  }
}

// Stay still and speak, wait for touch
void beerLiftRequestAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 1, 26, false);
    stopRobot();
  }
  if (getHandleTouched())
  {
    // Go to state 27 (HANDLE TOUCHED INTERACTION)
    setState(27);
  } else if (millis() - starting_time_state > WAIT_FOR_ANSWER) {
    // Go to state 39 (TIMEOUT)
    setState(39);
  }
}

void handleTouchedInteractionAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(3, 4, 27, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration()) {
    // TODO
  }
}

void handleTouchedSurprisedAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(3, 4, 28, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration()) {
    // TODO
  }
}

// Stay still and ask for preference
void askPreferenceAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 1, 23, false);
    stopRobot();
  }
  // Wait for input or timeout
  if (millis() - starting_time_state > WAIT_FOR_ANSWER ) {
    // Go to state 39 (TIMEOUT)
    setState(39);
  } else if (getHandleTouched ()) {
    // Go to state 41 (WINEPREFERENCE)
    setState(41);
  } else if (getFoamTouched ()) {
    // Go to state 42 (BEERPREFERENCE)
    setState(42);
  }
}

// Stay still and speak
void winePreferenceAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(3, 1, 24, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration())
  {
    // TODO
  }
}

// Stay still and speak
void beerPreferenceAnim() {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(2, 1, 25, false);
    stopRobot();
  }
  if (millis() - starting_time_state > getPlayDuration())
  {
    // TODO
  }
}


// Talk while rotating in place
void lookForPeopleAnim()  {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 1, 3, true);
    stopRobot();
  }
}

// Talk while rotating in place
void lookForGroundAnim()  {
  // Reset the variables if needed
  if (resetNeeded) {
    // K_ANIMATOR - Set the animations for this state
    resetAndSet(1, 1, 2, false);
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
