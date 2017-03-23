// BEERBOT 4.0 //
// TODO: detection of the distance in order to avoid the robot crashes something
// TODO: movement when it doesn't detect anything (now it just rotate on the spot)
// TODO: add another sonar to detect the ground and integrate with the exisisting code
// TODO: implement animantions (mustache and eyebrows) in the function sayHi()
// TODO: integrate miniDF player into sayHi() taking into account animations

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
#define IR_PIN A0                       // Arduino analog pin 0 tied to the IR sensor
#define model 1080                      // Model of the IR sensor

// VARIABLES //

// Anomaly (Marco)
bool ground_detected;                   // Tells if there is ground in front of the robot
bool look_for_ground;                   // Tells if the robot has to look for ground
bool rotate_left;                       // Tells if the robot has to rotate left or right
bool end_interaction = 0;               // Manage the end of the interaction
int anomaly_distance;                   // Distance acquired by the IR sensor
int ground_limit = 11;                  // Minimum distance for which the IR output is considered ground
int is_ground_counter = 0;              // Counter needed to have an effective detection of anomalies
int obstacle_limit = 5;                // Maximum distance for which the IR output is considered obstacle

// People detection (Nicola)
int detection_ping_counter = 0;
int awake_increment = 3;
int detection_distance;
boolean moved = 0;                      // TODO, maybe in the movement file

// Velocities
int sp = 180;                           // Default speed

// Sensors
NewPing detection_sonar(TRIGGER_PIN_DETECTION, ECHO_PIN_DETECTION, MAX_DISTANCE_DETECTION);
SharpIR SharpIR(IR_PIN, model);

// Millis
long starting_time_anomaly_IR;          // Tells when the IR starts to ping
long start_stop_look_for_ground;        // The robot has detected the ground? (TODO)
long starting_time_rotation;            // Tells when the robot starts to rotate to detect people
long starting_time_turning;             // Tells when the robot starts to rotate to detect ground
long starting_time_movement;            // Tells when the robot starts to move
long starting_time_detection_sonar;     // Tells when the sonar starts to ping
long starting_time_interaction;         // Tells when interactions with users starts

void setup() {
  // Initialize the serial port
  Serial.begin(9600);
  // Set a seed for the random number generator
  randomSeed(1234);
  // Initialize pins and whatever regarding the movement
  setupMovement();

  // Initialize millis
  starting_time_anomaly_IR = millis();
  starting_time_detection_sonar = millis();

  // Initialize other variables
  ground_detected = false;
  look_for_ground = true;
}

void loop() {
  // Detect if there is a border or an obstacle
  anomalyDetect(10);
  // Process the previous information
  processAnomalyDetection();

  // Move the robot
  moveRobot();

  // Print infos
  // Serial.print("Ground counter: ");
  // Serial.print(is_ground_counter);
  // Serial.print(", detection counter: ");
  // Serial.print(detection_ping_counter);
  // Serial.print(" and distance: ");
  // Serial.println(detection_distance);
}

// Border and obstacle IR detection
void anomalyDetect(int ping_time) {
  // if (millis() - starting_time_anomaly_IR > ping_time) {
    // Acquire border/obstacle distance
    anomaly_distance = SharpIR.distance();
    Serial.print("Looking for anomalies, got value: ");
    Serial.println(anomaly_distance);
    // Change the counter consequently
    if (anomaly_distance < ground_limit && anomaly_distance > obstacle_limit && anomaly_distance != 0) {
      is_ground_counter++;
    } else {
      is_ground_counter--;
    }
  // }
}

// Process the detection of the anomaly
void processAnomalyDetection() {
  if (is_ground_counter >= 2) {
    // Normalize the value
    is_ground_counter = 2;
    // If it's the "first" ground detected after an anomaly process it
    if (!ground_detected) {
      Serial.print("Ground detected, with value: ");
      Serial.println(anomaly_distance);
      ground_detected = true;
      setLookForGround(false);
    }
  } else if (is_ground_counter <= 0) {
    is_ground_counter = 0;
    stopRobot();
    if (ground_detected) {
      Serial.print("No more ground, with value: ");
      Serial.println(anomaly_distance);
      ground_detected = false;
      if (!look_for_ground) {
        setLookForGround(true);
      }
    }
  }
  starting_time_anomaly_IR = millis();
  // delay(10);
}

// Manage the robot looking for ground
void setLookForGround(bool b) {
  if (b) {
    // Serial.println("I'm looking for ground");
    look_for_ground = b;
    moveBackward(sp);
    // Get the sense of rotation;
    rotate_left = random(2);
  } else {
    // Serial.println("I'm going to stop looking for ground");
    look_for_ground = false;
    moved = 0;
  }
}

// Manage the movement of the robot
void moveRobot() {
  // Look for ground if the robot has to
  if (look_for_ground) {
    // Turn left or right depending on "rotate_left"
    rotate(sp, rotate_left);
    starting_time_turning = millis();
    while (millis() - starting_time_turning < 1000 && look_for_ground) {
      // Serial.println("I'm rotating looking for ground");
      anomalyDetect(50);
      if (anomaly_distance < ground_limit && anomaly_distance > obstacle_limit && anomaly_distance != 0) {
        setLookForGround(false);
      }
      starting_time_anomaly_IR = millis();
    }
  }
  else if (!moved) {
    // Serial.println("I'm moving free");
    processPeopleDetection();
  }
}

// Manage people detections via sonar <----------
void peopleDetect() {
  detection_distance = detection_sonar.ping_cm();
  // Serial.print("Value of 'detection_distance' is: ");
  // Serial.println(detection_distance);
  if (detection_distance > 5) {
    // Decrese the idle counter
    detection_ping_counter = detection_ping_counter - awake_increment;
    if (detection_ping_counter < 0) {
      detection_ping_counter = 0;
    }
  } else {
    detection_ping_counter++;
    if (detection_ping_counter >= 5) {
      moved = 0;
    }
  }
}

// Process the detection of people
void processPeopleDetection() {
  // Detection of people while rotating
  boolean flag = 0;

  // At the end of the interaction we don't need to update the counter because sayHi() has already done it
  if (!end_interaction) {
    peopleDetect();
  }

  // If needed the robot starts to look around in order to detect someone
  if (detection_ping_counter >= 10) {
    // Randomly rotate left or rigth
    // Serial.println("I have to rotate!");
    rotate(sp, random(2));
    starting_time_rotation = millis();
    while (millis() - starting_time_rotation < ROTATION_TIME && !flag) {
      peopleDetect();
      // Serial.print("Detected distance while rotating: ");
      // Serial.println(detection_distance);
      if (detection_distance > 5) {
        if (!end_interaction) {
          // If it detects someone it stops rotating and tries to approach the person
          detection_ping_counter = 0;
          stopRobot();
          moveForward(sp);
          moved = 1;
          // It moves forward for a fixed amount of time
          starting_time_movement = millis();
          while (millis() - starting_time_movement < 1000) {
            // Serial.println("I'm moving forward");
            delay(50);
            anomalyDetect(50);
            processAnomalyDetection();
            if (look_for_ground) {
              break;
            } else {
              moveRobot();
            }
          }
          // Then I stop the robot and salute
          stopRobot();
          sayHi();
          // Finally break the loop
          flag = 1;
        }
      } else {
        end_interaction = 0;
      }
    }
    // After a certain amount of time it stops anyway
    if (!flag) {
      stopRobot();
      end_interaction = 0;
    }
    // Reset the detection counter
    detection_ping_counter = 0;
  } else if (detection_ping_counter <= 0 ) {
    // Normalize the counter
    detection_ping_counter = 0;
    if (!moved) {
      // If it detects someone it stops rotating and tries to approach the person
      // Serial.println("Approaching the person");
      moveForward(sp);
      moved = 1;
      starting_time_movement = millis();
      while (millis() - starting_time_movement < 1000) {
        // Serial.println("I'm moving forward");
        delay(50);
        anomalyDetect(50);
        processAnomalyDetection();
        if (look_for_ground) {
          break;
        }
        else {
          moveRobot();
        }
      }
      stopRobot();
      sayHi();
    }
  }
  // TODO: moveAround();
}

// TODO: we have to model and implement the interaction with the user
// TODO: we have to stop rotating somewhere
// Interact with the suer
void sayHi() {
  starting_time_interaction = millis();
  // Serial.println("Hey man!");
  // Interact with the person
  while (millis() - starting_time_interaction < INTERACTION_TIME && detection_ping_counter <= 4) {
    // Serial.println("I'm making friends");
    delay(200);
    peopleDetect();
  }
  // Once done, disengage
  if (detection_ping_counter <= 4) {
    // rotate(sp, random(2));
    end_interaction = 1;
    detection_ping_counter = 10;
    processPeopleDetection();
  }
  moved = 0;
}
