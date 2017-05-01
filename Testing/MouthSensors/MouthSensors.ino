// ANOMALY DETECTION SUPPORT FUNCTIONS //

// INCLUSIONS //

#include <SharpIR.h>

// DEFINITIONS

#define IR_PIN_LEFT A0                  // Arduino analog pin 0 tied to the left IR sensor
#define IR_PIN_RIGTH A1                 // Arduino analog pin 1 tied to the rigth IR sensor
#define model 1080                      // Model of the IR sensor

// VARIABLES //

// Miscellaneous
bool ground_detected;                   // Tells if there is ground on front of the robot
bool ground_detected_lazy;              // Tells if there is ground on front of the robot
bool look_for_ground;                   // Tells if the robot has to look for ground
int anomaly_distance_left;              // Distance acquired by the left IR sensor
int anomaly_distance_rigth;             // Distance acquired by the rigth IR sensor
int ground_limit = 12;                  // Minimum distance for which the IR output is considered ground
int is_ground_counter_left = 0;         // Counter needed to have an effective detection of anomalies
int is_ground_counter_rigth = 0;        // Counter needed to have an effective detection of anomalies
int obstacle_limit = 4  ;               // Maximum distance for which the IR output is considered obstacle

// Sensors
SharpIR left_IR(IR_PIN_LEFT, model);
SharpIR rigth_IR(IR_PIN_RIGTH, model);

// CODE //

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  setupAnomalyDetection();
}

void loop() {
  anomalyDetect();
  processAnomalyDetection();
  delay(500);
}

// SUPPORT FUNCTIONS //

// Setup the anomaly detection
void setupAnomalyDetection() {
  ground_detected = false;
  ground_detected_lazy = false;
  look_for_ground = true;
}

// Border and obstacle IR detection
void anomalyDetect() {
  // Acquire border/obstacle distance
  anomaly_distance_left = left_IR.distance();
  anomaly_distance_rigth = rigth_IR.distance();
  // Change the counter consequently, first the left one
  if (anomaly_distance_left < ground_limit && anomaly_distance_left > obstacle_limit && anomaly_distance_left != 0) {
    Serial.print("LEFT: ground detected, got value: ");
    Serial.print(anomaly_distance_left);
    is_ground_counter_left++;
  } else {
    Serial.print("LEFT: anomaly detected, got value: ");
    Serial.print(anomaly_distance_left);
    is_ground_counter_left--;
  }
  Serial.print(", with counter: ");
  Serial.println(is_ground_counter_left);
  // And then the rigth one
  if (anomaly_distance_rigth < ground_limit && anomaly_distance_rigth > obstacle_limit && anomaly_distance_rigth != 0) {
    Serial.print("RIGTH: ground detected, got value: ");
    Serial.print(anomaly_distance_rigth);
    is_ground_counter_rigth++;
  } else {
    Serial.print("RIGTH: anomaly detected, got value: ");
    Serial.print(anomaly_distance_rigth);
    is_ground_counter_rigth--;
  }
  Serial.print(", with counter: ");
  Serial.println(is_ground_counter_rigth);
}

// Process the detection of the anomaly.
void processAnomalyDetection() {
  // First of all normalize the counters
  normalizeCounters();
  // Then process the information
  if (is_ground_counter_rigth >= 2 && is_ground_counter_left >= 2) {
    // If it's the "first" ground detected after an anomaly process it
    if (!ground_detected) {
      Serial.println("SWITCHED TO GROUND");
      ground_detected = true;
      setLookForGround(false);
    }
  } else if (is_ground_counter_rigth <= 0 || is_ground_counter_left <= 0) {
    if (ground_detected) {
      Serial.println("SWITCHED TO ANOMALY");
      ground_detected = false;
      if (!look_for_ground) {
        setLookForGround(true);
      }
    }
  }
}

//Process the detection of the anomaly. This lazy version doesn't call setLookForGround
void processAnomalyDetectionLazy() {
  // First of all normalize the counters
  normalizeCounters();
  // Then process the information
  if (is_ground_counter_rigth >= 2 && is_ground_counter_left >= 2) {
    // If it's the "first" ground detected after an anomaly process it
    if (!ground_detected_lazy) {
      Serial.println("SWITCHED TO GROUND");
      ground_detected_lazy = true;
    }
  } else if (is_ground_counter_rigth <= 0 || is_ground_counter_left <= 0) {
    if (ground_detected_lazy) {
      Serial.println("SWITCHED TO ANOMALY");
      ground_detected_lazy = false;
    }
  }
}

void normalizeCounters() {
  // Normalize left
  if (is_ground_counter_left >= 2) {
    is_ground_counter_left = 2;
  } else if (is_ground_counter_left <= 0) {
    is_ground_counter_left = 0;
  }
  // Normalize rigth
  if (is_ground_counter_rigth >= 2) {
    is_ground_counter_rigth = 2;
  } else if (is_ground_counter_rigth <= 0) {
    is_ground_counter_rigth = 0;
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
    // setRotateLeft(getRotation());
    // K_STATE - Start looking for ground, state 2
    // setState(2);
  } else {
    // Stop looking for ground
    look_for_ground = false;
    // K_ANIMATOR - I'm neutral
    // setEyebrowPosition(1);
    // K_STATE - Reset previous state
    // resetState();
  }
}

bool getRotation() {
  if (is_ground_counter_left == 0 && is_ground_counter_rigth != 0)
    return false;
  else if (is_ground_counter_left != 0 && is_ground_counter_rigth == 0)
    return true;
  else
    return random(2);
}
