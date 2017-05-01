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
int detection_treshold_handle = 1000;   // A person is detected if the input is lower than this
int detection_treshold_foam = 4000;     // A person is detected if the input is higher than this
bool person_detected_handle;            // Tells if a person is touching the handle
bool person_detected_foam;              // Tells if a person is touching the foam
long touch_handle;                      // Output of the handle touch sensor
long touch_foam;                        // Output of the handle foam sensor

// SUPPORT FUNCTIONS //

// Setup touch detection
void setupTouchDetection() {
  person_detected_handle = false;
  person_detected_foam = false;
}

// Manage unexpected touch
void unexpectedTouchDetection() {
  // Get the input
  touch_handle = cs_handle.capacitiveSensor(30);
  touch_foam = cs_handle.capacitiveSensor(30);

  // Process the handle data
  if (touch_handle < detection_treshold_handle && !person_detected_handle) {
    person_detected_handle = true;
    // Go to state 28 (HANDLE TOUCHED SURPRISED)
    Serial.print("Handle touched: ");
    Serial.println(touch_handle);
    setState(28);
  } else if (touch_handle >= detection_treshold_handle && person_detected_handle) {
    person_detected_handle = false;
  }

  // Process the foam data
  if (touch_foam > detection_treshold_foam && !person_detected_foam) {
    person_detected_foam = true;
    Serial.print("Foam touched: ");
    Serial.println(touch_foam);
    // Go to state 22 (FOAM TOUCHED SURPRISED)
    setState(22);
  } else if (touch_foam <= detection_treshold_foam && person_detected_foam) {
    person_detected_foam = false;
  }
}

// Manage expected touch
void expectedTouchDetection() {
  // Get the input
  touch_handle = cs_handle.capacitiveSensor(30);
  touch_foam = cs_handle.capacitiveSensor(30);

  // Process the handle data
  if (touch_handle < detection_treshold_handle && !person_detected_handle) {
    person_detected_handle = true;
    Serial.print("Handle touched: ");
    Serial.println(touch_handle);
  } else if (touch_handle >= detection_treshold_handle && person_detected_handle) {
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
  return person_detected_handle;
}

bool getFoamTouched () {
  return person_detected_foam;
}
