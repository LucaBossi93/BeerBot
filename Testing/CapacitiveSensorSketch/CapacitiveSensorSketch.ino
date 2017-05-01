// TOUCH DETECTION SUPPORT FUNCTIONS //

// INCLUDES //

#include <CapacitiveSensor.h>

// VARIABLES //

// Sensors
CapacitiveSensor cs_handle = CapacitiveSensor(8, 9);    // Use pins 27 and 26
CapacitiveSensor cs_foam = CapacitiveSensor(10, 11);    // Use pins 29 and 28

// Miscellaneous
int detection_treshold_handle = 3500;     // A person is detected if the input is higher than this
int detection_treshold_foam = 3550;       // A person is detected if the input is higher than this
bool person_detected_handle;            // Tells if a person is touching the handle
bool person_detected_foam;              // Tells if a person is touching the foam
long touch_handle;                      // Output of the handle touch sensor
long touch_foam;                        // Output of the handle foam sensor

// SUPPORT FUNCTIONS //

void setup() {
  setupTouchDetection();
  Serial.begin(9600);
}

void loop() {
  expectedTouchDetection();
  delay(1000);
}
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
    // TODO - Set state and do something
  } else if (touch_handle <= detection_treshold_handle && person_detected_handle) {
    person_detected_handle = false;
    // TODO - Set state and do something
  }

  // Process the foam data
  if (touch_foam > detection_treshold_foam && !person_detected_foam) {
    person_detected_foam = true;
    // TODO - Set state and do something
  } else if (touch_foam <= detection_treshold_foam && person_detected_foam) {
    person_detected_foam = false;
    // TODO - Set state and do something
  }
}

// Manage expected touch
void expectedTouchDetection() {
  // Get the input
  touch_handle = cs_handle.capacitiveSensor(30);
  touch_foam = cs_handle.capacitiveSensor(30);
  Serial.print("Person detected handle: ");
  Serial.println(touch_handle);
  Serial.print("Person detected foam: ");
  Serial.println(touch_foam);


  // Process the handle data
  if (touch_handle > detection_treshold_handle && !person_detected_handle) {
    person_detected_handle = true;

  } else if (touch_handle <= detection_treshold_handle && person_detected_handle) {
    person_detected_handle = false;
  }

  // Process the foam data
  if (touch_foam > detection_treshold_foam && !person_detected_foam) {
    person_detected_foam = true;

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
