// TOUCH DETECTION SUPPORT FUNCTIONS //

// INCLUDES //

#include <CapacitiveSensor.h>

// VARIABLES //

// Sensors
CapacitiveSensor cs_handle = CapacitiveSensor(8, 4);    // Use pins 27 and 26
CapacitiveSensor cs_foam = CapacitiveSensor(29, 28);    // Use pins 29 and 28

// Miscellaneous
int detection_treshold_handle = 80;     // A person is detected if the input is higher than this
int detection_treshold_foam = 80;       // A person is detected if the input is higher than this
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
  if (touch_handle > detection_treshold_handle && !person_detected_handle) {
    person_detected_handle = true;
    // Go to state 28 (HANDLE TOUCHED SURPRISED)
    setState(28);
  } else if (touch_handle <= detection_treshold_handle && person_detected_handle) {
    person_detected_handle = false;
  }

  // Process the foam data
  if (touch_foam > detection_treshold_foam && !person_detected_foam) {
    person_detected_foam = true;
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
  if (touch_handle > detection_treshold_handle && !person_detected_handle) {
    person_detected_handle = true;
  } else if (touch_handle <= detection_treshold_handle && person_detected_handle) {
    person_detected_handle = false;
  }

  // Process the foam data
  if (touch_foam > detection_treshold_foam && !person_detected_foam) {
    person_detected_foam = true;
    Serial.print("Person detected: ");
    Serial.println(person_detected_foam);
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
