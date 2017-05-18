// TOUCH DETECTION SUPPORT FUNCTIONS //

// INCLUDES //

#include <CapacitiveSensor.h>

// DEFINITIONS //

#define HANDLE_PIN_A 10                 // First pin of the handle capacitive sensor
#define HANDLE_PIN_B 11                 // Second pin of the handle capacitive sensor
#define FOAM_PIN_A 8                    // First pin of the foam capacitive sensor
#define FOAM_PIN_B 9                    // Second pin of the foam capacitive sensor

// VARIABLES //

// Sensors
CapacitiveSensor cs_handle = CapacitiveSensor(HANDLE_PIN_A, HANDLE_PIN_B);
CapacitiveSensor cs_foam = CapacitiveSensor(FOAM_PIN_A, FOAM_PIN_B);

// Miscellaneous 
int detection_treshold_handle = -2;     // A person is detected if the input has this value
int detection_treshold_foam = 1000;     // A person is detected if the input is higher than this
bool person_detected_handle;            // Tells if a person is touching the handle
bool person_detected_foam;              // Tells if a person is touching the foam
long touch_handle;                      // Output of the handle touch sensor
long touch_foam;                        // Output of the handle foam sensor
bool canDesaturate;                     // Tells if I can desaturate the capacitive sensor
int desaturationTime = 5000;            // Time I have to desaturate

// Millis
long starting_time_desaturation;          // Tells when the robot starts to desaturate

// SUPPORT FUNCTIONS //

// Desaturate the capacitive sensor
void menageCapacitive() {
  if (canDesaturate) {
    // Desaturate
    digitalWrite(HANDLE_PIN_A, LOW);
    digitalWrite(HANDLE_PIN_B, LOW);
    digitalWrite(FOAM_PIN_A, LOW);
    digitalWrite(FOAM_PIN_B, LOW);
    // Stop if needed
    if (millis() - starting_time_desaturation > desaturationTime) {
      canDesaturate = false;
    }
  }
}

// Start desaturation
void startDesaturation() {
  if (!canDesaturate) {
    starting_time_desaturation = millis();
    canDesaturate = true;
  }
}

// Setup touch detection
void setupTouchDetection() {
  cs_foam.set_CS_Timeout_Millis(50);
  cs_handle.set_CS_Timeout_Millis(50);
  person_detected_handle = false;
  person_detected_foam = false;
}

// Manage unexpected touch
void unexpectedTouchDetection() {
  // If I'm desaturating I don't do anything
  if (!canDesaturate) {
    // Get the input
    touch_handle = cs_handle.capacitiveSensor(30);
    touch_foam = cs_foam.capacitiveSensor(30);

    // Process the handle data
    if (touch_handle == detection_treshold_handle && !person_detected_handle) {
      person_detected_handle = true;
      Serial.print("Handle touched: ");
      Serial.println(touch_handle);
      // Go to state 28 (HANDLE TOUCHED SURPRISED)
      setState(28);
      // Start desaturation
      startDesaturation();
    } else if (touch_handle != detection_treshold_handle && person_detected_handle) {
      person_detected_handle = false;
    }

    // Process the foam data
    if (touch_foam > detection_treshold_foam && !person_detected_foam) {
      person_detected_foam = true;
      Serial.print("Foam touched: ");
      Serial.println(touch_foam);
      // Go to state 22 (FOAM TOUCHED SURPRISED)
      setState(22);
      // Start desaturation
      startDesaturation();
    } else if (touch_foam <= detection_treshold_foam && person_detected_foam) {
      person_detected_foam = false;
    }
  }
}

// Manage expected touch
void expectedTouchDetection() {
  // Stop desaturation if needed
  if (canDesaturate)
    canDesaturate = false;

  // Get the input
  touch_handle = cs_handle.capacitiveSensor(30);
  touch_foam = cs_foam.capacitiveSensor(30);
  // Serial.print("Touch values: handle = ");
  // Serial.print(touch_handle);
  // Serial.print(", foam = ");
  // Serial.println(touch_foam);
  
  // Process the handle data
  if (touch_handle == detection_treshold_handle && !person_detected_handle) {
    person_detected_handle = true;
    Serial.print("Handle touched: ");
    Serial.println(touch_handle);
  } else if (touch_handle != detection_treshold_handle && person_detected_handle) {
    person_detected_handle = false;
  }

  // Process the foam data
  if (touch_foam > detection_treshold_foam && !person_detected_foam) {
    person_detected_foam = true;
    Serial.print("Foam touched: ");
    Serial.println(touch_foam);
  } else if (touch_foam <= detection_treshold_foam && person_detected_foam) {
    person_detected_foam = false;
  }
}

bool getHandleTouched () {
  // Start desaturation
  if (person_detected_handle)
    // Start desaturation
    startDesaturation();

  return person_detected_handle;
}

bool getFoamTouched () {
  // Start desaturation
  if (person_detected_foam)
    // Start desaturation
    startDesaturation();

  return person_detected_foam;
}
