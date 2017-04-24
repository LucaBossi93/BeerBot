// PEOPLE DETECTION SUPPORT FUNCTIONS //

// INCLUSIONS //

#include <Adafruit_MLX90614.h>
#include <NewPing.h>
#include <Wire.h>

// DEFINITIONS //

#define TRIGGER_PIN_DETECTION 12        // Arduino pin 12 tied to trigger pin of the detection sonar
#define ECHO_PIN_DETECTION 11           // Arduino pin 11 tied to echo pin of the detection sonar
#define MAX_DISTANCE_DETECTION 150      // Maximum distance we want to ping for (in centimeters)

// VARIABLES //

// Miscellaneous
int no_detection_ping_counter = 0;
int awake_increment = 3;
int detection_distance;                 // Distance detected by the sonar
int detection_min = 10;                 // Minimum distance to be considered a person
int detection_max = 150;                // Maximum distance to be considered a person
int temperature_min = 30;               // Minimum temperature to be considered a person
int temperature_max = 40;               // Maximum temperature to be considered a person
int temperature;                        // Detected temperature
int person_detected;                    // Tells if a person has been detected
int person_detected_lazy;               // Tells if a person has been detected
int peopleCounter = 0;                  // Counter for crowd detection
int totalCounter = 0;                   // Counter for crowd detection
int isCrowdPercentage = 0.5;             // Percentage for which a crowd is detected

// Sensors
NewPing detection_sonar(TRIGGER_PIN_DETECTION, ECHO_PIN_DETECTION, MAX_DISTANCE_DETECTION);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// CODE //

void setup() {
  setupPeopleDetection();
}

void loop() {
  temperature = mlx.readObjectTempC();
  Serial.print("No person detected, with temperature: ");
  Serial.println(temperature);
}

// SUPPORT FUNCTIONS //

// Setup people detection
void setupPeopleDetection() {
  person_detected = false;
  mlx.begin();
}

// Manage people detections via sonar
void peopleDetect() {
  // Acquire the distance
  detection_distance = detection_sonar.ping_cm();
  // Acquire the temperature
  temperature = mlx.readObjectTempC();
  if ((detection_distance > detection_min && detection_distance < detection_max) && (temperature > temperature_min && temperature < temperature_max)) {
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

// Process the detection of people. This lazy version doesn't call setPersonDetected
void processPeopleDetectionLazy() {
  if (no_detection_ping_counter >= 12) {
    // Normalize the value
    no_detection_ping_counter = 12;
    if (person_detected_lazy) {
      Serial.println("SWITCHED TO NO PERSON DETECTED");
      person_detected_lazy = false;
    }
  } else if (no_detection_ping_counter <= 0) {
    // Normalize the value
    no_detection_ping_counter = 0;
    if (!person_detected_lazy) {
      Serial.println("SWITCHED TO PERSON DETECTED");
      person_detected_lazy = true;
    }
  }
}

bool isPersonLazy() {
  return person_detected_lazy;
}

// Manage the robot looking for people. This makes the state of the robot change.
void setPersonDetected(bool b) {
  if (b) {
    // Start looking for person
    person_detected = true;
    // K_STATE - Move towards the person
  } else {
    // Stop looking for person
    person_detected = false;
    // Set the sense of rotation
  }
}

// Counts how much people there is
void peopleCount() {
  // Acquire the temperature
  temperature = mlx.readObjectTempC();
  // Process
  if (temperature > temperature_min && temperature < temperature_max)
    peopleCounter++;
  else
    totalCounter++;
}


// Process the information about the cunted people
bool isCrowd() {
  if (totalCounter / peopleCounter > isCrowdPercentage) {
    // Reset values for the next call
    peopleCounter = 0;
    totalCounter = 0;
    return true;
  } else {
    // Reset values for the next call
    peopleCounter = 0;
    totalCounter = 0;
    return false;
  }
}
