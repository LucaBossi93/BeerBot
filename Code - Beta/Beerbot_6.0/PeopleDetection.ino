// PEOPLE DETECTION SUPPORT FUNCTIONS //

// VARIABLES //

// Miscellaneous
int no_detection_ping_counter = 0;
int awake_increment = 3;
int detection_distance;                 // Distance detected by the sonar
int detection_min = 10;                 // Minimum distance to be considered a person
int detection_max = 150;                // Maximum distance to be considered a person
int person_detected;                    // Tells if a person has been detected

// SUPPORT FUNCTIONS //

// Setup people detection
void setupPeopleDetection() {
  person_detected = false;
}

// Manage people detections via sonar
void peopleDetect() {
  // Acquire the distance
  detection_distance = detection_sonar.ping_cm();
  if (detection_distance > detection_min && detection_distance < detection_max) {
    no_detection_ping_counter = no_detection_ping_counter - awake_increment;
    Serial.print("Person detected, with distance: ");
  } else {
    // Otherwise decrese the idle counter
    no_detection_ping_counter++;
    Serial.print("No person detected, with distance: ");
  }
  Serial.print(detection_distance);
  Serial.print(" and counter: ");
  Serial.println(no_detection_ping_counter);

}

// Process the detection of people
void processPeopleDetection() {
  if (no_detection_ping_counter >= 12) {
    // Normalize the value
    no_detection_ping_counter = 12;
    if (person_detected) {
      Serial.println("SWITCHED TO NO PERSON DETECTED");
      setPersonDetected(false);
    }
  } else if (no_detection_ping_counter <= 0) {
    // Normalize the value
    no_detection_ping_counter = 0;
    if (!person_detected) {
      Serial.println("SWITCHED TO PERSON DETECTED");
      setPersonDetected(true);
    }
  }
}

// Manage the robot looking for people. This makes the state of the robot change.
void setPersonDetected(bool b) {
  if (b) {
    // Start looking for person
    person_detected = true;
    // K_STATE - Move towards the person
    setState(3);
  } else {
    // Stop looking for person
    person_detected = false;
    // Set the sense of rotation
    setRotateLeft(random(2));
    // K_STATE - Look for person
    setState(1);
  }
}
