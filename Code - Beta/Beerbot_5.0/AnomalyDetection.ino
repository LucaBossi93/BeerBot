// ANOMALY DETECTION SUPPORT FUNCTIONS //

// VARIABLES //

// Miscellaneous
bool ground_detected;                   // Tells if there is ground in front of the robot
bool ground_detected_lazy;              // Tells if there is ground in front of the robot
bool look_for_ground;                   // Tells if the robot has to look for ground
bool end_interaction = 0;               // Manage the end of the interaction
int anomaly_distance;                   // Distance acquired by the IR sensor
int ground_limit = 12;                  // Minimum distance for which the IR output is considered ground
int is_ground_counter = 0;              // Counter needed to have an effective detection of anomalies
int obstacle_limit = 4  ;               // Maximum distance for which the IR output is considered obstacle

// Millis

// SUPPORT FUNCTIONS //

// Setup the anomaly detection
void setupAnomalyDetection() {
  ground_detected = false;
  look_for_ground = true;
}

// Border and obstacle IR detection
void anomalyDetect() {
  // Acquire border/obstacle distance
  anomaly_distance = SharpIR.distance();
  // Change the counter consequently
  if (anomaly_distance < ground_limit && anomaly_distance > obstacle_limit && anomaly_distance != 0) {
    // Serial.print("Ground detected, got value: ");
    // Serial.print(anomaly_distance);
    is_ground_counter++;
  } else {
    // Serial.print("Anomaly detected, got value: ");
    // Serial.print(anomaly_distance);
    is_ground_counter--;
  }
  // Serial.print(", with counter: ");
  // Serial.println(is_ground_counter);
}

// Process the detection of the anomaly.
void processAnomalyDetection() {
  if (is_ground_counter >= 2) {
    // Normalize the value
    is_ground_counter = 2;
    // If it's the "first" ground detected after an anomaly process it
    if (!ground_detected) {
      Serial.println("SWITCHED TO GROUND");
      ground_detected = true;
      setLookForGround(false);
    }
  } else if (is_ground_counter <= 0) {
    is_ground_counter = 0;
    if (ground_detected) {
      Serial.println("SWITCHED TO ANOMALY");
      ground_detected = false;
      if (!look_for_ground) {
        setLookForGround(true);
      }
    }
  }
}
//P rocess the detection of the anomaly. This lazy version doesn't call setLookForGround
void processAnomalyDetectionLazy() {
  if (is_ground_counter >= 2) {
    // Normalize the value
    is_ground_counter = 2;
    // If it's the "first" ground detected after an anomaly process it
    if (!ground_detected_lazy) {
      Serial.println("SWITCHED TO GROUND");
      ground_detected_lazy = true;
    }
  } else if (is_ground_counter <= 0) {
    is_ground_counter = 0;
    if (ground_detected_lazy) {
      Serial.println("SWITCHED TO ANOMALY");
      ground_detected_lazy = false;
    }
  }
}

bool isGroundLazy() {
  return ground_detected_lazy;
}

// Manage the robot looking for ground. This makes the state of the robot change.
void setLookForGround(bool b) {
  if (b) {
    // Start looking for ground
    look_for_ground = b;
    // Get the sense of rotation;
    setRotateLeft(random(2));
    // K_STATE - Start looking for ground, state 2
    setState(2);
  } else {
    // Stop looking for ground
    look_for_ground = false;
    // K_ANIMATOR - I'm neutral
    setEyebrowPosition(1);
    // K_STATE - Reset previous state
    resetState();
  }
}
